## Sensor Expander — Hardware & Wiring

!!! warning "Power Off Before Wiring"
    Always **power off the main controller** before connecting or disconnecting the Sensor Expander or its sensors.

### Connecting the Expander

*(To be completed with board photos and connector details.)*

The Sensor Expander connects to OpenSprinkler over I2C. Once connected and powered on, the firmware detects the expander's ADS1115 automatically; detection status is reported in the app.

### Wiring Analog Sensors

*(To be completed with per-channel wiring diagrams and voltage-range notes.)*

The ADS1115 provides analog input channels. Each analog sensor is wired to one channel and mapped to a firmware sensor via its **pin** number (`1`–`16`) when [configuring the sensor](configuration.md).

Key wiring considerations to document here:

* Supply voltage and reference range for the ADS1115 inputs.
* Grounding — sensor common vs. controller common.
* Powering sensors that require an external supply.

### Detection & Troubleshooting

*(To be completed.)*

* Confirming the expander is detected (hardware-detected flag).
* What to check if channels read incorrectly or the expander is not found.
