#pragma once

// External / analog sensor subsystem
//
// The types in this file (Sensor, EnsembleSensor, WeatherSensor, ADS1115Sensor,
// SensorAdjustment, etc.) model the *external* sensor board — an ADS1115-based
// I2C ADC add-on that reads analog probes (soil moisture, temperature, etc.).
// It is enabled by the USE_SENSORS compile-time guard.
//
// These are DISTINCT from the two onboard digital sensor inputs (SENSOR1 /
// SENSOR2) that are wired directly to GPIO pins.  Those are simple binary
// (open/closed) or pulse inputs handled entirely in OpenSprinkler.h/.cpp via
// os.sensor1_status / os.sensor2_status and related IOPT_SENSOR* options.
// No types from this file are involved in the onboard sensor logic.

#include <stdint.h>
#include "utils.h"
#include "defines.h"
#include "bfiller.h"

#define SENSOR_NAME_LEN 33
#define SENSOR_CUSTOM_UNIT_LEN 9

#define SENSOR_UUID_NONE 0  // sentinel: "no sensor assigned" (0 = uninitialized/disabled)

typedef struct {
	uint32_t interval;
	uint32_t next_update;
	float    value;
	uint16_t uuid;   // stable sensor identifier (0 = empty slot)
	uint16_t flags;  // was uint32_t; only 2 bits used, uint16_t keeps struct at 16 bytes
} sensor_memory_t;

// Sensor log file format — both structs are tightly packed (no padding) so
// sizeof() gives the exact on-disk byte count and offsetof() gives exact field offsets.
struct __attribute__((packed)) SensorLogHeader {
	uint8_t  magic;            // SENSOR_LOG_MAGIC
	uint8_t  version;          // SENSOR_LOG_VERSION
	uint16_t max_files;        // number of data files in rotation
	uint16_t records_per_file; // max records per data file
	uint16_t cur_file;         // index of the data file currently being written
	uint8_t  wrapped;          // 1 once all max_files slots have been used at least once
	uint8_t  reserved[7];
};  // 16 bytes

struct __attribute__((packed)) SensorLogRecord {
	uint32_t timestamp;    // unix epoch seconds
	float    value;        // sensor reading
	uint16_t uuid;         // sensor UUID (replaces sid+reserved, same 10-byte size)
};

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
	Sensor(uint32_t interval, float min, float max, float scale, float offset, const char *name, SensorUnit unit, uint16_t flags);
	Sensor();
	virtual ~Sensor() {}

	float get_new_value();
	uint32_t serialize(char *buf);

	static Sensor *parse(os_file_type file);         // statically allocated, do not delete
	static Sensor *get(uint8_t index);               // statically allocated, do not delete
	static void    write(Sensor *sensor, uint8_t index);
	static void    load_count();
	static void    save_count();
	static unsigned char add(Sensor *sensor);
	static unsigned char modify(uint8_t index, Sensor *sensor); // index is positional index
	static unsigned char del(uint8_t index); // index is positional index
	static void          load_all();
	static uint8_t       find_index(uint16_t uuid);
	static void          test_log(uint32_t n_records);

	void virtual emit_extra_json(BufferFiller *bfill) = 0;

	uint32_t interval = 1;
	float min = 0.f;
	float max = 0.f;
	float scale = 0.f;
	float offset = 0.f;
	uint16_t flags = 0;
	uint16_t uuid = 0;   // assigned by write_sensor on creation; 0 = not yet assigned
	SensorUnit unit = SensorUnit::None;
	char name[SENSOR_NAME_LEN] = {0};

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

enum class WeatherAction {
	MAX_VALUE,
};

typedef float (*WeatherGetter)(WeatherAction);

class WeatherSensor : public Sensor {
	public:
	WeatherSensor(uint32_t interval, float min, float max, float scale, float offset, const char *name, SensorUnit unit, uint16_t flags, WeatherGetter weather_getter, WeatherAction action);
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
	SensorAdjustment(uint8_t flags, uint16_t uuid, uint8_t point_count, sensor_adjustment_point_t *points);

	static SensorAdjustment *read(uint8_t index, uint8_t nprograms); // returns statically allocated object, do not delete
	static void              write(SensorAdjustment *adj, uint8_t index);

	float get_adjustment_factor(sensor_memory_t *sensors);

	sensor_adjustment_point_t points[SENSOR_ADJUSTMENT_POINTS];
	uint16_t uuid;        // sensor UUID (SENSOR_UUID_NONE = adjustment disabled)
	uint8_t  flags;
	uint8_t  point_count;
};

#define SENSOR_ADJUSTMENT_SIZE sizeof(SensorAdjustment)

const char *enum_string(SensorUnitGroup group);
const char *enum_string(EnsembleAction action);
const char *enum_string(WeatherAction action);

const char* get_sensor_unit_name(SensorUnit unit);
const char* get_sensor_unit_short(SensorUnit unit);
const SensorUnitGroup get_sensor_unit_group(SensorUnit unit);
const uint32_t get_sensor_unit_index(SensorUnit unit);

// Sensor log file helpers
void         get_sensor_log_filename(char *buf, uint16_t file_no);
os_file_type open_sensor_log(uint16_t file_no, FileOpenMode mode);
os_file_type open_sensor_log_header(FileOpenMode mode);
void         remove_sensor_log(int16_t file_no = -1);  // -1 removes header + all data files
