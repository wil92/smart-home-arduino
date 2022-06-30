//
// Created by guille on 30.06.22.
//

#ifndef SMART_HOME_NETWORKMANAGER_H
#define SMART_HOME_NETWORKMANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

const unsigned long NETWORK_SCAN_INTERVAL_IN_MICRO_SEC = 2000000L;

class NetworkManager {
private:
    ESP8266WiFiMulti wiFiMulti;

    IPAddress local_ip;
    IPAddress gateway;
    IPAddress subnet;

    unsigned long lastNetworkScan = 1;
public:
    NetworkManager();
    void connectToNetwork(char *ssidNetwork, char *passwordNetwork);

    void createHostpot(const char *ssidNetwork, const char *passwordNetwork);

    void scanNetworks();
    void loopScanNetworks();
};


#endif //SMART_HOME_NETWORKMANAGER_H
