#ifndef SMART_HOME_MESSAGEOUT_H
#define SMART_HOME_MESSAGEOUT_H

#include <ArduinoJson.h>

struct MessageOut {
    static void buildOutMessage(
            const char* mid,
            const char* messageType,
            const char* ID,
            const char* type,
            const char* name,
            bool status,
            char *json);
};

#endif //SMART_HOME_MESSAGEOUT_H
