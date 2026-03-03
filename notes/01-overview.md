# Overview

## Назначение

Скетч управляет вспышкой ESP32-CAM (GPIO4) как `switch` в Home Assistant через MQTT.

## Логика работы

1. Генерируется уникальный `nodeId` из MAC (`esp32cam_<suffix>`).
2. Формируются MQTT-топики состояния, команды и availability.
3. Устройство подключается к Wi-Fi и MQTT.
4. Публикуется Home Assistant MQTT discovery config.
5. При команде `ON/OFF` в `commandTopic` меняется состояние GPIO4.
6. Текущее состояние и доступность публикуются как retained.

## Важные сущности

- `FLASH_PIN = 4` - пин вспышки.
- `setFlash(bool on)` - переключение GPIO + публикация state.
- `handleMqttMessage(...)` - обработка команд `ON/OFF`.
- `publishDiscovery()` - регистрация switch в Home Assistant.
- `connectWiFi()` / `connectMqtt()` - восстановление соединений.
