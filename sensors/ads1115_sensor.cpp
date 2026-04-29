#include "ads1115_sensor.h"

#if defined(USE_SENSORS)

ADS1115Sensor::ADS1115Sensor(uint32_t interval, float min, float max, float scale, float offset, const char* name, SensorUnit unit, uint16_t flags, ADS1115** sensors, uint8_t sensor_index, uint8_t pin) :
	Sensor(interval, min, max, scale, offset, name, unit, flags),
	sensor_index(sensor_index),
	pin(pin),
	sensors(sensors) {}

void ADS1115Sensor::emit_extra_json(BufferFiller *bfill) {
	bfill->emit_p(PSTR("{\"pin\":$D}"), ((this->sensor_index << 2) + this->pin + 1));
}

void ADS1115Sensor::emit_description_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR(
		"{\"name\":\"ADS1115 Sensor\","
		"\"args\":["
			"{\"name\":\"Pin Number\","
			 "\"arg\":\"pin\","
			 "\"type\":\"int::[1,16]\","
			 "\"default\":\"1\"}"
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
	else {
		return ((float)this->sensors[sensor_index]->get_pin_value(this->pin)) * ADS1115_SCALE_FACTOR;
	}
}

uint32_t ADS1115Sensor::_serialize_internal(char *buf) {
	uint32_t i = 0;
	buf[i++] = static_cast<uint8_t>(this->sensor_index);
	buf[i++] = static_cast<uint8_t>(this->pin);
	return i;
}

ADS1115Sensor::ADS1115Sensor(ADS1115 **sensors, char *buf) {
	uint32_t i = Sensor::_deserialize(buf);
	this->sensor_index = static_cast<uint8_t>(buf[i++]);
	this->pin = static_cast<uint8_t>(buf[i++]);
	this->sensors = sensors;
}

#endif
