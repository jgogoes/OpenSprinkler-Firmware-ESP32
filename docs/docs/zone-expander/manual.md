## Zone Expander User Manual

### Overview

The **Zone Expander** adds 16 physical sprinkler zones to an OpenSprinkler controller. The main controller provides zones 1–8; additional expanders continue the zone numbering in groups of 16.

| Hardware | Physical-zone capacity |
|:---------|:-----------------------|
| **OpenSprinkler v3** | Up to four Zone Expanders, for 72 zones total |
| **OpenSprinkler Pi (OSPi)** | Expanders are daisy-chained through their IN and OUT ports |

Use a Zone Expander compatible with the controller's hardware generation and power model. Verify compatibility from the product label before connecting it.

### Hardware & Wiring

!!! warning "Power Off Before Wiring"
    Always power off the main controller before connecting, disconnecting, or reconfiguring a Zone Expander.

1. Connect one end of the expander cable to the controller's **Zone Expander** port. Do not connect it to the Ethernet-module port.
2. Connect the other end:
    * **OpenSprinkler v3:** connect to either expander port. Use the other port to attach the next expander.
    * **OSPi:** connect to the expander's **IN** port, then daisy-chain additional expanders from **OUT** to **IN**.
3. Connect each valve's zone wire to its numbered terminal on the expander.
4. Join the other wire from every valve to the system **COM** wire. Do not use a sensor GND terminal as valve common.

For OpenSprinkler v3, assign every expander a unique index with the DIP switch on its back:

| Expander | Index | Zones |
|:---------|:------|:------|
| First | 1 | 9–24 |
| Second | 2 | 25–40 |
| Third | 3 | 41–56 |
| Fourth | 4 | 57–72 |

OSPi determines an expander's position from its order in the daisy chain and does not use DIP-switch indexing.

### Configure Zones

After wiring the expanders:

1. Power on the controller.
2. Open **Edit Options** in the OpenSprinkler app.
3. Set the total number of zones required by the installation.
4. Open **Edit Stations** and assign each added zone a name and its operating options.
5. Run each zone briefly and confirm that the displayed zone number activates the expected terminal.

The firmware may detect connected expander hardware, but the configured zone count still controls how many zones appear in the app. Zones configured beyond the physically connected outputs may be used as virtual Remote, HTTP(S), GPIO, or RF zones.

See the current [OpenSprinkler User Manual](../manual.md#zone-wiring-diagram) for the complete controller wiring diagram and station configuration reference.
