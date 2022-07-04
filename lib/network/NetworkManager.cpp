//
// Created by guille on 30.06.22.
//

#include "NetworkManager.h"

NetworkManager::NetworkManager() {
    local_ip = IPAddress(192, 168, 1, 1);
    gateway = IPAddress(192, 168, 1, 1);
    subnet = IPAddress(255, 255, 255, 0);
}

void NetworkManager::connectToNetwork(char *ssidNetwork, char *passwordNetwork) {
    wiFiMulti.addAP(ssidNetwork, passwordNetwork);
    Serial.print("Connecting to ");
    Serial.print(ssidNetwork);
    Serial.println(" ...");

    int i = 0;
    while (wiFiMulti.run() != WL_CONNECTED) {
        delay(1000);
        Serial.print(++i);
        Serial.print('.');
    }

    Serial.println('\n');
    Serial.println("Connection established!");
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
}

void NetworkManager::createHostpot(const char *ssidNetwork, const char *passwordNetwork) {
    WiFi.softAP(ssidNetwork, passwordNetwork);
    WiFi.softAPConfig(local_ip, gateway, subnet);
}

void NetworkManager::scanNetworks() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        Serial.println(WiFi.SSID(i));
    }
}

void NetworkManager::loopScanNetworks() {
    unsigned long current = micros();
    if ((current - lastNetworkScan) / NETWORK_SCAN_INTERVAL_IN_MICRO_SEC) {
        lastNetworkScan = current;
        if (lastNetworkScan) {
            lastNetworkScan = 0;
            scanNetworks();
        }
    }
}
