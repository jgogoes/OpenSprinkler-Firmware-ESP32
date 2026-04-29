#include "aggregate_sensor.h"
#include "../OpenSprinkler.h"

extern OpenSprinkler os;

AggregateSensor::AggregateSensor(uint32_t interval, float min, float max, const char* name, SensorUnit unit, uint16_t flag, sensor_memory_t* sensors, aggregate_children_t* children, uint8_t children_count, AggregateAction action) :
	Sensor(interval, min, max, name, unit, flag),
	action(action),
	sensors(sensors) {
	for (size_t i = 0; i < AGGREGATE_SENSOR_CHILDREN_COUNT; i++) {
		if (i < children_count) {
			this->children[i] = children[i];
		} else {
			this->children[i] = aggregate_children_t{ AGGREGATE_CHILD_DEFAULT_MIN, AGGREGATE_CHILD_DEFAULT_MAX, SENSOR_UUID_NONE };
		}
	}
}

void AggregateSensor::emit_extra_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR("{\"action\":$D,\"children\":["), this->action);
	for (size_t i = 0; i < AGGREGATE_SENSOR_CHILDREN_COUNT; i++) {
		if (i) bfill->emit_p(PSTR(","));
		aggregate_children_t* child = &this->children[i];
		bfill->emit_p(PSTR("{\"uuid\":$D,\"min\":$E,\"max\":$E}"), child->uuid, child->min, child->max);
	}
	bfill->emit_p(PSTR("]}"));
}

void AggregateSensor::emit_description_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR(
		"{\"name\":\"Aggregate Sensor\","
		"\"args\":["
			"{\"name\":\"Child Sensors\","
			 "\"arg\":\"children\","
			 "\"type\":\"array::" SENSOR_DEFAULT_STR(AGGREGATE_SENSOR_CHILDREN_COUNT) "\","
			 "\"extra\":["
				"{\"name\":\"Sensor UUID\",\"arg\":\"uuid\",\"type\":\"sensor\",\"default\":\"0\"},"
				"{\"name\":\"Minimum Value\",\"arg\":\"min\",\"type\":\"float\",\"default\":\"-3.4e+38\"},"
				"{\"name\":\"Maximum Value\",\"arg\":\"max\",\"type\":\"float\",\"default\":\"3.4e+38\"}"
			"]},"
			"{\"name\":\"Aggregate Action\","
			 "\"arg\":\"action\","
			 "\"type\":\"enum::AggregateAction\","
			 "\"default\":\"0\"}"
		"]}"
	));
}

float AggregateSensor::get_initial_value() {
	return 0.0f;
}

float AggregateSensor::_get_raw_value() {
	float values[AGGREGATE_SENSOR_CHILDREN_COUNT];
	uint8_t count = 0;

	for (size_t i = 0; i < AGGREGATE_SENSOR_CHILDREN_COUNT; i++) {
		if (this->children[i].uuid == SENSOR_UUID_NONE) continue;
		uint8_t idx = Sensor::find_index(this->children[i].uuid);
		if (idx >= OpenSprinkler::nsensors || !sensors[idx].interval) continue;

		float value = sensors[idx].value;
		if (value < this->children[i].min) value = this->children[i].min;
		if (value > this->children[i].max) value = this->children[i].max;
		values[count++] = value;
	}

	if (count == 0) return 0.0f;

	switch (this->action) {
	case AggregateAction::Min: {
		float result = values[0];
		for (uint8_t i = 1; i < count; i++) if (values[i] < result) result = values[i];
		return result;
	}
	case AggregateAction::Max: {
		float result = values[0];
		for (uint8_t i = 1; i < count; i++) if (values[i] > result) result = values[i];
		return result;
	}
	case AggregateAction::Average: {
		float sum = 0;
		for (uint8_t i = 0; i < count; i++) sum += values[i];
		return sum / count;
	}
	case AggregateAction::Sum: {
		float sum = 0;
		for (uint8_t i = 0; i < count; i++) sum += values[i];
		return sum;
	}
	case AggregateAction::Median: {
		// Insertion sort (up to AGGREGATE_SENSOR_CHILDREN_COUNT elements)
		for (uint8_t i = 1; i < count; i++) {
			float key = values[i];
			int8_t j = i - 1;
			while (j >= 0 && values[j] > key) { values[j + 1] = values[j]; j--; }
			values[j + 1] = key;
		}
		if (count % 2 == 1) return values[count / 2];
		return (values[count / 2 - 1] + values[count / 2]) / 2.0f;
	}
	case AggregateAction::Range: {
		float lo = values[0], hi = values[0];
		for (uint8_t i = 1; i < count; i++) {
			if (values[i] < lo) lo = values[i];
			if (values[i] > hi) hi = values[i];
		}
		return hi - lo;
	}
	default:
		return 0.0f;
	}
}

uint32_t AggregateSensor::_serialize_internal(char* buf) {
	uint32_t i = 0;
	for (size_t j = 0; j < AGGREGATE_SENSOR_CHILDREN_COUNT; j++) {
		i += write_buf<uint16_t>(buf + i, this->children[j].uuid);
		i += write_buf<float>(buf + i, this->children[j].min);
		i += write_buf<float>(buf + i, this->children[j].max);
	}
	buf[i++] = static_cast<uint8_t>(this->action);
	return i;
}

AggregateSensor::AggregateSensor(sensor_memory_t* sensors, char* buf) {
	uint32_t i = Sensor::_deserialize(buf);
	for (size_t j = 0; j < AGGREGATE_SENSOR_CHILDREN_COUNT; j++) {
		this->children[j].uuid = read_buf<uint16_t>(buf, &i);
		this->children[j].min  = read_buf<float>(buf, &i);
		this->children[j].max  = read_buf<float>(buf, &i);
	}
	this->action = static_cast<AggregateAction>(buf[i++]);
	this->sensors = sensors;
}
