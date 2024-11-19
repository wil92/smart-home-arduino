#include <cstdio>

#include <Arduino.h>

#include "../lib/communication/MessageIn.h"
#include "../lib/websocket/WebsocketManager.h"
#include "../lib/network/NetworkManager.h"
#include "../lib/flash/FlashManager.h"
#include "../lib/webserver/WebServerManager.h"

#if defined(ENV) && ENV == 1
// nodemcu
#define RELAY_PIN LED_BUILTIN
#define ACTION_PIN 1
#else
// esp01
#define RELAY_PIN 2
#define ACTION_PIN 1
#endif

NetworkManager networkManager;
FlashManager flashManager;
WebServerManager webServer;
WebsocketManager websocketManager = WebsocketManager({flashManager.ID, flashManager.type, flashManager.name});

void (*resetFunc)(void) = 0;

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

    delay(3000);

    Serial.println("Hello World!");

    pinMode(RELAY_PIN, OUTPUT);
    updateRelayPin(websocketManager.isStatus());

    if (flashManager.isSetup) {
        // connect to network
        networkManager.connectToNetwork(flashManager.ssidNetwork, flashManager.passwordNetwork);

        websocketManager.onUpdateStatusEvent(updateRelayPin);
        websocketManager.settingUpWebSocket(webSocketEvent, flashManager.port, flashManager.host, flashManager.url);
    } else {
        // create hotpot
        if (!networkManager.createHostpot(flashManager.ssid, flashManager.password)) {
            // reset arduino if hotpot fails to start
            resetFunc();
        }
        delay(100);

        // start web server
        webServer.setup();
        webServer.run();
    }
}

void loop() {
    if (flashManager.isSetup) {
        websocketManager.loop();

        // check if websocket connected or reset arduino
        if (websocketManager.isConnectionAlive()) {
            resetFunc();
        }
    } else {
        // scan networks
        webServer.loop();
        // Serial.println("[WServer] loop");
        //    networkManager.loopScanNetworks();
    }
}
