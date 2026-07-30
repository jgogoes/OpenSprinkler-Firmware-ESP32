## Sensor Expander User Manual

### Overview

!!! note "Requires firmware 2.2.1(5) or later"
    The Sensor Expander and the external-sensor framework described here require OpenSprinkler firmware **2.2.1(5)** or newer. See the current [OpenSprinkler User Manual](../manual.md) and [API Reference](../api.md).

#### What Is the Sensor Expander?

The **Sensor Expander** is an add-on board that adds analog sensor inputs to OpenSprinkler. It connects over I2C and uses an ADS1115 analog-to-digital converter to read analog sensors — such as soil moisture, pressure, temperature, or any sensor with an analog voltage output — and make their readings available to the firmware.

Together with the firmware's **external-sensor framework**, sensor readings can be:

* **Displayed and logged** over time, viewable in the app and downloadable.
* **Combined** across multiple sensors (aggregate sensors: min / max / average / sum / median / range).
* **Used to adjust watering** — a sensor reading can automatically scale a program's watering level through a configurable adjustment curve.

#### Sensor Types

The external-sensor framework supports three kinds of sensors, only the first of which requires the Sensor Expander hardware:

| Type | Requires hardware | Description |
|:-----|:------------------|:------------|
| **Analog (ADS1115)** | Sensor Expander | An analog input channel read through the expander's ADS1115. |
| **Aggregate** | No | A virtual sensor that combines the readings of other sensors. |
| **Weather** | No | A value pulled from the weather service. |

#### How to Use This Manual

* **[Hardware & Wiring](#hardware-wiring)** — connecting the Sensor Expander and wiring analog sensors.
* **[Configuring Sensors](#configuring-sensors)** — adding, editing, and calibrating sensors in the app.
* **[Sensor-Based Watering](#sensor-based-watering)** — using sensor readings to adjust program watering levels.

For the underlying HTTP endpoints, see the [Expanded Sensors section](../api.md#get-expanded-sensors-jsn) of the API reference.

### Hardware & Wiring

!!! warning "Power Off Before Wiring"
    Always **power off the main controller** before connecting or disconnecting the Sensor Expander or its sensors.

#### Connecting the Expander

*(To be completed with board photos and connector details.)*

The Sensor Expander connects to OpenSprinkler over I2C. Once connected and powered on, the firmware detects the expander's ADS1115 automatically; detection status is reported in the app.

#### Wiring Analog Sensors

*(To be completed with per-channel wiring diagrams and voltage-range notes.)*

The ADS1115 provides analog input channels. Each analog sensor is wired to one channel and mapped to a firmware sensor via its **pin** number (`1`–`16`) when [configuring the sensor](#configuring-sensors).

Key wiring considerations to document here:

* Supply voltage and reference range for the ADS1115 inputs.
* Grounding — sensor common vs. controller common.
* Powering sensors that require an external supply.

#### Detection & Troubleshooting

*(To be completed.)*

* Confirming the expander is detected (hardware-detected flag).
* What to check if channels read incorrectly or the expander is not found.

### Configuring Sensors

Sensors are added and edited in the app's sensor settings. Each sensor has a stable ID (`uuid`) that is preserved when sensors are added, deleted, or reordered.

#### Common Sensor Settings

Every sensor, regardless of type, has the following settings:

| Setting | Description |
|:--------|:------------|
| **Name** | A label for the sensor. |
| **Type** | Analog (ADS1115), Aggregate, or Weather. |
| **Unit** | The unit the reading is displayed in. |
| **Sampling interval** | How often the sensor is read, in minutes. |
| **Min / Max** | Clamping range applied to the reading. |
| **Enable** | Whether the sensor is active. |
| **Log** | Whether readings are written to the sensor log. |

#### Analog (ADS1115) Sensors

*(To be completed with calibration walkthrough.)*

An analog sensor maps to one ADS1115 input channel via its **pin** number (`1`–`16`). The raw voltage is converted to a meaningful value using the sensor's min/max (and, where applicable, a calibration mapping).

#### Aggregate Sensors

An **aggregate sensor** is a virtual sensor that combines the readings of other sensors. It does not require any hardware.

* **Children** — the list of source sensors, each with an optional scale and offset.
* **Action** — how the children are combined: Min, Max, Average, Sum, Median, or Range.

#### Weather Sensors

A **weather sensor** pulls a value from the weather service rather than from hardware.

#### Viewing and Exporting Logs

*(To be completed.)*

When logging is enabled, readings are recorded over time and can be viewed in the app or exported (JSON, CSV, or binary) via the [`/jsl`](../api.md#get-expanded-sensor-log-jsl) endpoint.

### Sensor-Based Watering

A sensor reading can automatically scale a program's watering level through an **adjustment curve**. This lets, for example, a soil-moisture sensor reduce watering when the soil is already wet, or a temperature sensor increase watering in hot weather.

#### How Adjustment Works

Each program can be linked to a sensor and an adjustment curve. When the program runs, the sensor's current reading is mapped through the curve to a **scaling factor**, which multiplies the program's base watering level.

The final station runtime combines both the weather adjustment and the sensor adjustment:

```
effective runtime = base duration × weather factor × sensor factor
```

The combined factors are reported per program by the [`/jpa`](../api.md#get-program-adjustments-jpa) endpoint (`wa` weather, `sa` sensor, `ta` total).

!!! note "Extended runtimes"
    Because both factors can exceed 1.0, the effective runtime can exceed the 18-hour limit that applies to *programmed* durations. Effective runtimes are capped at a firmware maximum (`maxrt`, reported by `/jpa`).

#### Configuring an Adjustment Curve

*(To be completed with a UI walkthrough and a worked example.)*

An adjustment curve maps sensor readings to scaling factors using up to several interpolation points. Between points, the factor is interpolated; outside the range, the nearest endpoint applies.

#### Worked Example

*(To be completed — e.g. a soil-moisture curve that scales watering from 100% at dry to 0% at saturated.)*
