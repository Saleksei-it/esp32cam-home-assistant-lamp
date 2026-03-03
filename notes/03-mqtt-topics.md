# MQTT Topics

`<nodeId> = esp32cam_<macSuffix>`

## Runtime topics

| Назначение | Топик | Payload |
|---|---|---|
| Доступность (LWT/online) | `ha/<nodeId>/status` | `online` / `offline` |
| Команда | `ha/<nodeId>/flash/set` | `ON` / `OFF` |
| Состояние | `ha/<nodeId>/flash/state` | `ON` / `OFF` |

## Discovery topic

- `homeassistant/switch/<nodeId>/flash/config` - JSON-конфиг для автообнаружения switch в Home Assistant.

## Поведение retained

- Discovery и state публикуются с retained.
- Availability также публикуется retained + LWT (`offline`), чтобы Home Assistant корректно показывал статус после перезапуска брокера/устройства.
