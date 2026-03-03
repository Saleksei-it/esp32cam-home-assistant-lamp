# ESP32-CAM Home Assistant Lamp

ESP32-CAM project for Home Assistant.

Current implemented stage:

- ESPHome firmware for ESP32-CAM
- Home Assistant integration via ESPHome API
- Flash LED on `GPIO4` exposed as a Home Assistant switch
- Verified entity in Home Assistant: `switch.esp32_cam_flash_flash`

This repository is the base project that will be extended further into a multi-channel lamp controller.

## Files

- `esp32cam_flash_esphome.yaml` - current ESPHome firmware config
- `esp32cam_flash_ha.ino` - earlier MQTT-based Arduino sketch (kept as reference)
- `secrets.example.yaml` - template for local Wi-Fi secrets
- `notes/` - design notes and MQTT reference from the first iteration

## Hardware

- Board: ESP32-CAM (AI Thinker style pinout)
- Current output: onboard flash LED on `GPIO4`

## Current Home Assistant Setup

The device is added to Home Assistant through the `ESPHome` integration.

Current tested behavior:

1. Device boots and joins Wi-Fi
2. ESPHome API is available on port `6053`
3. Home Assistant discovers the node and creates `switch.esp32_cam_flash_flash`
4. Turning the switch on/off controls the onboard flash LED

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

## Roadmap

- Add external lamp output pin
- Extend to 3-channel lamp control
- Add cleaner Home Assistant dashboard controls
- Split hardware profiles (flash-only vs external lamp)

## Notes

The repository intentionally excludes private credentials and Home Assistant tokens.
