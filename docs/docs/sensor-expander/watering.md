## Sensor Expander — Sensor-Based Watering

A sensor reading can automatically scale a program's watering level through an **adjustment curve**. This lets, for example, a soil-moisture sensor reduce watering when the soil is already wet, or a temperature sensor increase watering in hot weather.

### How Adjustment Works

Each program can be linked to a sensor and an adjustment curve. When the program runs, the sensor's current reading is mapped through the curve to a **scaling factor**, which multiplies the program's base watering level.

The final station runtime combines both the weather adjustment and the sensor adjustment:

```
effective runtime = base duration × weather factor × sensor factor
```

The combined factors are reported per program by the [`/jpa`](../2.2.1/221_5_api.md#13a-get-program-adjustments-jpa) endpoint (`wa` weather, `sa` sensor, `ta` total).

!!! note "Extended runtimes"
    Because both factors can exceed 1.0, the effective runtime can exceed the 18-hour limit that applies to *programmed* durations. Effective runtimes are capped at a firmware maximum (`maxrt`, reported by `/jpa`).

### Configuring an Adjustment Curve

*(To be completed with a UI walkthrough and a worked example.)*

An adjustment curve maps sensor readings to scaling factors using up to several interpolation points. Between points, the factor is interpolated; outside the range, the nearest endpoint applies.

### Worked Example

*(To be completed — e.g. a soil-moisture curve that scales watering from 100% at dry to 0% at saturated.)*
