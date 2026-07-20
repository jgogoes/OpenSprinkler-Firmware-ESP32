## Sensor Expander — Overview

!!! note "Requires firmware 2.2.1(5) or later"
    The Sensor Expander and the external-sensor framework described here require OpenSprinkler firmware **2.2.1(5)** or newer. See the [Firmware 2.2.1(5) User Manual](../2.2.1/221_5_manual.md) and [API Reference](../2.2.1/221_5_api.md).

### What Is the Sensor Expander?

The **Sensor Expander** is an add-on board that adds analog sensor inputs to OpenSprinkler. It connects over I2C and uses an ADS1115 analog-to-digital converter to read analog sensors — such as soil moisture, pressure, temperature, or any sensor with an analog voltage output — and make their readings available to the firmware.

Together with the firmware's **external-sensor framework**, sensor readings can be:

* **Displayed and logged** over time, viewable in the app and downloadable.
* **Combined** across multiple sensors (aggregate sensors: min / max / average / sum / median / range).
* **Used to adjust watering** — a sensor reading can automatically scale a program's watering level through a configurable adjustment curve.

### Sensor Types

The external-sensor framework supports three kinds of sensors, only the first of which requires the Sensor Expander hardware:

| Type | Requires hardware | Description |
|:-----|:------------------|:------------|
| **Analog (ADS1115)** | Sensor Expander | An analog input channel read through the expander's ADS1115. |
| **Aggregate** | No | A virtual sensor that combines the readings of other sensors. |
| **Weather** | No | A value pulled from the weather service. |

### How to Use This Documentation

* **[Hardware & Wiring](hardware.md)** — connecting the Sensor Expander and wiring analog sensors.
* **[Configuring Sensors](configuration.md)** — adding, editing, and calibrating sensors in the app.
* **[Sensor-Based Watering](watering.md)** — using sensor readings to adjust program watering levels.

For the underlying HTTP endpoints, see the sensor sections of the [2.2.1(5) API Reference](../2.2.1/221_5_api.md#24-get-sensors-jsn).
