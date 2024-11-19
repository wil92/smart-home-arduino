//
// Created by guillermo on 11/18/24.
//

#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H

#include <EEPROM.h>
#include <WString.h>

#define STRINGIGY(v) #v
#define TOSTRING(v) STRINGIGY(v)

class FlashManager {
private:
    void saveString(String value, int *index);

    String loadString(int *index);

public:
    bool isSetup = false;

    // Hotpot. todo: add this variables to the env
    const char *ssid = "SmartHomeLight";
    const char *password = "smarthome";

    // Network
    String ssidNetwork = TOSTRING(NETWORK_SSID);
    String passwordNetwork = TOSTRING(NETWORK_PASSWORD);

    // Server config
    uint16_t port = PORT;
    String host = TOSTRING(HOST);
    String url = TOSTRING(URL);

    // Device configuration
    String ID = "Yztyqd1Ops0QAXfhxMs2";
    String name = "Luz1";
    String type = "action.devices.types.OUTLET";

    void loadSetup();

    void saveSetup();

    void setVariable(const String &key, const String &value);
};


#endif //FLASHMANAGER_H
