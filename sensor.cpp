#include "sensor.h"
#include "OpenSprinkler.h"

static const char* const GROUP_NAMES[] = {
	PSTR("No Group"),         // None
	"Temperature",      // Temperature
	"Length",           // Length
	"Volume",           // Volume
	"Light",            // Light
	"Energy",           // Energy
	"Velocity",         // Velocity
	"Pressure",         // Pressure
	"Flow"              // Flow
	// MAX_VALUE is handled by bounds checking
};

static const char* const ENSEMBLE_NAMES[] = {
	"Min", "Max", "Average", "Sum", "Product"
};

// --- Sensor Unit Definition Struct ---
struct UnitDefinition {
	const char* name;
	const char* short_name;
	SensorUnitGroup group;
};

// --- Master Unit Table ---
// The order MUST match the SensorUnit enum in sensor.h exactly.
static const UnitDefinition UNIT_DATA[] = {
	// Name,                  Short,   Group
	{ "None",                 "",      SensorUnitGroup::None },        // None
	{ "Celsius",              "°C",    SensorUnitGroup::Temperature }, // Celsius
	{ "Fahrenheit",           "°F",    SensorUnitGroup::Temperature }, // Fahrenheit
	{ "Kelvin",               "K",     SensorUnitGroup::Temperature }, // Kelvin
	{ "Millimeter",           "mm",    SensorUnitGroup::Length },      // Millimeter (Fixed typo)
	{ "Centimeter",           "cm",    SensorUnitGroup::Length },      // Centimeter (Fixed typo)
	{ "Meter",                "m",     SensorUnitGroup::Length },      // Meter
	{ "Kilometer",            "km",    SensorUnitGroup::Length },      // Kilometer
	{ "Inch",                 "in",    SensorUnitGroup::Length },      // Inch
	{ "Foot",                 "ft",    SensorUnitGroup::Length },      // Foot
	{ "Mile",                 "mi",    SensorUnitGroup::Length },      // Mile
	{ "Lux",                  "lx",    SensorUnitGroup::Light },       // Lux
	{ "Lumen",                "lm",    SensorUnitGroup::Light },       // Lumen
	{ "Millivolt",            "mV",    SensorUnitGroup::Energy },      // Millivolt (Fixed typo)
	{ "Volt",                 "V",     SensorUnitGroup::Energy },      // Volt
	{ "Milliampere",          "mA",    SensorUnitGroup::Energy },      // Milliampere (Fixed typo)
	{ "Ampere",               "A",     SensorUnitGroup::Energy },      // Ampere
	{ "Percent",              "%",     SensorUnitGroup::None },        // Percent
	{ "Miles Per Hour",       "mph",   SensorUnitGroup::Velocity },    // MilesPerHour
	{ "Kilometers Per Hour",  "km/h",  SensorUnitGroup::Velocity },    // KilometersPerHour
	{ "Meters Per Second",    "m/s",   SensorUnitGroup::Velocity },    // MetersPerSecond (Fixed "xxx")
	{ "Dielectric Constant",  "Dk",    SensorUnitGroup::Energy },      // DielectricConstant (Fixed typo & "xxx")
	{ "Parts Per Million",    "ppm",   SensorUnitGroup::None },        // PartsPerMillion
	{ "Ohm",                  "Ω",     SensorUnitGroup::Energy },      // Ohm
	{ "Milliohm",             "mΩ",    SensorUnitGroup::Energy },      // Milliohm (Fixed typo)
	{ "Kiloohm",              "kΩ",    SensorUnitGroup::Energy },      // Kiloohm
	{ "Bar",                  "bar",   SensorUnitGroup::Pressure },    // Bar
	{ "Kilopascal",           "kPa",   SensorUnitGroup::Pressure },    // Kilopascal
	{ "Pascal",               "Pa",    SensorUnitGroup::Pressure },    // Pascal
	{ "Torr",                 "torr",  SensorUnitGroup::Pressure },    // Torr
	{ "Liters Per Second",    "L/s",   SensorUnitGroup::Flow },        // LitersPerSecond
	{ "Gallons Per Second",   "gal/s", SensorUnitGroup::Flow }         // GallonsPerSecond
};

// TODO: PSTR()?
const char *enum_string(SensorUnitGroup group) {
	if (static_cast<int>(group) >= static_cast<int>(SensorUnitGroup::MAX_VALUE)) {
		return nullptr;
	}
	return GROUP_NAMES[static_cast<int>(group)];
}

const char *enum_string(EnsembleAction action) {
	if (static_cast<int>(action) >= static_cast<int>(EnsembleAction::MAX_VALUE)) {
		return nullptr;
	}
	return ENSEMBLE_NAMES[static_cast<int>(action)];
}

const char *enum_string(WeatherAction action) {
	// TODO: Currently empty in your original code
	return nullptr;
}

const char* get_sensor_unit_name(SensorUnit unit) {
	if (static_cast<int>(unit) >= static_cast<int>(SensorUnit::MAX_VALUE)) {
		return nullptr;
	}
	return UNIT_DATA[static_cast<int>(unit)].name;
}

const char* get_sensor_unit_short(SensorUnit unit) {
	if (static_cast<int>(unit) >= static_cast<int>(SensorUnit::MAX_VALUE)) {
		return nullptr;
	}
	return UNIT_DATA[static_cast<int>(unit)].short_name;
}

const SensorUnitGroup get_sensor_unit_group(SensorUnit unit) {
	if (static_cast<int>(unit) >= static_cast<int>(SensorUnit::MAX_VALUE)) {
		return SensorUnitGroup::MAX_VALUE;
	}
	return UNIT_DATA[static_cast<int>(unit)].group;
}

// TODO: it was returning 0 for everything, why?
const uint32_t get_sensor_unit_index(SensorUnit unit) {
	auto idx = static_cast<size_t>(unit);
	if (idx >= static_cast<size_t>(SensorUnit::MAX_VALUE)) {
			return 0; // or some invalid index
	}
	return static_cast<uint32_t>(idx);
}

Sensor::Sensor(uint32_t interval, double min, double max, double scale, double offset, const char* name, SensorUnit unit, uint32_t flags) :
	interval(interval), min(min), max(max), scale(scale), offset(offset), unit(unit), flags(flags) {
	strncpy(this->name, name, SENSOR_NAME_LEN);
	this->name[SENSOR_NAME_LEN - 1] = 0;
}

Sensor::Sensor() {}

double Sensor::get_new_value() {
	double value = this->_get_raw_value();
	value = (value * this->scale) + this->offset;
	if (value < this->min) value = this->min;
	if (value > this->max) value = this->max;

	return value;
}

template <typename T>
uint32_t write_buf(char* buf, T val) {
	std::memcpy(buf, &val, sizeof(val));
	return sizeof(val);
}

template <typename T>
T read_buf(char* buf, uint32_t* i) {
	T val;
	std::memcpy(&val, buf + (*i), sizeof(T));
	*i += sizeof(T);
	return val;
}


uint32_t Sensor::serialize(char* buf) {
	uint32_t i = 0;

	buf[i++] = static_cast<uint8_t>(this->get_sensor_type());
	memcpy(buf + i, this->name, SENSOR_NAME_LEN);
	i += SENSOR_NAME_LEN;
	buf[i++] = static_cast<uint8_t>(this->unit);
	i += write_buf<uint32_t>(buf + i, this->interval);
	i += write_buf<uint32_t>(buf + i, this->flags);
	i += write_buf<double>(buf + i, this->scale);
	i += write_buf<double>(buf + i, this->offset);
	i += write_buf<double>(buf + i, this->min);
	i += write_buf<double>(buf + i, this->max);

	i += this->_serialize_internal(buf + i);
	return i;
}

uint32_t Sensor::_deserialize(char* buf) {
	uint32_t i = 1; // Skip sensor type

	memcpy(this->name, buf + i, SENSOR_NAME_LEN);
	i += SENSOR_NAME_LEN;
	this->unit = static_cast<SensorUnit>(buf[i++]);
	this->interval = read_buf<uint32_t>(buf, &i);
	this->flags = read_buf<uint32_t>(buf, &i);
	this->scale = read_buf<double>(buf, &i);
	this->offset = read_buf<double>(buf, &i);
	this->min = read_buf<double>(buf, &i);
	this->max = read_buf<double>(buf, &i);

	return i;
}

EnsembleSensor::EnsembleSensor(uint32_t interval, double min, double max, double scale, double offset, const char* name, SensorUnit unit, uint32_t flags, sensor_memory_t* sensors, ensemble_children_t* children, uint8_t children_count, EnsembleAction action) :
	Sensor(interval, min, max, scale, offset, name, unit, flags),
	action(action),
	sensors(sensors) {
	for (size_t i = 0; i < ENSEMBLE_SENSOR_CHILDREN_COUNT; i++) {
		if (i < children_count) {
			this->children[i] = children[i];
		}
		else {
			this->children[i] = ensemble_children_t{ sensor_id: 255, min : 0.0, max : 0.0, scale : 0.0, offset : 0.0 };
		}
	}
}

void EnsembleSensor::emit_extra_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR("{\"action\":$D,\"children\":["), this->action);
	for (size_t i = 0; i < ENSEMBLE_SENSOR_CHILDREN_COUNT; i++) {
		if (i) bfill->emit_p(PSTR(","));
		ensemble_children_t* child = &this->children[i];
		bfill->emit_p(PSTR("{\"sid\":$D,\"max\":$E,\"min\":$E,\"scale\":$E,\"offset\":$E}"), child->sensor_id, child->max, child->min, child->scale, child->offset);
	}
	bfill->emit_p(PSTR("]}"));
}

void EnsembleSensor::emit_description_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR("{\"name\":\"Ensemble Sensor\",\"args\":[{\"name\":\"Argument Sensors\",\"arg\":\"children\",\"type\":\"array::4\",\"extra\":[{\"name\":\"Sensor ID\",\"arg\":\"sid\",\"type\":\"sensor\",\"default\":\"\",\"extra\":[]},{\"name\":\"Minimum Value\",\"arg\":\"min\",\"type\":\"double\",\"default\":\"\",\"extra\":[]},{\"name\":\"Maximum Value\",\"arg\":\"max\",\"type\":\"double\",\"default\":\"\",\"extra\":[]},{\"name\":\"Scale\",\"arg\":\"scale\",\"type\":\"double\",\"default\":\"\",\"extra\":[]},{\"name\":\"Offset\",\"arg\":\"offset\",\"type\":\"double\",\"default\":\"\",\"extra\":[]}]},{\"name\":\"Ensemble Action\",\"arg\":\"action\",\"type\":\"enum::EnsembleAction\",\"default\":\"0\",\"extra\":[]}]}"));
}

double EnsembleSensor::get_initial_value() {
	switch (this->action) {
	case EnsembleAction::Min:
		return this->max;
		break;
	case EnsembleAction::Max:
		return this->min;
		break;
	case EnsembleAction::Average:
	case EnsembleAction::Sum:
		return 0;
		break;
	case EnsembleAction::Product:
		return 1;
		break;
	default:
		// Unreachable
		return 0.0;
	}
}

double EnsembleSensor::_get_raw_value() {
	double initial = this->get_initial_value();
	uint8_t count = 0;

	for (size_t i = 0; i < ENSEMBLE_SENSOR_CHILDREN_COUNT; i++) {
		uint8_t sensor = this->children[i].sensor_id;
		if (sensor < MAX_SENSORS && sensors[sensor].interval) {
			double value = sensors[sensor].value;
			value = (value * this->children[i].scale) + this->children[i].offset;
			if (value < this->children[i].min) value = this->children[i].min;
			if (value > this->children[i].max) value = this->children[i].max;

			switch (this->action) {
			case EnsembleAction::Min:
				if (value < initial) initial = value;
				break;
			case EnsembleAction::Max:
				if (value > initial) initial = value;
				break;
			case EnsembleAction::Average:
			case EnsembleAction::Sum:
				initial += value;
				break;
			case EnsembleAction::Product:
				initial *= value;
				break;
			default:
				// Unreachable
				return 0.0;
			}

			count += 1;
		}
	}

	if (count == 0) {
		return 0.0;
	}
	else if (this->action == EnsembleAction::Average) {
		return initial / (double)count;
	}
	else {
		return initial;
	}
}

uint32_t EnsembleSensor::_serialize_internal(char* buf) {
	uint32_t i = 0;
	for (size_t j = 0; j < ENSEMBLE_SENSOR_CHILDREN_COUNT; j++) {
		i += write_buf<uint8_t>(buf + i, this->children[j].sensor_id);
		i += write_buf<double>(buf + i, this->children[j].min);
		i += write_buf<double>(buf + i, this->children[j].max);
		i += write_buf<double>(buf + i, this->children[j].scale);
		i += write_buf<double>(buf + i, this->children[j].offset);
	}

	buf[i++] = static_cast<uint8_t>(this->action);
	return i;
}

EnsembleSensor::EnsembleSensor(sensor_memory_t* sensors, char* buf) {
	uint32_t i = Sensor::_deserialize(buf);
	for (size_t j = 0; j < ENSEMBLE_SENSOR_CHILDREN_COUNT; j++) {
		this->children[j].sensor_id = read_buf<uint8_t>(buf, &i);
		this->children[j].min = read_buf<double>(buf, &i);
		this->children[j].max = read_buf<double>(buf, &i);
		this->children[j].scale = read_buf<double>(buf, &i);
		this->children[j].offset = read_buf<double>(buf, &i);
	}

	this->action = static_cast<EnsembleAction>(buf[i++]);
	this->sensors = sensors;
}


WeatherSensor::WeatherSensor(uint32_t interval, double min, double max, double scale, double offset, const char* name, SensorUnit unit, uint32_t flags, WeatherGetter weather_getter, WeatherAction action) :
	Sensor(interval, min, max, scale, offset, name, unit, flags),
	action(action),
	weather_getter(weather_getter) {
}

void WeatherSensor::emit_extra_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR("{\"action\":$D}"), this->action);
}

void WeatherSensor::emit_description_json(BufferFiller* bfill) {
	bfill->emit_p(PSTR("{\"name\":\"Weather Sensor\",\"args\":[{\"name\":\"Weather Information\",\"arg\":\"action\",\"type\":\"enum::WeatherAction\",\"default\":\"0\",\"extra\":[]}]}"));
}

double WeatherSensor::get_initial_value() {
	return 0.0;
}

double WeatherSensor::_get_raw_value() {
	return this->weather_getter(this->action);
}

uint32_t WeatherSensor::_serialize_internal(char* buf) {
	uint32_t i = 0;
	buf[i++] = static_cast<uint8_t>(this->action);
	return i;
}

WeatherSensor::WeatherSensor(WeatherGetter weather_getter, char* buf) : weather_getter(weather_getter) {
	uint32_t i = Sensor::_deserialize(buf);
	this->action = static_cast<WeatherAction>(buf[i++]);
}

SensorAdjustment::SensorAdjustment(uint8_t flags, uint8_t sid, uint8_t point_count, sensor_adjustment_point_t* points) {
	this->flags = flags;
	this->sid = sid;
	if (point_count > SENSOR_ADJUSTMENT_POINTS) point_count = SENSOR_ADJUSTMENT_POINTS;
	this->point_count = point_count;
	for (size_t i = 0; i < point_count; i++) {
		this->points[i] = points[i];
	}
}

SensorAdjustment::SensorAdjustment(char* buf) {
	uint32_t i = 0;
	this->flags = buf[i++];
	this->sid = buf[i++];
	this->point_count = buf[i++];

	for (size_t j = 0; j < SENSOR_ADJUSTMENT_POINTS; j++) {
		this->points[j].x = read_buf<double>(buf, &i);
		this->points[j].y = read_buf<double>(buf, &i);
	}
}

double SensorAdjustment::get_adjustment_factor(sensor_memory_t* sensors) {
	if (this->flags & (1 << SENADJ_FLAG_ENABLE) && this->sid < MAX_SENSORS && sensors[this->sid].interval) {
		double value = sensors[this->sid].value;
		if (this->point_count < 1) { return 1.0; }
		if (this->point_count == 1) { return this->points[0].y; }
		if (value <= this->points[0].x) return this->points[0].y;
		if (value >= this->points[this->point_count - 1].x) return this->points[this->point_count - 1].y;

		uint8_t i;

		for (i = 0; i < this->point_count - 1; i++) {
			if (value < this->points[i + 1].x) {
				break;
			}
		}

		sensor_adjustment_point_t left = this->points[i];
		sensor_adjustment_point_t right = this->points[i + 1];
		if (right.x == left.x) return left.y;

		value = (value - left.x) / (right.x - left.x) * (right.y - left.y) + left.y;

		if (value < 0) return 0;
		return value;
	}
	else {
		return 1.0;
	}
}

uint32_t SensorAdjustment::serialize(char* buf) {
	uint32_t i = 0;
	buf[i++] = this->flags;
	buf[i++] = this->sid;
	buf[i++] = this->point_count;

	for (size_t j = 0; j < SENSOR_ADJUSTMENT_POINTS; j++) {
		i += write_buf<double>(buf + i, this->points[j].x);
		i += write_buf<double>(buf + i, this->points[j].y);
	}

	return i;
}