//
// Created by guille on 22.06.22.
//

#include "MessageIn.h"

MessageIn MessageIn::parseObject(unsigned char *payload) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
//        Serial.print(F("deserializeJson() failed: "));
//        Serial.println(error.f_str());
        return {};
    }

    const char *messageId = "";
    MessagePayload messagePayload = {};
    if (doc.containsKey("mid")) {
        messageId = doc["mid"];
    }
    if (doc.containsKey("payload")) {
        const char *messageType = "";
        MessagePayload::MessageCommand command = {};
        if (doc["payload"].containsKey("messageType")) {
            messageType = doc["payload"]["messageType"];
        }
        if (doc["payload"].containsKey("command")) {
            bool on = NULL;
            if (doc["payload"]["command"].containsKey("on")) {
                on = doc["payload"]["command"]["on"];
            }
            command = {on};
        }
        messagePayload = {messageType, command};
    }

    return {messageId, messagePayload};
}
