#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "config.h" // 必须在 PubSubClient.h 之前引用，以确保 MQTT_MAX_PACKET_SIZE 生效
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <vector>
#include <ArduinoJson.h>
#include "drawing_history.h"

void mqttInit(const char* server, int port);
void mqttLoop();
bool isMqttConnected();
void sendStroke(const std::vector<TouchData_t>& stroke);
void sendResetMessage();

#endif // MQTT_HANDLER_H
