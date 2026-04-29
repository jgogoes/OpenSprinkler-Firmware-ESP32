#pragma once

#include "../defines.h"

#if defined(USE_SENSORS)

#include "sensor.h"
#include "../ads1115.h"

class ADS1115Sensor : public Sensor {
	public:
	ADS1115Sensor(uint32_t interval, float min, float max, float scale, float offset, const char *name, SensorUnit unit, uint16_t flags, ADS1115 **sensors, uint8_t sensor_index, uint8_t pin);
	ADS1115Sensor(ADS1115 **sensors, char *buf);

	void emit_extra_json(BufferFiller *bfill);
	static void emit_description_json(BufferFiller *bfill);

	SensorType get_sensor_type() {
		return SensorType::ADS1115;
	}

	uint8_t sensor_index;
	uint8_t pin;

	float get_initial_value();

	private:
	float _get_raw_value();
	uint32_t _serialize_internal(char *buf);

	ADS1115 **sensors;
};

#endif
