//
// Created by guillermo on 11/18/24.
//

#include "FlashManager.h"

void FlashManager::loadSetup() {
    if (EEPROM.read(0)) {
        // todo: update this to 'true'
        isSetup = false;

        // todo: load other configurations
    } else {
        isSetup = false;
    }
}
