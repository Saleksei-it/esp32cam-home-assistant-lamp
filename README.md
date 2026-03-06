# ESP32-CAM Home Assistant Lamp

ESP32-CAM project for Home Assistant.

Current implemented stage:

- ESPHome firmware for ESP32-CAM
- Home Assistant integration via ESPHome API
- 4 GPIO switches exposed to Home Assistant:
  - `GPIO4` (flash LED)
  - `GPIO13`
  - `GPIO14`
  - `GPIO15`

This repository is the working base for further lamp/automation features.

## Files

- `esp32cam_flash_esphome.yaml` - current ESPHome firmware config
- `esp32cam_flash_ha.ino` - earlier MQTT-based Arduino sketch (kept as reference)
- `secrets.example.yaml` - template for local Wi-Fi secrets
- `notes/` - design notes and MQTT reference from the first iteration

## Hardware

- Board: ESP32-CAM (AI Thinker style pinout)
- Current outputs:
  - `GPIO4` - onboard flash LED
  - `GPIO13` - Channel 1
  - `GPIO14` - Channel 2
  - `GPIO15` - Channel 3

## Current Home Assistant Setup

The device is added to Home Assistant through the `ESPHome` integration.

Current tested behavior:

1. Device boots and joins Wi-Fi
2. ESPHome API is available on port `6053`
3. Home Assistant discovers/loads the node and creates 4 switch entities:
   - `switch.esp32_cam_flash_flash_led_gpio4`
   - `switch.esp32_cam_flash_channel_1_gpio13`
   - `switch.esp32_cam_flash_channel_2_gpio14`
   - `switch.esp32_cam_flash_channel_3_gpio15`
4. All 4 switches are verified via `switch.turn_on` / `switch.turn_off`

## Local Setup

1. Copy `secrets.example.yaml` to `secrets.yaml`
2. Fill in your Wi-Fi values
3. Build and flash:

```bash
~/.platformio/penv/bin/esphome run esp32cam_flash_esphome.yaml --device /dev/cu.usbserial-XXXX
```

4. Add the device in Home Assistant:

- Go to `Settings -> Devices & Services`
- Add integration `ESPHome`
- Enter the device IP address

## Current ESPHome Config

- Name: `esp32cam-flash`
- Friendly name: `ESP32-CAM Flash`
- Web UI enabled on port `80`
- OTA enabled
- Native API enabled
- 4 switch outputs mapped to GPIO4/13/14/15

## Pin Notes

- `GPIO15` is a strapping pin on ESP32. Avoid adding strong external pull-up/pull-down on this line to prevent boot issues.

## Roadmap

- Add safe power-stage examples (MOSFET/relay)
- Optional migration from GPIO15 to another pin profile, if hardware constraints require it
- Add cleaner Home Assistant dashboard controls
- Split hardware profiles (flash-only vs external lamp)

## Notes

The repository intentionally excludes private credentials and Home Assistant tokens.
