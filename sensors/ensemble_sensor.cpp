#include "ensemble_sensor.h"
#include "../OpenSprinkler.h"

extern OpenSprinkler os;

EnsembleSensor::EnsembleSensor(uint32_t interval, float min, float max, float scale, float offset, const char* name, SensorUnit unit, uint16_t flags, sensor_memory_t* sensors, ensemble_children_t* children, uint8_t children_count, EnsembleAction action) :
	Sensor(interval, min, max, scale, offset, name, unit, flags),
	action(action),
	sensors(sensors) {
	for (size_t i = 0; i < ENSEMBLE_SENSOR_CHILDREN_COUNT; i++) {
		if (i < children_count) {
			this->children[i] = children[i];
		}
		else {
			this->children[i] = ensemble_children_t{ 0.f, 0.f, 0.f, 0.f, SENSOR_UUID_NONE };
		}
	}
}

void EnsembleSensor::emit_extra_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR("{\"action\":$D,\"children\":["), this->action);
	for (size_t i = 0; i < ENSEMBLE_SENSOR_CHILDREN_COUNT; i++) {
		if (i) bfill->emit_p(PSTR(","));
		ensemble_children_t* child = &this->children[i];
		bfill->emit_p(PSTR("{\"uuid\":$D,\"max\":$E,\"min\":$E,\"scale\":$E,\"offset\":$E}"), child->uuid, child->max, child->min, child->scale, child->offset);
	}
	bfill->emit_p(PSTR("]}"));
}

void EnsembleSensor::emit_description_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR(
		"{\"name\":\"Ensemble Sensor\","
		"\"args\":["
			"{\"name\":\"Argument Sensors\","
			 "\"arg\":\"children\","
			 "\"type\":\"array::4\","
			 "\"extra\":["
				"{\"name\":\"Sensor ID\",\"arg\":\"sid\",\"type\":\"sensor\",\"default\":\"\"},"
				"{\"name\":\"Minimum Value\",\"arg\":\"min\",\"type\":\"float\",\"default\":\"\"},"
				"{\"name\":\"Maximum Value\",\"arg\":\"max\",\"type\":\"float\",\"default\":\"\"},"
				"{\"name\":\"Scale\",\"arg\":\"scale\",\"type\":\"float\",\"default\":\"\"},"
				"{\"name\":\"Offset\",\"arg\":\"offset\",\"type\":\"float\",\"default\":\"\"}"
			"]},"
			"{\"name\":\"Ensemble Action\","
			 "\"arg\":\"action\","
			 "\"type\":\"enum::EnsembleAction\","
			 "\"default\":\"0\"}"
		"]}"
	));
}

float EnsembleSensor::get_initial_value() {
	switch (this->action) {
	case EnsembleAction::Min:
		return this->max;
	case EnsembleAction::Max:
		return this->min;
	case EnsembleAction::Average:
	case EnsembleAction::Sum:
		return 0;
	case EnsembleAction::Product:
		return 1;
	default:
		return 0.f;
	}
}

float EnsembleSensor::_get_raw_value() {
	float inital = this->get_initial_value();
	uint8_t count = 0;

	for (size_t i = 0; i < ENSEMBLE_SENSOR_CHILDREN_COUNT; i++) {
		uint8_t sensor = Sensor::find_index(this->children[i].uuid);
		if (sensor < OpenSprinkler::nsensors && sensors[sensor].interval) {
			float value = sensors[sensor].value;
			value = (value * this->children[i].scale) + this->children[i].offset;
			if (value < this->children[i].min) value = this->children[i].min;
			if (value > this->children[i].max) value = this->children[i].max;

			switch (this->action) {
			case EnsembleAction::Min:
				if (value < inital) inital = value;
				break;
			case EnsembleAction::Max:
				if (value > inital) inital = value;
				break;
			case EnsembleAction::Average:
			case EnsembleAction::Sum:
				inital += value;
				break;
			case EnsembleAction::Product:
				inital *= value;
				break;
			default:
				return 0.f;
			}

			count += 1;
		}
	}

	if (count == 0) {
		return 0.f;
	}
	else if (this->action == EnsembleAction::Average) {
		return inital / (float)count;
	}
	else {
		return inital;
	}
}

uint32_t EnsembleSensor::_serialize_internal(char* buf) {
	uint32_t i = 0;
	for (size_t j = 0; j < ENSEMBLE_SENSOR_CHILDREN_COUNT; j++) {
		i += write_buf<uint16_t>(buf + i, this->children[j].uuid);
		i += write_buf<float>(buf + i, this->children[j].min);
		i += write_buf<float>(buf + i, this->children[j].max);
		i += write_buf<float>(buf + i, this->children[j].scale);
		i += write_buf<float>(buf + i, this->children[j].offset);
	}

	buf[i++] = static_cast<uint8_t>(this->action);
	return i;
}

EnsembleSensor::EnsembleSensor(sensor_memory_t* sensors, char* buf) {
	uint32_t i = Sensor::_deserialize(buf);
	for (size_t j = 0; j < ENSEMBLE_SENSOR_CHILDREN_COUNT; j++) {
		this->children[j].uuid = read_buf<uint16_t>(buf, &i);
		this->children[j].min = read_buf<float>(buf, &i);
		this->children[j].max = read_buf<float>(buf, &i);
		this->children[j].scale = read_buf<float>(buf, &i);
		this->children[j].offset = read_buf<float>(buf, &i);
	}

	this->action = static_cast<EnsembleAction>(buf[i++]);
	this->sensors = sensors;
}
