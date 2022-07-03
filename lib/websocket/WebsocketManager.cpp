//
// Created by guille on 30.06.22.
//

#include "WebsocketManager.h"

#define STRINGIGY(v) #v
#define TOSTRING(v) STRINGIGY(v)
#define EXPAND_TO(v) _##v##_
#define EXPAND(v) EXPAND_TO(v)

#define ttt(v) ~(~v + 0) == 0 && ~(~v + 1) == 1
#define tt(v) ttt(v)

WebsocketManager::WebsocketManager(DeviceConfig config) {
    WebsocketManager::config = config;
    status = true;
}

void WebsocketManager::settingUpWebSocket(WebSocketsClient::WebSocketClientEvent webSocketClientEvent) {

    uint16_t port = PORT;
    const char *host = TOSTRING(HOST);
    const char *url = TOSTRING(URL);

#if defined(WSS) && WSS == 1
    X509List *certCA = new X509List(home_url_CA);
    webSocket.beginSslWithCA(host, port, url, certCA);
#else
    webSocket.begin(host, port, url);
#endif

    // event handler
    webSocket.onEvent(webSocketClientEvent);

    // use HTTP Basic Authorization this is optional remove if not needed
//    webSocket.setAuthorization("user", "Password");

    // try ever 5000 again if connection has failed
    webSocket.setReconnectInterval(5000);
}

void WebsocketManager::messageReceived(MessageIn msg) {
    if (!std::strcmp(msg.payload.messageType, "EXECUTE")) {
        status = msg.payload.command.on;
        updateStatusEvent(status);
    }
    sendCurrentStatus(msg.mid, msg.payload.messageType);
}

void WebsocketManager::sendCurrentStatus(const char* mid, const char* messageType) {
    char json[400];
    MessageOut::buildOutMessage(
            mid,
            messageType,
            config.ID,
            config.type,
            config.name,
            status,
            json);
    webSocket.sendTXT(json);
}

void WebsocketManager::loop() {
    webSocket.loop();
}

void WebsocketManager::onUpdateStatusEvent(UpdateStatusEvent updateStatusEvent) {
    WebsocketManager::updateStatusEvent = updateStatusEvent;
}

bool WebsocketManager::isStatus() const {
    return status;
}
