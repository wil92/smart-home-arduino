//
// Created by guillermo on 11/18/24.
//

#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H

#include <EEPROM.h>
#include <WString.h>

#define STRINGIGY(v) #v
#define TOSTRING(v) STRINGIGY(v)

#define IS_ENABLE_FLAG 7
#define CONFIG_STATUS_ADDRESS 0
#define SWITCH_STATUS_ADDRESS 1
#define CONFIG_DATA_ADDRESS 2


class FlashManager {
    bool switchStatus = true;

    void saveString(String value, int *index);

    String loadString(int *index);

public:
    bool isSetup = false;

    // Hotpot. todo: add this variables to the env
    const char *ssid = "SmartHomeDevice";
    const char *password = "smarthome";

    // Network
    String ssidNetwork = TOSTRING(NETWORK_SSID);
    String passwordNetwork = TOSTRING(NETWORK_PASSWORD);

    // Server config
    uint16_t port = PORT;
    String host = TOSTRING(HOST);
    String url = TOSTRING(URL);

    // Device configuration
    String ID = "";
    String name = "Luz";
    String type = "action.devices.types.OUTLET";

    void loadSetup();

    void saveSetup();

    [[nodiscard]] bool getSwitchStatus() const;

    void setSwitchStatus(bool status);

    void setVariable(const String &key, const String &value);
};


#endif //FLASHMANAGER_H
