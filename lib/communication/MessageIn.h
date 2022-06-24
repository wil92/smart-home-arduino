//
// Created by guille on 22.06.22.
//

#ifndef SMART_HOME_MESSAGEIN_H
#define SMART_HOME_MESSAGEIN_H

#include <ArduinoJson.h>

struct MessageIn {
    struct MessagePayload {
        struct MessageCommand {
            bool on;
        };

        const char *messageType;
        MessageCommand command;
    };

    const char *mid;
    MessagePayload payload;

    static MessageIn parseObject(unsigned char *payload);
};


#endif //SMART_HOME_MESSAGEIN_H
