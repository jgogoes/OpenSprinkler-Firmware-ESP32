# OpenSprinkler Firmware — ESP32 (ESP32-Relay-X8 board)

Firmware for an **ESP32 8-channel relay board** running the OpenSprinkler Unified Firmware, configured from the
**V1pr ESP32 port** with upstream **2.2.1(5)** merged in.

Everything is set up through **hardware defines in [`esp32.h`](esp32.h)** and
[`defines.h`](defines.h) — you don't edit application code to change wiring, you flip pins/defines and rebuild with PlatformIO:

```bash
pio run -e esp32_sprinkler          # build
pio run -e esp32_sprinkler -t upload   # flash
```

> ⚠️ This is a custom board + firmware. Always bench-test each relay/peripheral against a dummy load before attaching real valves or mains-level pumps. Some ESP32 GPIOs are high/low at boot or are input-only — see the pin notes below.

---

## 1. Pin map (ESP32-Relay-X8)

All wiring is configured in `esp32.h`. **`STATION_LOGIC 1`** = relay ON when GPIO is HIGH.

| Function | GPIO(s) | Notes |
|----------|---------|-------|
| **Relay 1 – 8 (zones + pump)** | **32, 33, 25, 26, 27, 14, 12, 13** | `ON_BOARD_GPIN_LIST` — driven directly from GPIO (shift-register disabled) |
| Status LED | 2 | Moved off GPIO 13 (now relay 8) |
| **SH1106 OLED** (1.3") | **SDA 21 / SCL 22**, addr **0x3C** | `USE_SH1106` in `defines.h` |
| **EC11 rotary encoder** | **A=15, B=16, SW=5** | `USE_ROTARY_ENCODER`; drives the boot menu (`BOOT_MENU_V2`) |
| Sensor 1 (rain switch) | 4 | full GPIO, internal pull-up works |
| Sensor 2 (soil / ADC) | 39 | input-only ADC1_CH3 — needs external pull-up/resistor if used |
| Master valve / pump (optional) | 19 | `SEPARATE_MASTER_VALVE` |
| I²C (shared bus) | 21 (SDA), 22 (SCL) | OLED + optional RTC |
| Buttons (legacy) | 18, 5, 17 | superseded by EC11 when enabled |

**Pins to avoid for inputs:** 34, 35 and 39 are **input-only** on ESP32 (no internal pull-up; 34/35 can't drive
`attachInterrupt`). Don't put the rain/soil switch or rotary-encoder A/B on them. GPIO 12 (relay 6) is a boot-strap pin — the
firmware holds relays OFF (LOW) at init, so this is fine; don't hard-wire it to pull high at power-on.

---

## 2. Wiring / schematic

Below is the recommended hook-up. All logic is 3.3 V off the ESP32 board; relays/pump get their own supply per the X8
board's design (5 V or 7–30 V at the power input).

```
                    ESP32-Relay-X8
   ┌──────────────────────────────┐
   │                      3V3  o──┴── OLED VCC
   │                              │    GND── GND (common)
   │  SDA  21 o───────────────────┴── OLED SDA
   │  SCL  22 o───────────────────┴── OLED SCL
   │                              │
   │  ENC_A 15 o──────────────────┼── EC11  A
   │  ENC_B 16 o──────────────────┼── EC11  B
   │  ENC_SW 5 o──────────────────┼── EC11  SW (button)
   │         GND o────────────────┴── EC11  GND / encoder common
   │                              │
   │  SENS1 4  o──────────────────┼── Rain switch (one side) ── GND
   │  SENS2 39 o──────────────────┼── Soil probe / divider ── GND (ext. pull-up)
   │                              │
   │  RLY8  13 o──────────[ pump ]│   (or use master-valve GPIO 19)
   │  RLY1..8 32/33/25/26/27/14/12/13 ─── 8 zone relay channels
   └──────────────────────────────┘
```

### Board hook-up summary
- **OLED (SH1106):** `VCC→3V3`, `GND→GND`, `SDA→21`, `SCL→22`. Two-wire I²C shared with the RTC.
- **EC11 encoder:** `A→15`, `B→16`, button `SW→5`, common→`GND`. (Parallel a 0.1 µF cap across A/B to common if you get jitter.)
- **Rain sensor:** dry-contact switch between GPIO 4 and GND. `INPUT_PULLUP` is set in firmware, so it reads "no rain" when open.
- **Soil sensor:** use GPIO 39 with an **external** pull-up/resistor (GPIO 39 is input-only and has no internal pull-up). Alternatively put soil on GPIO 4 and rain on another free pin.
- **Pump:** use **any of the 8 relays** as a pump zone, or use **GPIO 19** as an independent master-valve/pump that auto-runs for every zone (`SEPARATE_MASTER_VALVE`).

---

## 3. Sensors

OpenSprinkler supports up to 4 logical sensors (SN1–SN4); on this port SN1/SN2 are wired to the GPIOs above. Configure each
sensor's **type** (none / rain / soil / flow / pswitch) in the web UI (**Options → Sensor 1/2 Type**) and its active
polarity/`option`.

- **Rain** (SN1, GPIO 4): a switch that closes when it rains — pauses watering.
- **Soil** (SN2, GPIO 39): moisture probe; drives soil-based watering.
- **Flow** (optional): a flow meter pulses on a GPIO — count pulses to measure water used.

> **Can pins be defined with no hardware attached?** Yes. Defined-but-unconnected pins are harmless at runtime —
> a floating input just reads inactive, and unused outputs drive nothing. You don't have to comment anything out. The one
> caveat is sensor 2 on GPIO 39, which needs an external pull-up *if you actually install the sensor*.

---

## 4. Adding an RTC (battery RTC — no firmware change needed)

The firmware **auto-detects** an I²C RTC at boot and uses it to keep time through power loss (the ESP32 re-syncs from
NTP otherwise). **No pin re-assignment and no `#define` is required** — just put the chip on the same SDA/SCL bus (21/22).

Supported RTC chips (see `I2CRTC.h`):

| Chip | I²C address |
|------|-------------|
| DS1307 | 0x68 |
| MCP7940 | 0x6F |
| PCF8563 | 0x51 |

Wire-up: `SDA→21`, `SCL→22`, `GND→GND`, `VCC→3V3`, plus a coin cell (CR1220/CR2032) for the RTC's battery backup.
Attach it any time — nothing about the merge or pin map changes.

---

## 5. Water pump

Two supported approaches:

1. **Plain zone (simplest):** designate one relay (e.g. relay 8, GPIO 13) as the pump in the web UI. Open it in the same
   program as your irrigation zones. No code change.
2. **Master valve / auto pump:** `SEPARATE_MASTER_VALVE` on **GPIO 19** — the pump runs automatically whenever any zone
   is active, so it always matches the zones and keeps all 8 relays for zones.

---

## 6. Build / flash

```bash
pio run -e esp32_sprinkler                  # compile (verified: RAM ~21%, Flash ~77%)
pio run -e esp32_sprinkler -t upload        # flash over USB
```

Flash layout: `ESP32_FLASH_4MB` (default). Change in `esp32.h` if your module is 8/32 MB.

---

## 7. Background

- **Base:** V1pr's ESP32 port (forked from JaCharer's ESP32 port) of the OpenSprinkler Unified Firmware.
- **Merged:** upstream `OpenSprinkler/OpenSprinkler-Firmware` master (2.2.1(5)) into the ESP32 port, keeping all
  ESP32-specific code.
- Display options: `USE_SSD1306` (0.96") or `USE_SH1106` (1.3") in `defines.h`.
- Legacy button menu (`BOOT_MENU_V2`) and rotary-encoder support are compiled in; the EC11 drives the menu when
  `USE_ROTARY_ENCODER` is defined (it is, in `esp32.h`).

> **Use at your own risk.** This is bespoke/experimental hardware + firmware — be careful connecting external/high-power
> devices and always test with a dummy load first.
