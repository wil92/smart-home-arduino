//
// Created by guillermo on 11/18/24.
//

#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H

#include <EEPROM.h>

#define STRINGIGY(v) #v
#define TOSTRING(v) STRINGIGY(v)

class FlashManager {
public:
    bool isSetup = false;

    // Hotpot. todo: add this variables to the env
    const char *ssid = "SmartHomeLight";
    const char *password = "smarthome";

    // Network
    char *ssidNetwork = TOSTRING(NETWORK_SSID);
    char *passwordNetwork = TOSTRING(NETWORK_PASSWORD);

    // Server config
    uint16_t port = PORT;
    const char *host = TOSTRING(HOST);
    const char *url = TOSTRING(URL);

    // Device configuration
    const char *ID = "Yztyqd1Ops0QAXfhxMs2";
    const char *name = "Luz1";
    const char *type = "action.devices.types.OUTLET";

    void loadSetup();
};


#endif //FLASHMANAGER_H
