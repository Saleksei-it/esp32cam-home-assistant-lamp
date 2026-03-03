#include <WiFi.h>
#include <PubSubClient.h>

// ===== User settings =====
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char* MQTT_HOST = "192.168.1.10";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_USER = "mqtt_user";
const char* MQTT_PASSWORD = "mqtt_password";
// ========================

// ESP32-CAM AI Thinker flash LED pin
static const uint8_t FLASH_PIN = 4;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

String nodeId;
String deviceName;
String availabilityTopic;
String stateTopic;
String commandTopic;
String discoveryTopic;

bool flashOn = false;
unsigned long lastReconnectAttemptMs = 0;

String macSuffix() {
  uint64_t chipid = ESP.getEfuseMac();
  char buf[13];
  snprintf(buf, sizeof(buf), "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
  return String(buf);
}

void setFlash(bool on) {
  flashOn = on;
  digitalWrite(FLASH_PIN, flashOn ? HIGH : LOW);
  mqttClient.publish(stateTopic.c_str(), flashOn ? "ON" : "OFF", true);
}

void handleMqttMessage(char* topic, byte* payload, unsigned int length) {
  if (String(topic) != commandTopic) {
    return;
  }

  String msg;
  msg.reserve(length);
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();

  if (msg == "ON") {
    setFlash(true);
  } else if (msg == "OFF") {
    setFlash(false);
  }
}

void publishDiscovery() {
  String payload = "{";
  payload += "\"name\":\"" + deviceName + " Flash\",";
  payload += "\"uniq_id\":\"" + nodeId + "_flash\",";
  payload += "\"cmd_t\":\"" + commandTopic + "\",";
  payload += "\"stat_t\":\"" + stateTopic + "\",";
  payload += "\"pl_on\":\"ON\",";
  payload += "\"pl_off\":\"OFF\",";
  payload += "\"avty_t\":\"" + availabilityTopic + "\",";
  payload += "\"pl_avail\":\"online\",";
  payload += "\"pl_not_avail\":\"offline\",";
  payload += "\"icon\":\"mdi:flashlight\",";
  payload += "\"dev\":{";
  payload += "\"ids\":[\"" + nodeId + "\"],";
  payload += "\"name\":\"" + deviceName + "\",";
  payload += "\"mdl\":\"ESP32-CAM\",";
  payload += "\"mf\":\"Espressif\"";
  payload += "}";
  payload += "}";

  mqttClient.publish(discoveryTopic.c_str(), payload.c_str(), true);
}

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

bool connectMqtt() {
  if (mqttClient.connected()) {
    return true;
  }

  String clientId = nodeId + "_client";
  bool ok;
  if (strlen(MQTT_USER) == 0) {
    ok = mqttClient.connect(
      clientId.c_str(),
      availabilityTopic.c_str(),
      1,
      true,
      "offline"
    );
  } else {
    ok = mqttClient.connect(
      clientId.c_str(),
      MQTT_USER,
      MQTT_PASSWORD,
      availabilityTopic.c_str(),
      1,
      true,
      "offline"
    );
  }

  if (!ok) {
    return false;
  }

  mqttClient.publish(availabilityTopic.c_str(), "online", true);
  mqttClient.subscribe(commandTopic.c_str());
  publishDiscovery();
  mqttClient.publish(stateTopic.c_str(), flashOn ? "ON" : "OFF", true);
  return true;
}

void setup() {
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, LOW);

  String suffix = macSuffix();
  nodeId = "esp32cam_" + suffix;
  deviceName = "ESP32-CAM " + suffix;

  availabilityTopic = "ha/" + nodeId + "/status";
  stateTopic = "ha/" + nodeId + "/flash/state";
  commandTopic = "ha/" + nodeId + "/flash/set";
  discoveryTopic = "homeassistant/switch/" + nodeId + "/flash/config";

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(handleMqttMessage);

  connectWiFi();
  connectMqtt();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttemptMs > 2000) {
      lastReconnectAttemptMs = now;
      connectMqtt();
    }
  } else {
    mqttClient.loop();
  }
}
