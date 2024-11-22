//
// Created by guille on 30.06.22.
//

#ifndef SMART_HOME_WEBSOCKETMANAGER_H
#define SMART_HOME_WEBSOCKETMANAGER_H

#define CONNECTION_LOST_TIMEOUT 60000

#include <SocketIOclient.h>
#include "../communication/MessageIn.h"
#include "../flash/FlashManager.h"

typedef std::function<void(bool status)> UpdateStatusEvent;

struct DeviceConfig {
    const char *ID;
    const char *type;
    const char *name;
};

class WebsocketManager {
    WebSocketsClient webSocket;
    UpdateStatusEvent updateStatusEvent;
    unsigned long lastConnection = 0;

public:
    FlashManager *flashManager = nullptr;
    DeviceConfig config = {};

    void settingUpWebSocket(WebSocketsClient::WebSocketClientEvent webSocketClientEvent,
                            uint16_t port,
                            const char *host,
                            const char *url);

    bool isConnectionAlive();

    void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

    void messageReceived(MessageIn msg);

    void sendCurrentStatus(const char *mid, const char *messageType);

    void loop();

    void onUpdateStatusEvent(UpdateStatusEvent updateStatusEvent);
};


#endif //SMART_HOME_WEBSOCKETMANAGER_H
