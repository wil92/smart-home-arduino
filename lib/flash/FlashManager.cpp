//
// Created by guillermo on 11/18/24.
//

#include "FlashManager.h"

#include <HardwareSerial.h>

void FlashManager::loadSetup() {
    if (int index = 0; EEPROM.read(index)) {
        isSetup = true;
        index++;

        // device
        ID = loadString(&index);
        name = loadString(&index);
        type = loadString(&index);

        // network
        ssidNetwork = loadString(&index);
        passwordNetwork = loadString(&index);

        // server
        host = loadString(&index);
        url = loadString(&index);
        port = EEPROM.read(index++);
        port |= EEPROM.read(index) << 8;

        // only on debug
        Serial.println("Config from EEPROM---");
        Serial.println(ID);
        Serial.println(name);
        Serial.println(type);
        Serial.println(ssidNetwork);
        Serial.println(passwordNetwork);
        Serial.println(host);
        Serial.println(url);
        Serial.println(port);
        Serial.println("---------------------");
    } else {
        isSetup = false;
    }
}

void FlashManager::saveSetup() {
    int index = 0;
    EEPROM.write(index++, 1);

    // device
    saveString(String(ID), &index);
    saveString(String(name), &index);
    saveString(String(type), &index);

    // network
    saveString(String(ssidNetwork), &index);
    saveString(String(passwordNetwork), &index);

    // server
    saveString(String(host), &index);
    saveString(String(url), &index);
    EEPROM.write(index++, port & ((1 << 8) - 1));
    EEPROM.write(index, (port >> 8) & ((1 << 8) - 1));

    EEPROM.commit();
    Serial.println("Data saved in flash memory");
}

void FlashManager::saveString(String value, int *index) {
    EEPROM.write(*index, value.length());
    (*index)++;
    for (int i = 0; i < static_cast<int>(value.length()); i++) {
        EEPROM.write(*index, value[i]);
        (*index)++;
    }
}

String FlashManager::loadString(int *index) {
    const int len = EEPROM.read((*index)++);
    String result = "";
    for (int i = 0; i < len; i++) {
        result += String(static_cast<char>(EEPROM.read((*index)++)));
    }
    return result;
}

void FlashManager::setVariable(const String &key, const String &value) {
    if (key == "deviceId") {
        ID = value.c_str();
    } else if (key == "deviceType") {
        type = value.c_str();
    } else if (key == "deviceName") {
        name = value.c_str();
    } else if (key == "host") {
        host = value.c_str();
    } else if (key == "port") {
        port = value.toInt();
    } else if (key == "url") {
        url = value.c_str();
    } else if (key == "ssid") {
        ssidNetwork = value.c_str();
    } else if (key == "password") {
        passwordNetwork = value.c_str();
    }
}
