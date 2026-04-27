#include "sensor.h"
#include "OpenSprinkler.h"

extern OpenSprinkler os;
extern char tmp_buffer[];

const char *enum_string(SensorUnitGroup group) {
	switch (group) {
		case SensorUnitGroup::None:
			return PSTR("No Group");
		case SensorUnitGroup::Temperature:
			return PSTR("Temperature");
		case SensorUnitGroup::Length:
			return PSTR("Length");
		case SensorUnitGroup::Volume:
			return PSTR("Volume");
		case SensorUnitGroup::Light:
			return PSTR("Light");
		case SensorUnitGroup::Energy:
			return PSTR("Energy");
		case SensorUnitGroup::Velocity:
			return PSTR("Velocity");
		case SensorUnitGroup::Pressure:
			return PSTR("Pressure");
		case SensorUnitGroup::Flow:
			return PSTR("Flow");
		case SensorUnitGroup::MAX_VALUE:
			return nullptr;
	}

	return nullptr;
}

const char *enum_string(EnsembleAction action) {
	switch (action) {
		case EnsembleAction::Min: return PSTR("Min");
		case EnsembleAction::Max: return PSTR("Max");
		case EnsembleAction::Average: return PSTR("Average");
		case EnsembleAction::Sum: return PSTR("Sum");
		case EnsembleAction::Product: return PSTR("Product");
		case EnsembleAction::MAX_VALUE: return nullptr;
	}

	return nullptr;
}

const char *enum_string(WeatherAction action) {
	switch (action) {
		case WeatherAction::MAX_VALUE: return nullptr;
	}

	return nullptr;
}

const char* get_sensor_unit_name(SensorUnit unit) {
	switch (unit) {
	case SensorUnit::None:
		return PSTR("None");
	case SensorUnit::Celsius:
		return PSTR("Celsius");
	case SensorUnit::Fahrenheit:
		return PSTR("Fahrenheit");
	case SensorUnit::Kelvin:
		return PSTR("Kelvin");
	case SensorUnit::Millimeter:
		return PSTR("Millimeter");
	case SensorUnit::Centimeter:
		return PSTR("Centimeter");
	case SensorUnit::Meter:
		return PSTR("Meter");
	case SensorUnit::Kilometer:
		return PSTR("Kilometer");
	case SensorUnit::Inch:
		return PSTR("Inch");
	case SensorUnit::Foot:
		return PSTR("Foot");
	case SensorUnit::Mile:
		return PSTR("Mile");
	case SensorUnit::Lux:
		return PSTR("Lux");
	case SensorUnit::Lumen:
		return PSTR("Lumen");
	case SensorUnit::Millivolt:
		return PSTR("Millivolt");
	case SensorUnit::Volt:
		return PSTR("Volt");
	case SensorUnit::Milliampere:
		return PSTR("Milliampere");
	case SensorUnit::Ampere:
		return PSTR("Ampere");
	case SensorUnit::Percent:
		return PSTR("Percent");
	case SensorUnit::MilesPerHour:
		return PSTR("Miles Per Hour");
	case SensorUnit::KilometersPerHour:
		return PSTR("Kilometers Per Hour");
	case SensorUnit::MetersPerSecond:
		return PSTR("Meters Per Second");
	case SensorUnit::DielectricConstant:
		return PSTR("Dielectric Constant");
	case SensorUnit::PartsPerMillion:
		return PSTR("Parts Per Million");
	case SensorUnit::Ohm:
		return PSTR("Ohm");
	case SensorUnit::Milliohm:
		return PSTR("Milliohm");
	case SensorUnit::Kiloohm:
		return PSTR("Kiloohm");
	case SensorUnit::Bar:
		return PSTR("Bar");
	case SensorUnit::Kilopascal:
		return PSTR("Kilopascal");
	case SensorUnit::Pascal:
		return PSTR("Pascal");
	case SensorUnit::Torr:
		return PSTR("Torr");
	case SensorUnit::LitersPerSecond:
		return PSTR("Liters Per Second");
	case SensorUnit::GallonsPerSecond:
		return PSTR("Gallons");
	case SensorUnit::MAX_VALUE:
		return nullptr;
	}

	return nullptr;
}

const char* get_sensor_unit_short(SensorUnit unit) {
	switch (unit) {
	case SensorUnit::None:
		return PSTR("");
	case SensorUnit::Celsius:
		return PSTR("°C");
	case SensorUnit::Fahrenheit:
		return PSTR("°F");
	case SensorUnit::Kelvin:
		return PSTR("K");
	case SensorUnit::Millimeter:
		return PSTR("mm");
	case SensorUnit::Centimeter:
		return PSTR("cm");
	case SensorUnit::Meter:
		return PSTR("m");
	case SensorUnit::Kilometer:
		return PSTR("km");
	case SensorUnit::Inch:
		return PSTR("in");
	case SensorUnit::Foot:
		return PSTR("ft");
	case SensorUnit::Mile:
		return PSTR("mi");
	case SensorUnit::Lux:
		return PSTR("lx");
	case SensorUnit::Lumen:
		return PSTR("lm");
	case SensorUnit::Millivolt:
		return PSTR("mV");
	case SensorUnit::Volt:
		return PSTR("V");
	case SensorUnit::Milliampere:
		return PSTR("mA");
	case SensorUnit::Ampere:
		return PSTR("A");
	case SensorUnit::Percent:
		return PSTR("%");
	case SensorUnit::MilesPerHour:
		return PSTR("mph");
	case SensorUnit::KilometersPerHour:
		return PSTR("km/h");
	case SensorUnit::MetersPerSecond:
		return PSTR("m/s");
	case SensorUnit::DielectricConstant:
		return PSTR("-");
	case SensorUnit::PartsPerMillion:
		return PSTR("ppm");
	case SensorUnit::Ohm:
		return PSTR("Ω");
	case SensorUnit::Milliohm:
		return PSTR("mΩ");
	case SensorUnit::Kiloohm:
		return PSTR("kΩ");
	case SensorUnit::Bar:
		return PSTR("bar");
	case SensorUnit::Kilopascal:
		return PSTR("kPa");
	case SensorUnit::Pascal:
		return PSTR("Pa");
	case SensorUnit::Torr:
		return PSTR("torr");
	case SensorUnit::LitersPerSecond:
		return PSTR("L/s");
	case SensorUnit::GallonsPerSecond:
		return PSTR("gal/s");
	case SensorUnit::MAX_VALUE:
		return nullptr;
	}

	return nullptr;
}

const SensorUnitGroup get_sensor_unit_group(SensorUnit unit) {
	switch (unit) {
	case SensorUnit::None:
		return SensorUnitGroup::None;
	case SensorUnit::Celsius:
		return SensorUnitGroup::Temperature;
	case SensorUnit::Fahrenheit:
		return SensorUnitGroup::Temperature;
	case SensorUnit::Kelvin:
		return SensorUnitGroup::Temperature;
	case SensorUnit::Millimeter:
		return SensorUnitGroup::Length;
	case SensorUnit::Centimeter:
		return SensorUnitGroup::Length;
	case SensorUnit::Meter:
		return SensorUnitGroup::Length;
	case SensorUnit::Kilometer:
		return SensorUnitGroup::Length;
	case SensorUnit::Inch:
		return SensorUnitGroup::Length;
	case SensorUnit::Foot:
		return SensorUnitGroup::Length;
	case SensorUnit::Mile:
		return SensorUnitGroup::Length;
	case SensorUnit::Lux:
		return SensorUnitGroup::Light;
	case SensorUnit::Lumen:
		return SensorUnitGroup::Light;
	case SensorUnit::Millivolt:
		return SensorUnitGroup::Energy;
	case SensorUnit::Volt:
		return SensorUnitGroup::Energy;
	case SensorUnit::Milliampere:
		return SensorUnitGroup::Energy;
	case SensorUnit::Ampere:
		return SensorUnitGroup::Energy;
	case SensorUnit::Percent:
		return SensorUnitGroup::None;
	case SensorUnit::MilesPerHour:
		return SensorUnitGroup::Velocity;
	case SensorUnit::KilometersPerHour:
		return SensorUnitGroup::Velocity;
	case SensorUnit::MetersPerSecond:
		return SensorUnitGroup::Velocity;
	case SensorUnit::DielectricConstant:
		return SensorUnitGroup::Energy;
	case SensorUnit::PartsPerMillion:
		return SensorUnitGroup::None;
	case SensorUnit::Ohm:
		return SensorUnitGroup::Energy;
	case SensorUnit::Milliohm:
		return SensorUnitGroup::Energy;
	case SensorUnit::Kiloohm:
		return SensorUnitGroup::Energy;
	case SensorUnit::Bar:
		return SensorUnitGroup::Pressure;
	case SensorUnit::Kilopascal:
		return SensorUnitGroup::Pressure;
	case SensorUnit::Pascal:
		return SensorUnitGroup::Pressure;
	case SensorUnit::Torr:
		return SensorUnitGroup::Pressure;
	case SensorUnit::LitersPerSecond:
		return SensorUnitGroup::Flow;
	case SensorUnit::GallonsPerSecond:
		return SensorUnitGroup::Flow;
	case SensorUnit::MAX_VALUE:
		return SensorUnitGroup::MAX_VALUE;
	}

	return SensorUnitGroup::MAX_VALUE;
}

const uint32_t get_sensor_unit_index(SensorUnit unit) {
	return static_cast<uint32_t>(unit);
}

Sensor::Sensor(uint32_t interval, float min, float max, float scale, float offset, const char* name, SensorUnit unit, uint16_t flags) :
	interval(interval), min(min), max(max), scale(scale), offset(offset), flags(flags), unit(unit) {
	strncpy(this->name, name, SENSOR_NAME_LEN);
	this->name[SENSOR_NAME_LEN - 1] = 0;
}

Sensor::Sensor() {}

float Sensor::get_new_value() {
	float value = this->_get_raw_value();
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
	i += write_buf<uint16_t>(buf + i, this->flags);
	i += write_buf<float>(buf + i, this->scale);
	i += write_buf<float>(buf + i, this->offset);
	i += write_buf<float>(buf + i, this->min);
	i += write_buf<float>(buf + i, this->max);
	i += write_buf<uint16_t>(buf + i, this->uuid);

	i += this->_serialize_internal(buf + i);
	return i;
}

uint32_t Sensor::_deserialize(char* buf) {
	uint32_t i = 1; // Skip sensor type

	memcpy(this->name, buf + i, SENSOR_NAME_LEN);
	i += SENSOR_NAME_LEN;
	this->unit = static_cast<SensorUnit>(buf[i++]);
	this->interval = read_buf<uint32_t>(buf, &i);
	this->flags = read_buf<uint16_t>(buf, &i);
	this->scale = read_buf<float>(buf, &i);
	this->offset = read_buf<float>(buf, &i);
	this->min = read_buf<float>(buf, &i);
	this->max = read_buf<float>(buf, &i);
	this->uuid = read_buf<uint16_t>(buf, &i);

	return i;
}

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
	bfill->emit_p(PSTR("{\"name\":\"Ensemble Sensor\",\"args\":[{\"name\":\"Argument Sensors\",\"arg\":\"children\",\"type\":\"array::4\",\"extra\":[{\"name\":\"Sensor ID\",\"arg\":\"sid\",\"type\":\"sensor\",\"default\":\"\",\"extra\":[]},{\"name\":\"Minimum Value\",\"arg\":\"min\",\"type\":\"float\",\"default\":\"\",\"extra\":[]},{\"name\":\"Maximum Value\",\"arg\":\"max\",\"type\":\"float\",\"default\":\"\",\"extra\":[]},{\"name\":\"Scale\",\"arg\":\"scale\",\"type\":\"float\",\"default\":\"\",\"extra\":[]},{\"name\":\"Offset\",\"arg\":\"offset\",\"type\":\"float\",\"default\":\"\",\"extra\":[]}]},{\"name\":\"Ensemble Action\",\"arg\":\"action\",\"type\":\"enum::EnsembleAction\",\"default\":\"0\",\"extra\":[]}]}"));
}

float EnsembleSensor::get_initial_value() {
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
		return 0.f;
	}
}

float EnsembleSensor::_get_raw_value() {
	float inital = this->get_initial_value();
	uint8_t count = 0;

	for (size_t i = 0; i < ENSEMBLE_SENSOR_CHILDREN_COUNT; i++) {
		uint8_t sensor = Sensor::find_index(this->children[i].uuid);
		if (sensor < MAX_SENSORS && sensors[sensor].interval) {
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
				// Unreachable
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


WeatherSensor::WeatherSensor(uint32_t interval, float min, float max, float scale, float offset, const char* name, SensorUnit unit, uint16_t flags, WeatherGetter weather_getter, WeatherAction action) :
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

float WeatherSensor::get_initial_value() {
	return 0.f;
}

float WeatherSensor::_get_raw_value() {
	return this->weather_getter(this->action);
}

uint32_t WeatherSensor::_serialize_internal(char* buf) {
	uint32_t i = 0;
	buf[i++] = static_cast<uint8_t>(this->action);
	return i;
}

WeatherSensor::WeatherSensor(WeatherGetter weather_getter, char* buf) {
	uint32_t i = Sensor::_deserialize(buf);
	this->action = static_cast<WeatherAction>(buf[i++]);
}

SensorAdjustment::SensorAdjustment(uint8_t flags, uint16_t uuid, uint8_t point_count, sensor_adjustment_point_t* points) {
	this->flags = flags;
	this->uuid = uuid;
	if (point_count > SENSOR_ADJUSTMENT_POINTS) point_count = SENSOR_ADJUSTMENT_POINTS;
	this->point_count = point_count;
	for (size_t i = 0; i < point_count; i++) {
		this->points[i] = points[i];
	}
}

float SensorAdjustment::get_adjustment_factor(sensor_memory_t* sensors) {
	if (this->flags & (1 << SENADJ_FLAG_ENABLE) && this->uuid != SENSOR_UUID_NONE) {
		uint8_t idx = Sensor::find_index(this->uuid);
		if (idx < MAX_SENSORS && sensors[idx].interval) {
			float value = sensors[idx].value;
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

			if (value < 0) value = 0;
			return value;
		}
	}
	return 1.f;
}

SensorAdjustment *SensorAdjustment::read(uint8_t index, uint8_t nprograms) {
	static SensorAdjustment result(0, SENSOR_UUID_NONE, 0, nullptr);

	if (index >= nprograms) return nullptr;
	os_file_type file = file_open(SENADJ_FILENAME, FileOpenMode::Read);
	if (file) {
		uint32_t pos = (uint32_t)index * SENSOR_ADJUSTMENT_SIZE;
		if (file_size(file) < pos + SENSOR_ADJUSTMENT_SIZE) {
			file_close(file);
			return nullptr;
		}
		file_seek(file, pos, FileSeekMode::Set);
		file_read(file, &result, SENSOR_ADJUSTMENT_SIZE);
		file_close(file);
		if (result.uuid != SENSOR_UUID_NONE) {
			return &result;
		}
	}
	return nullptr;
}

void SensorAdjustment::write(SensorAdjustment *adj, uint8_t index) {
	uint32_t pos = (uint32_t)SENSOR_ADJUSTMENT_SIZE * index;

	os_file_type file = file_open(SENADJ_FILENAME, FileOpenMode::ReadWrite);
	if (file) {
		SensorAdjustment disabled(0, SENSOR_UUID_NONE, 0, nullptr);

		uint32_t cur_size = file_size(file);
		if (cur_size < pos) {
			file_seek(file, 0, FileSeekMode::End);
			while (cur_size < pos) {
				file_write(file, &disabled, SENSOR_ADJUSTMENT_SIZE);
				cur_size += SENSOR_ADJUSTMENT_SIZE;
			}
		}

		file_seek(file, pos, FileSeekMode::Set);
		SensorAdjustment *to_write = adj ? adj : &disabled;
		file_write(file, to_write, SENSOR_ADJUSTMENT_SIZE);

		file_close(file);
	} else {
		DEBUG_PRINT("Failed to open file: ");
		DEBUG_PRINTLN(SENADJ_FILENAME);
	}
}

// ---------------------------------------------------------------------------
// Sensor file I/O
// ---------------------------------------------------------------------------

Sensor *Sensor::parse(os_file_type file) {
	static uint8_t sensor_scratchpad[sizeof(OpenSprinkler::SensorUnion)] __attribute__((aligned(4)));
	static Sensor *active_sensor = nullptr;

	if (active_sensor != nullptr) {
		active_sensor->~Sensor();
		active_sensor = nullptr;
	}
	uint32_t len = 0;
	file_read(file, &len, sizeof(len));

	if (len == 0 || len > (TMP_BUFFER_SIZE - sizeof(uint32_t))) return nullptr;

	file_read(file, tmp_buffer, len);
	file_seek(file, TMP_BUFFER_SIZE - sizeof(uint32_t) - len, FileSeekMode::Current);

	if ((uint8_t)(tmp_buffer[0]) >= (uint8_t)SensorType::MAX_VALUE) return nullptr;

	SensorType sensor_type = static_cast<SensorType>(*tmp_buffer);
	switch (sensor_type) {
		case SensorType::Ensemble:
			active_sensor = new (sensor_scratchpad) EnsembleSensor(os.sensors, (char*)tmp_buffer);
			break;
		case SensorType::ADS1115:
			active_sensor = new (sensor_scratchpad) ADS1115Sensor(os.ads1115_devices, (char*)tmp_buffer);
			break;
		case SensorType::Weather:
			active_sensor = new (sensor_scratchpad) WeatherSensor(os.get_sensor_weather_data, (char*)tmp_buffer);
			break;
		default:
			return nullptr;
	}
	return active_sensor;
}

Sensor *Sensor::get(uint8_t index) {
	uint32_t pos = 1 + (uint32_t)TMP_BUFFER_SIZE * index;

	os_file_type file = file_open(SENSORS_FILENAME, FileOpenMode::Read);
	if (file) {
		file_seek(file, pos, FileSeekMode::Set);
		Sensor *result = Sensor::parse(file);
		file_close(file);
		return result;
	} else {
		DEBUG_PRINT("Failed to open file: ");
		DEBUG_PRINTLN(SENSORS_FILENAME);
		return nullptr;
	}
}

void Sensor::write(Sensor *sensor, uint8_t index) {
	uint32_t pos = 1 + (uint32_t)TMP_BUFFER_SIZE * index;
	uint32_t len = 0;

	// Zero data area before serializing so the full TMP_BUFFER_SIZE slot is clean.
	memset(tmp_buffer, 0, TMP_BUFFER_SIZE - sizeof(uint32_t));
	if (sensor) {
		len = sensor->serialize(tmp_buffer);
		if (len > (TMP_BUFFER_SIZE - sizeof(uint32_t)))
			len = TMP_BUFFER_SIZE - sizeof(uint32_t);
	}

	os_file_type file = file_open(SENSORS_FILENAME, FileOpenMode::ReadWrite);
	if (file) {
		file_seek(file, pos, FileSeekMode::Set);
		file_write(file, &len, sizeof(len));
		file_write(file, tmp_buffer, TMP_BUFFER_SIZE - sizeof(uint32_t));  // always write full slot
		file_close(file);
	} else {
		DEBUG_PRINT("Failed to open file: ");
		DEBUG_PRINTLN(SENSORS_FILENAME);
	}
}

/** Load sensor count from sensor file */
void Sensor::load_count() {
	OpenSprinkler::nsensors = file_read_byte(SENSORS_FILENAME, 0);
}

/** Save sensor count to sensor file */
void Sensor::save_count() {
	file_write_byte(SENSORS_FILENAME, 0, OpenSprinkler::nsensors);
}

/** Add a sensor */
unsigned char Sensor::add(Sensor *sensor) {
	if (OpenSprinkler::nsensors >= MAX_SENSORS) return 0;

	Sensor::write(sensor, OpenSprinkler::nsensors);

	// update in-memory state
	sensor_memory_t &m = OpenSprinkler::sensors[OpenSprinkler::nsensors];
	m.interval = sensor->interval;
	m.flags = sensor->flags;
	m.uuid = sensor->uuid;
	m.next_update = 0;
	m.value = sensor->get_initial_value();

	OpenSprinkler::nsensors++;
	Sensor::save_count();
	return 1;
}

/** Modify a sensor */
unsigned char Sensor::modify(uint8_t index, Sensor *sensor) {
	if (index >= OpenSprinkler::nsensors) return 0;
	Sensor::write(sensor, index);

	// update memory
	sensor_memory_t &m = OpenSprinkler::sensors[index];
	m.interval = sensor->interval;
	m.flags = sensor->flags;
	m.uuid = sensor->uuid;
	m.next_update = 0;
	m.value = sensor->get_initial_value();

	return 1;
}

/** Delete a sensor */
unsigned char Sensor::del(uint8_t index) {
	if (index >= OpenSprinkler::nsensors) return 0;
	if (OpenSprinkler::nsensors == 0) return 0;

	// erase by copying backward
	for (uint8_t i = index; i < OpenSprinkler::nsensors - 1; i++) {
		file_copy_block(SENSORS_FILENAME, 1 + (uint32_t)(i + 1) * TMP_BUFFER_SIZE, 1 + (uint32_t)i * TMP_BUFFER_SIZE, TMP_BUFFER_SIZE, tmp_buffer);
		// also shift in-memory state
		OpenSprinkler::sensors[i] = OpenSprinkler::sensors[i + 1];
	}

	OpenSprinkler::nsensors--;
	Sensor::save_count();
	return 1;
}

/** Load all sensors from file to memory */
void Sensor::load_all() {
	OpenSprinkler::lcd.clear();
	OpenSprinkler::lcd.setCursor(0, 0);
	OpenSprinkler::lcd_print_pgm(PSTR("Init sensors..."));

	// 1. Check if the configuration file exists.
	// If not, we need to initialize it with a 0 count.
	if (!file_exists(SENSORS_FILENAME)) {
		DEBUG_PRINTLN(F("Sensor files missing. Initializing..."));
		OpenSprinkler::nsensors = 0;
		Sensor::save_count();
	} else {
		Sensor::load_count();
	}

	// 2. Proceed with existing load logic
	Sensor *sensor;
	os_file_type file = file_open(SENSORS_FILENAME, FileOpenMode::Read);
	if (file) {
		// skip the first byte (count)
		file_seek(file, 1, FileSeekMode::Set);
		for (size_t i = 0; i < OpenSprinkler::nsensors; i++) {
			if ((sensor = Sensor::parse(file))) {
				sensor_memory_t &m = OpenSprinkler::sensors[i];
				m.interval = sensor->interval;
				m.flags = sensor->flags;
				m.uuid = sensor->uuid;
				m.next_update = 0;
				m.value = sensor->get_initial_value();
			}
		}

		file_close(file);
	} else {
		DEBUG_PRINT("Failed to open file: ");
		DEBUG_PRINTLN(SENSORS_FILENAME);
	}
}

/** Find sensor index by UUID */
uint8_t Sensor::find_index(uint16_t uuid) {
	if (uuid == SENSOR_UUID_NONE) return OpenSprinkler::nsensors;
	for (uint8_t i = 0; i < OpenSprinkler::nsensors; i++) {
		if (OpenSprinkler::sensors[i].uuid == uuid) return i;
	}
	return OpenSprinkler::nsensors;
}

/** Sensor log performance test */
void Sensor::test_log(uint32_t n_records) {
	remove_sensor_log();

	DEBUG_PRINTF("sensor log test: writing %lu records\n", (unsigned long)n_records);
	uint32_t t0 = millis();

	for (uint32_t i = 0; i < n_records; i++) {
		if (OpenSprinkler::nsensors > 0) os.log_sensor((uint8_t)((i + 1) % OpenSprinkler::nsensors), (float)i / 1000.f);
	}

	uint32_t write_ms = millis() - t0;
	DEBUG_PRINTF("sensor log write: %lu ms total, %.2f ms/record\n",
		(unsigned long)write_ms,
		n_records ? (float)write_ms / n_records : 0.f);

	// Read pass: iterate all files in order (oldest to newest)
	os_file_type hfile = open_sensor_log_header(FileOpenMode::Read);
	if (!hfile) {
		DEBUG_PRINTLN("sensor log test: cannot open header");
		return;
	}
	SensorLogHeader hdr = {};
	file_read(hfile, &hdr, sizeof(hdr));
	file_close(hfile);
	if (hdr.magic != SENSOR_LOG_MAGIC || hdr.version != SENSOR_LOG_VERSION) {
		DEBUG_PRINTLN("sensor log test: bad header");
		return;
	}

	uint16_t first_file  = hdr.wrapped ? (uint16_t)((hdr.cur_file + 1) % hdr.max_files) : 0;
	uint16_t total_files = hdr.wrapped ? hdr.max_files : (uint16_t)(hdr.cur_file + 1);
	DEBUG_PRINTF("sensor log state: max_files=%u records_per_file=%u cur_file=%u wrapped=%u total_files=%u\n",
		hdr.max_files, hdr.records_per_file, hdr.cur_file, hdr.wrapped, total_files);

	uint32_t tr = millis();
	uint32_t count = 0;
	for (uint16_t fi = 0; fi < total_files; fi++) {
		uint16_t file_no = (first_file + fi) % hdr.max_files;
		os_file_type dfile = open_sensor_log(file_no, FileOpenMode::Read);
		if (!dfile) continue;
		SensorLogRecord rec;
		while (file_read(dfile, &rec, sizeof(rec)) == (int)sizeof(rec)) count++;
		file_close(dfile);
	}

	uint32_t read_ms = millis() - tr;
	DEBUG_PRINTF("sensor log read: %lu records in %lu ms (%.2f ms/record)\n",
		(unsigned long)count, (unsigned long)read_ms,
		count ? (float)read_ms / count : 0.f);
}

// ---------------------------------------------------------------------------
// Sensor log file helpers
// ---------------------------------------------------------------------------

void get_sensor_log_filename(char *buf, uint16_t file_no) {
	snprintf(buf, 24, "%s%03u", SENSORS_LOG_FILENAME, file_no % 1000);
}

os_file_type open_sensor_log(uint16_t file_no, FileOpenMode mode) {
	char fname[24];
	get_sensor_log_filename(fname, file_no);
	return file_open(fname, mode);
}

os_file_type open_sensor_log_header(FileOpenMode mode) {
	return file_open(SENSORS_LOG_HEADER_FILENAME, mode);
}

void remove_sensor_log(int16_t file_no) {
	char fname[24];
	if (file_no < 0) {
		remove_file(SENSORS_LOG_HEADER_FILENAME);
		for (uint16_t i = 0; i < SENSOR_LOG_MAX_FILES; i++) {
			get_sensor_log_filename(fname, i);
			remove_file(fname);
		}
	} else {
		get_sensor_log_filename(fname, (uint16_t)file_no);
		remove_file(fname);
	}
}