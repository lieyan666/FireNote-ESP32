
/*
 * @Author: Lieyan
 * @Date: 2025-06-22 12:58:18
 * @LastEditors: Lieyan
 * @LastEditTime: 2025-06-23 06:16:43
 * @FilePath: /FireNote-ESP32/src/mqtt_handler.cpp
 * @Description: 
 * @Contact: QQ: 2102177341  Website: lieyan.space  Github: @lieyan666
 * @Copyright: Copyright (c) 2025 by lieyanDevTeam, All Rights Reserved. 
 */
#include "mqtt_handler.h"
#include "config.h"
#include "ui_manager.h"
#include "touch_handler.h"
#include <Arduino.h>
#include <TFT_eSPI.h>

// External variables
extern TFT_eSPI tft;
extern TS_Point lastRemotePoint;
extern unsigned long lastRemoteDrawTime;
extern DrawingHistory allDrawingHistory;

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Forward declarations
void mqttCallback(char* topic, byte* payload, unsigned int length);
void processStroke(const JsonObject& stroke);
void processReset();
void mqttReconnect();

void mqttInit(const char* server, int port) {
    client.setServer(server, port);
    client.setCallback(mqttCallback);
    Serial.println("MQTT Handler Initialized.");
}

void mqttLoop() {
    if (!client.connected()) {
        mqttReconnect();
    }
    client.loop();
}

bool isMqttConnected() {
    return client.connected();
}

void mqttReconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "FireNote-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), DEFAULT_MQTT_USER, DEFAULT_MQTT_PASSWORD)) {
            Serial.println("connected");
            client.subscribe("firenote/strokes");
            client.subscribe("firenote/control");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void sendStroke(const std::vector<TouchData_t>& stroke) {
    if (stroke.empty()) {
        return;
    }

    StaticJsonDocument<MQTT_MAX_PACKET_SIZE - 50> doc; // Reserve 50 bytes for overhead
    doc["c"] = stroke[0].color;
    JsonArray points = doc.createNestedArray("p");
    for (const auto& p : stroke) {
        points.add(p.x);
        points.add(p.y);
    }

    char buffer[MQTT_MAX_PACKET_SIZE - 50];
    size_t n = serializeJson(doc, buffer);

    if (client.connected()) {
        if (!client.publish("firenote/strokes", buffer, n)) {
            Serial.println("MQTT publish failed. Message might be too large.");
        }
    }
}

void sendResetMessage() {
    if (client.connected()) {
        client.publish("firenote/control", "reset");
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (strcmp(topic, "firenote/strokes") == 0) {
        StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
        deserializeJson(doc, payload, length);
        processStroke(doc.as<JsonObject>());
    } else if (strcmp(topic, "firenote/control") == 0) {
        if (length == 5 && strncmp((char*)payload, "reset", 5) == 0) {
            processReset();
        }
    }
}

void processStroke(const JsonObject& stroke) {
    uint16_t color = stroke["c"];
    JsonArray points = stroke["p"];

    for (size_t i = 0; i < points.size(); i += 2) {
        TouchData_t data;
        data.x = points[i];
        data.y = points[i+1];
        data.color = color;
        data.timestamp = millis(); // Use arrival time for remote points
        data.isReset = false;

        allDrawingHistory.push_back(data);

        if (i == 0 || (data.timestamp - lastRemoteDrawTime > TOUCH_STROKE_INTERVAL)) {
            tft.drawPixel(data.x, data.y, data.color);
        } else {
            tft.drawLine(lastRemotePoint.x, lastRemotePoint.y, data.x, data.y, data.color);
        }

        lastRemotePoint.x = data.x;
        lastRemotePoint.y = data.y;
        lastRemotePoint.z = 1;
        lastRemoteDrawTime = data.timestamp;
    }
}

void processReset() {
    clearScreenAndCache();
}
