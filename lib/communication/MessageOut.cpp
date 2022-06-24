//
// Created by guille on 24.06.22.
//

#include "MessageOut.h"

void MessageOut::buildOutMessage(
        const char *mid,
        const char *messageType,
        const char *ID,
        const char *type,
        const char *name,
        bool status,
        char *json) {

    DynamicJsonDocument doc(1024);
    doc["mid"] = mid;
    doc["messageType"] = messageType;
    doc["payload"]["id"] = ID;
    doc["payload"]["type"] = type;
    doc["payload"]["name"]["name"] = name;
    doc["payload"]["on"] = status;

    serializeJson(doc, json, 300);
}
