#pragma once

#include "../defines.h"

#if defined(USE_SENSORS)

#include "sensor.h"
#include "../ads1115.h"

#define ADS1115_DEFAULT_SCALE   1
#define ADS1115_DEFAULT_OFFSET  0

class ADS1115Sensor : public Sensor {
	public:
	ADS1115Sensor(uint32_t interval, float min, float max, const char *name, SensorUnit unit, uint16_t flag, ADS1115 **sensors, uint8_t sensor_index, uint8_t pin, float scale, float offset);
	ADS1115Sensor(ADS1115 **sensors, char *buf, uint32_t len);

	void emit_extra_json(BufferFiller *bfill);
	static void emit_description_json(BufferFiller *bfill);

	SensorType get_sensor_type() {
		return SensorType::ADS1115;
	}

	uint8_t sensor_index;
	uint8_t pin;
	float scale;
	float offset;

	private:
	float _get_raw_value();
	uint32_t _serialize_internal(char *buf);

	ADS1115 **sensors;
};

#endif
