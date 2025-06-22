/*
 * @Author: Lieyan
 * @Date: 2025-06-22 12:57:53
 * @LastEditors: Lieyan
 * @LastEditTime: 2025-06-22 14:05:22
 * @FilePath: /FireNote-ESP32/src/wifi_manager.cpp
 * @Description: 
 * @Contact: QQ: 2102177341  Website: lieyan.space  Github: @lieyan666
 * @Copyright: Copyright (c) 2025 by lieyanDevTeam, All Rights Reserved. 
 */
#include "wifi_manager.h"
#include <Arduino.h>

void wifiManagerInit() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    Serial.println("WiFi Manager Initialized.");
}

void connectToWiFi(const char* ssid, const char* password) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 30) { // ~15 seconds timeout
        delay(500);
        Serial.print(".");
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi.");
    }
}

bool isWifiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void disconnectWiFi() {
    WiFi.disconnect();
    Serial.println("WiFi disconnected.");
}
