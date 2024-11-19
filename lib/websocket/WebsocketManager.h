//
// Created by guille on 30.06.22.
//

#ifndef SMART_HOME_WEBSOCKETMANAGER_H
#define SMART_HOME_WEBSOCKETMANAGER_H

#define CONNECTION_LOST_TIMEOUT 60000

#include <SocketIOclient.h>
#include "../communication/MessageIn.h"

typedef std::function<void(bool status)> UpdateStatusEvent;

struct DeviceConfig {
    const char *ID;
    const char *type;
    const char *name;
};

class WebsocketManager {
    WebSocketsClient webSocket;
    UpdateStatusEvent updateStatusEvent;
    bool status = true;
    unsigned long lastConnection = 0;

public:
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

    bool isStatus() const;
};


#endif //SMART_HOME_WEBSOCKETMANAGER_H
