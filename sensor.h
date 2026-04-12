#pragma once

#include <stdint.h>
#if defined(ARDUINO)
#include <Arduino.h>
#else
#include "utils.h"
#endif
#include "defines.h"
#include "bfiller.h"

#define SENSOR_NAME_LEN 33
#define SENSOR_CUSTOM_UNIT_LEN 9

typedef struct {
	uint32_t interval;
	uint32_t flags;
	uint32_t next_update;
	float value;
} sensor_memory_t;

enum class SensorType : uint8_t {
	Ensemble = 0,
	ADS1115,
	Weather,
	MAX_VALUE,
};

enum class SensorUnitGroup : uint8_t {
	None = 0,
	Temperature,
	Length,
	Volume,
	Light,
	Energy,
	Velocity,
	Pressure,
	Flow,
	MAX_VALUE,
};

enum class SensorUnit : uint8_t {
	None = 0,
	Celsius,
	Fahrenheit,
	Kelvin,
	Millimeter,
	Centimeter,
	Meter,
	Kilometer,
	Inch,
	Foot,
	Mile,
	Lux,
	Lumen,
	Millivolt,
	Volt,
	Milliampere,
	Ampere,
	Percent,
	MilesPerHour,
	KilometersPerHour,
	MetersPerSecond,
	DielectricConstant,
	PartsPerMillion,
	Ohm,
	Milliohm,
	Kiloohm,
	Bar,
	Kilopascal,
	Pascal,
	Torr,
	LitersPerSecond,
	GallonsPerSecond,
	MAX_VALUE,
};

typedef enum {
	SENSOR_FLAG_ENABLE = 0,
	SENSOR_FLAG_LOG,
	SENSOR_FLAG_COUNT
} sensor_flags;

class Sensor {
public:
	Sensor(uint32_t interval, float min, float max, float scale, float offset, const char *name, SensorUnit unit, uint32_t flags);
	Sensor();
	virtual ~Sensor() {}

	float get_new_value();
	uint32_t serialize(char *buf);

	void virtual emit_extra_json(BufferFiller *bfill) = 0;

	uint32_t interval = 1;
	float min = 0.f;
	float max = 0.f;
	float scale = 0.f;
	float offset = 0.f;
	char name[SENSOR_NAME_LEN] = {0};
	SensorUnit unit = SensorUnit::None;

	uint32_t flags = 0;

	SensorType virtual get_sensor_type() = 0;
	float virtual get_initial_value() = 0;

	private:
	float virtual _get_raw_value() = 0;
	protected:
	uint32_t _deserialize(char *buf);
	uint32_t virtual _serialize_internal(char *buf) = 0;
};

enum class EnsembleAction : uint8_t {
	Min = 0,
	Max,
	Average,
	Sum,
	Product,
	MAX_VALUE,
};

typedef Sensor* (*SensorGetter)(uint8_t);

typedef struct {
	uint8_t sensor_id;
	float min;
	float max;
	float scale;
	float offset;
} ensemble_children_t;

#define ENSEMBLE_SENSOR_CHILDREN_COUNT 4

class EnsembleSensor : public Sensor {
	public:
	EnsembleSensor(uint32_t interval, float min, float max, float scale, float offset, const char *name, SensorUnit unit, uint32_t flags, sensor_memory_t *sensors, ensemble_children_t *children, uint8_t children_count, EnsembleAction action);
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

enum class WeatherAction {
	MAX_VALUE,
};

typedef float (*WeatherGetter)(WeatherAction);

class WeatherSensor : public Sensor {
	public:
	WeatherSensor(uint32_t interval, float min, float max, float scale, float offset, const char *name, SensorUnit unit, uint32_t flags, WeatherGetter weather_getter, WeatherAction action);
	WeatherSensor(WeatherGetter weather_getter, char *buf);

	void emit_extra_json(BufferFiller *bfill);
	static void emit_description_json(BufferFiller *bfill);

	SensorType get_sensor_type() {
		return SensorType::Weather;
	}

	WeatherAction action;

	float get_initial_value();

	private:
	float _get_raw_value();
	uint32_t _serialize_internal(char *buf);
	
	WeatherGetter weather_getter;
};

typedef struct {
	float x;
	float y;
} sensor_adjustment_point_t;

#define SENSOR_ADJUSTMENT_POINTS 8

typedef enum {
	SENADJ_FLAG_ENABLE = 0,
} senadj_flags;

class SensorAdjustment {
public:
	SensorAdjustment(uint8_t flags, uint8_t sid, uint8_t point_count, sensor_adjustment_point_t *points);
	SensorAdjustment(char *buf);

	float get_adjustment_factor(sensor_memory_t *sensors);
	uint32_t serialize(char *buf);

	uint8_t flags;
	uint8_t sid;
	uint8_t point_count;
	sensor_adjustment_point_t points[SENSOR_ADJUSTMENT_POINTS];
};

#define SENSOR_ADJUSTMENT_SIZE (3 + (SENSOR_ADJUSTMENT_POINTS * sizeof(sensor_adjustment_point_t)))

const char *enum_string(SensorUnitGroup group);
const char *enum_string(EnsembleAction action);
const char *enum_string(WeatherAction action);

const char* get_sensor_unit_name(SensorUnit unit);
const char* get_sensor_unit_short(SensorUnit unit);
const SensorUnitGroup get_sensor_unit_group(SensorUnit unit);
const uint32_t get_sensor_unit_index(SensorUnit unit);
