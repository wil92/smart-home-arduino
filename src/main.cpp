#include <cstdio>

#include <Arduino.h>
#include "../lib/communication/MessageIn.h"
#include "../lib/websocket/WebsocketManager.h"
#include "../lib/network/NetworkManager.h"

#if defined(ENV) && ENV == 1
int RELAY_PIN = LED_BUILTIN;
#else
int RELAY_PIN = 2;
#endif

// Hotpot credentials
const char *ssid = "SmartHomeLight";
const char *password = "smarthome";

// Network credentials
char *ssidNetwork = TOSTRING(NETWORK_SSID);
char *passwordNetwork = TOSTRING(NETWORK_PASSWORD);

// device static configuration
const char *ID = "Yztyqd1Ops0QAXfhxMs2";
const char *name = "Luz1";
const char *type = "action.devices.types.OUTLET";

NetworkManager networkManager;
WebsocketManager websocketManager = WebsocketManager({ID, type, name});

void updateRelayPin(bool s) {
    if (s) {
        digitalWrite(RELAY_PIN, LOW);
    } else {
        digitalWrite(RELAY_PIN, HIGH);
    }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {

    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            Serial.printf("[WSc] Connected to url: %s\n", payload);

            // send message to server when Connected
            websocketManager.sendCurrentStatus("", "QUERY");
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] get text: %s\n", payload);

            // send message to server
            // webSocket.sendTXT("message here");
            websocketManager.messageReceived(MessageIn::parseObject(payload));
            break;
        case WStype_PING:
            // pong will be send automatically
            Serial.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            Serial.printf("[WSc] get pong\n");
            break;
    }

}

void setup() {
    Serial.begin(9600);

    pinMode(RELAY_PIN, OUTPUT);
    updateRelayPin(websocketManager.isStatus());

    // create hotpot
//    networkManager.createHostpot(ssid, password);
//    delay(100);

    // connect to network
    networkManager.connectToNetwork(ssidNetwork, passwordNetwork);

    websocketManager.onUpdateStatusEvent(updateRelayPin);
    websocketManager.settingUpWebSocket(webSocketEvent);
}

void loop() {
    // scan networks
//    networkManager.loopScanNetworks();

    websocketManager.loop();
}
