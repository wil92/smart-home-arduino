//
// Created by guille on 30.06.22.
//

#include "WebsocketManager.h"

#include "certs.h"
#include "../communication/MessageOut.h"

void WebsocketManager::settingUpWebSocket(WebSocketsClient::WebSocketClientEvent webSocketClientEvent,
                                          uint16_t port,
                                          const char *host,
                                          const char *url) {
    // todo: this should be configurable too
#if defined(WSS) && WSS == 1
    X509List *certCA = new X509List(home_url_CA);
    webSocket.beginSslWithCA(host, port, url, certCA, "wss");
#else
    webSocket.begin(host, port, url);
#endif

    // event handler
    webSocket.onEvent(webSocketClientEvent);

    // use HTTP Basic Authorization this is optional remove if not needed
    //    webSocket.setAuthorization("user", "Password");

    // try ever 5000 again if connection has failed
    webSocket.setReconnectInterval(5000);

    // initialize lastConnectionValue
    lastConnection = millis();
}

bool WebsocketManager::isConnectionAlive() {
    return millis() - lastConnection <= CONNECTION_LOST_TIMEOUT ||
           webSocket.isConnected();
}

void WebsocketManager::messageReceived(MessageIn msg) {
    if (!std::strcmp(msg.payload.messageType, "EXECUTE")) {
        flashManager->setSwitchStatus(!msg.payload.command.on);
        updateStatusEvent(flashManager->getSwitchStatus());
    }
    sendCurrentStatus(msg.mid, msg.payload.messageType);
}

void WebsocketManager::sendCurrentStatus(const char *mid, const char *messageType) {
    char json[400];
    MessageOut::buildOutMessage(
        mid,
        messageType,
        config.ID,
        config.type,
        config.name,
        !flashManager->getSwitchStatus(),
        json);
    webSocket.sendTXT(json);
}

void WebsocketManager::loop() {
    if (webSocket.isConnected()) {
        lastConnection = millis();
    }

    webSocket.loop();
}

void WebsocketManager::onUpdateStatusEvent(UpdateStatusEvent updateStatusEvent) {
    WebsocketManager::updateStatusEvent = updateStatusEvent;
}
