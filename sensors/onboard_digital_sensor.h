#pragma once

#include "../defines.h"
#include "sensor.h"

// Wraps the firmware's onboard digital inputs (SN1, SN2) as Sensor instances.
// Reports the *debounced* state (post delayed-on / delayed-off filter), so
// adjustment logic sees the same "is this input considered active right now?"
// signal that the rest of the firmware uses internally.
//
// Numeric values are stable across builds; future entries (raw state, computed
// flow rate) can be appended without disturbing existing records.
enum class OnboardInput : uint8_t {
	SN1 = 0,    // -> os.status.sensor1_active
	SN2 = 1,    // -> os.status.sensor2_active
	MAX_VALUE,
};

class OnboardDigitalSensor : public Sensor {
	public:
	OnboardDigitalSensor(uint32_t interval, float min, float max, const char *name, SensorUnit unit, uint8_t flag,
	                     OnboardInput input);
	OnboardDigitalSensor(char *buf, uint32_t len);

	void emit_extra_json(BufferFiller *bfill);
	static void emit_description_json(BufferFiller *bfill);

	SensorType get_sensor_type() {
		return SensorType::OnboardDigital;
	}

	OnboardInput input;

	private:
	float _get_raw_value();
	uint32_t _serialize_internal(char *buf);
};
