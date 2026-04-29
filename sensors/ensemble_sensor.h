#pragma once

#include "sensor.h"

typedef struct {
	float min;
	float max;
	float scale;
	float offset;
	uint16_t uuid;   // UUID of child sensor (SENSOR_UUID_NONE = unused slot)
} ensemble_children_t;

#define ENSEMBLE_SENSOR_CHILDREN_COUNT 4

class EnsembleSensor : public Sensor {
	public:
	EnsembleSensor(uint32_t interval, float min, float max, float scale, float offset, const char *name, SensorUnit unit, uint16_t flags, sensor_memory_t *sensors, ensemble_children_t *children, uint8_t children_count, EnsembleAction action);
	EnsembleSensor(sensor_memory_t *sensors, char *buf);

	void emit_extra_json(BufferFiller *bfill);
	static void emit_description_json(BufferFiller *bfill);

	SensorType get_sensor_type() {
		return SensorType::Ensemble;
	}

	ensemble_children_t children[ENSEMBLE_SENSOR_CHILDREN_COUNT];
	EnsembleAction action;

	float get_initial_value();

	private:
	float _get_raw_value();
	uint32_t _serialize_internal(char *buf);

	sensor_memory_t *sensors;
};
