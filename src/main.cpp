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
#define ACTION_PIN 13
#else
// esp01
#define RELAY_PIN 2
#define ACTION_PIN 0
#endif

NetworkManager networkManager;
FlashManager flashManager;
WebServerManager webServer;
WebsocketManager websocketManager;

void (*resetFunc)(void) = 0;

typedef std::function<void(bool status)> UpdateStatusEvent;
void updateRelayPin(const bool status) {
    digitalWrite(RELAY_PIN, status ? LOW : HIGH);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            // send message to server when Connected
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            websocketManager.sendCurrentStatus("", "QUERY");
            break;
        case WStype_TEXT:
            // send message to server
            Serial.printf("[WSc] get text: %s\n", payload);
            websocketManager.messageReceived(MessageIn::parseObject(payload));
            break;
        case WStype_PING:
            // pong will be sent automatically
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
    EEPROM.begin(512);

    // todo: remove this delay
    delay(3000);

    flashManager.loadSetup();

    pinMode(RELAY_PIN, OUTPUT);
    pinMode(ACTION_PIN, INPUT);
    updateRelayPin(flashManager.getSwitchStatus());

    if (flashManager.isSetup) {
        // connect to network
        networkManager.connectToNetwork(flashManager.ssidNetwork.c_str(), flashManager.passwordNetwork.c_str());

        websocketManager.flashManager = &flashManager;
        websocketManager.config = {
            flashManager.ID.c_str(),
            flashManager.type.c_str(),
            flashManager.name.c_str()
        };
        websocketManager.onUpdateStatusEvent(updateRelayPin);
        websocketManager.settingUpWebSocket(webSocketEvent, flashManager.port, flashManager.host.c_str(),
                                            flashManager.url.c_str());
    } else {
        // create hotpot
        if (!networkManager.createHostpot(flashManager.ssid, flashManager.password)) {
            // reset arduino if hotpot fails to start
            resetFunc();
        }
        delay(100);

        // start web server
        webServer.flashManager = &flashManager;
        webServer.rf = resetFunc;
        webServer.setup();
        webServer.run();
    }
}

void loop() {
    if (flashManager.isSetup) {
        websocketManager.loop();

        // check if websocket is connected or reset arduino
        if (!websocketManager.isConnectionAlive()) {
            Serial.println("Connection lost");
            resetFunc();
        }

        // check if action pin is pressed
        if (digitalRead(ACTION_PIN) == LOW) {
            Serial.println("Reset configuration executed");
            EEPROM.write(0, 0);
            EEPROM.commit();
            resetFunc();
        }
    } else {
        // scan networks
        webServer.loop();
    }
}
