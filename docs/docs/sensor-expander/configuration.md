## Sensor Expander — Configuring Sensors

Sensors are added and edited in the app's sensor settings. Each sensor has a stable ID (`uuid`) that is preserved when sensors are added, deleted, or reordered.

### Common Sensor Settings

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

### Analog (ADS1115) Sensors

*(To be completed with calibration walkthrough.)*

An analog sensor maps to one ADS1115 input channel via its **pin** number (`1`–`16`). The raw voltage is converted to a meaningful value using the sensor's min/max (and, where applicable, a calibration mapping).

### Aggregate Sensors

An **aggregate sensor** is a virtual sensor that combines the readings of other sensors. It does not require any hardware.

* **Children** — the list of source sensors, each with an optional scale and offset.
* **Action** — how the children are combined: Min, Max, Average, Sum, Median, or Range.

### Weather Sensors

A **weather sensor** pulls a value from the weather service rather than from hardware.

### Viewing and Exporting Logs

*(To be completed.)*

When logging is enabled, readings are recorded over time and can be viewed in the app or exported (JSON, CSV, or binary) via the [`/jsl`](../2.2.1/221_5_api.md#27-get-sensor-log-jsl) endpoint.
