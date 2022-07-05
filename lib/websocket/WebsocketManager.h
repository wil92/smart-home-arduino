//
// Created by guille on 30.06.22.
//

#ifndef SMART_HOME_WEBSOCKETMANAGER_H
#define SMART_HOME_WEBSOCKETMANAGER_H

#define STRINGIGY(v) #v
#define TOSTRING(v) STRINGIGY(v)

#include <SocketIOclient.h>
#include "certs.h"
#include "../communication/MessageIn.h"
#include "../communication/MessageOut.h"

typedef std::function<void(bool status)> UpdateStatusEvent;

struct DeviceConfig {
    const char *ID;
    const char *type;
    const char *name;
};

class WebsocketManager {
private:
    WebSocketsClient webSocket;
    UpdateStatusEvent updateStatusEvent;
    DeviceConfig config;
    bool status;
public:
    WebsocketManager(DeviceConfig config);

    void settingUpWebSocket(WebSocketsClient::WebSocketClientEvent webSocketClientEvent);

    void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

    void messageReceived(MessageIn msg);

    void sendCurrentStatus(const char *mid, const char *messageType);

    void loop();

    void onUpdateStatusEvent(UpdateStatusEvent updateStatusEvent);

    bool isStatus() const;
};


#endif //SMART_HOME_WEBSOCKETMANAGER_H
