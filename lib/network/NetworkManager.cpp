//
// Created by guille on 30.06.22.
//

#include "NetworkManager.h"

NetworkManager::NetworkManager() {
    localIp = IPAddress(192, 168, 1, 1);
    gateway = IPAddress(192, 168, 1, 1);
    subnet = IPAddress(255, 255, 255, 0);
}

void NetworkManager::connectToNetwork(const char *ssidNetwork, const char *passwordNetwork) {
    wiFiMulti = std::make_unique<ESP8266WiFiMulti>();
    wiFiMulti->addAP(ssidNetwork, passwordNetwork);
    Serial.print("Connecting to ");
    Serial.print(ssidNetwork);
    Serial.println(" ...");

    int i = 0;
    while (wiFiMulti->run() != WL_CONNECTED) {
        delay(1000);
        Serial.print('.');
    }

    Serial.println('\n');
    Serial.println("Connection established!");
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
}

bool NetworkManager::createHostpot(const char *ssidNetwork, const char *passwordNetwork) const {
    WiFi.softAPConfig(localIp, gateway, subnet);
    int tries = 10;

    Serial.println("Try to start soft AP");
    while (!WiFi.softAP(ssidNetwork, passwordNetwork) && tries-- > 0) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");

    if (tries == 0) {
        Serial.println("Soft AP failed");
        return false;
    }

    Serial.println("Soft AP ready");
    Serial.print("Local IP: ");
    Serial.println(WiFi.softAPIP());
    return true;
}
