/*
 * @Author: Lieyan
 * @Date: 2025-06-22 12:56:31
 * @LastEditors: Lieyan
 * @LastEditTime: 2025-06-22 12:57:49
 * @FilePath: /FireNote-ESP32/src/wifi_manager.h
 * @Description: 
 * @Contact: QQ: 2102177341  Website: lieyan.space  Github: @lieyan666
 * @Copyright: Copyright (c) 2025 by lieyanDevTeam, All Rights Reserved. 
 */
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

void wifiManagerInit();
void connectToWiFi(const char* ssid, const char* password);
bool isWifiConnected();
void disconnectWiFi();

#endif // WIFI_MANAGER_H
