#pragma once

#include <float.h>
#include "sensor.h"

typedef struct {
	float min;
	float max;
	uint16_t uuid;   // UUID of child sensor (SENSOR_UUID_NONE = unused slot)
} aggregate_children_t;

#define AGGREGATE_SENSOR_CHILDREN_COUNT 8

// Defaults for newly-initialized child slots
#define AGGREGATE_CHILD_DEFAULT_MIN  (-FLT_MAX)
#define AGGREGATE_CHILD_DEFAULT_MAX  (FLT_MAX)

class AggregateSensor : public Sensor {
	public:
	AggregateSensor(uint32_t interval, float min, float max, const char *name, SensorUnit unit, uint16_t flag, sensor_memory_t *sensors, aggregate_children_t *children, uint8_t children_count, AggregateAction action);
	AggregateSensor(sensor_memory_t *sensors, char *buf);

	void emit_extra_json(BufferFiller *bfill);
	static void emit_description_json(BufferFiller *bfill);

	SensorType get_sensor_type() {
		return SensorType::Aggregate;
	}

	aggregate_children_t children[AGGREGATE_SENSOR_CHILDREN_COUNT];
	AggregateAction action;

	float get_initial_value();

	private:
	float _get_raw_value();
	uint32_t _serialize_internal(char *buf);

	sensor_memory_t *sensors;
};
