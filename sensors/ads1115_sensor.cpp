#include "ads1115_sensor.h"

#if defined(USE_SENSORS)

ADS1115Sensor::ADS1115Sensor(uint32_t interval, float min, float max, const char* name, SensorUnit unit, uint16_t flag, ADS1115** sensors, uint8_t sensor_index, uint8_t pin, float scale, float offset) :
	Sensor(interval, min, max, name, unit, flag),
	sensor_index(sensor_index),
	pin(pin),
	scale(scale),
	offset(offset),
	sensors(sensors) {}

void ADS1115Sensor::emit_extra_json(BufferFiller *bfill) {
	bfill->emit_p(PSTR("{\"pin\":$D,\"scale\":$E,\"offset\":$E}"),
		((this->sensor_index << 2) + this->pin + 1),
		this->scale,
		this->offset);
}

void ADS1115Sensor::emit_description_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR(
		"{\"name\":\"ADS1115 Sensor\","
		"\"args\":["
			"{\"name\":\"Pin Number\","
			 "\"arg\":\"pin\","
			 "\"type\":\"int::[1,16]\","
			 "\"default\":\"1\"},"
			"{\"name\":\"Linear Scale\","
			 "\"arg\":\"scale\","
			 "\"type\":\"float\","
			 "\"default\":\"" SENSOR_DEFAULT_STR(ADS1115_DEFAULT_SCALE) "\"},"
			"{\"name\":\"Value Offset\","
			 "\"arg\":\"offset\","
			 "\"type\":\"float\","
			 "\"default\":\"" SENSOR_DEFAULT_STR(ADS1115_DEFAULT_OFFSET) "\"}"
		"]}"
	));
}

float ADS1115Sensor::get_initial_value() {
	return 0.0;
}

float ADS1115Sensor::_get_raw_value() {
	if (this->sensors[sensor_index] == nullptr) {
		return 0.0;
	}
	float raw = ((float)this->sensors[sensor_index]->get_pin_value(this->pin)) * ADS1115_SCALE_FACTOR;
	return raw * this->scale + this->offset;
}

uint32_t ADS1115Sensor::_serialize_internal(char *buf) {
	uint32_t i = 0;
	buf[i++] = static_cast<uint8_t>(this->sensor_index);
	buf[i++] = static_cast<uint8_t>(this->pin);
	i += write_buf<float>(buf + i, this->scale);
	i += write_buf<float>(buf + i, this->offset);
	return i;
}

ADS1115Sensor::ADS1115Sensor(ADS1115 **sensors, char *buf, uint32_t len) {
	uint8_t subclass_len = 0;
	uint32_t i = Sensor::_deserialize(buf, len, &subclass_len);
	uint32_t end = i + subclass_len;

	this->sensor_index = 0;
	this->pin = 0;
	this->scale = ADS1115_DEFAULT_SCALE;
	this->offset = ADS1115_DEFAULT_OFFSET;

	if (i + 1 <= end) this->sensor_index = static_cast<uint8_t>(buf[i]);
	i++;
	if (i + 1 <= end) this->pin = static_cast<uint8_t>(buf[i]);
	i++;
	if (i + sizeof(float) <= end) this->scale = read_buf<float>(buf, &i);
	if (i + sizeof(float) <= end) this->offset = read_buf<float>(buf, &i);
	this->sensors = sensors;
}

#endif
