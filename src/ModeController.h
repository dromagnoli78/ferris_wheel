#ifndef ModeController_h
#define ModeController_h
#include "Constants.h"
#include <Arduino.h>

class ModeController {
private:
    int mode;
public:
    void begin() {
        if (CURRENT_MODE > DEBUG_MODE) {
            Serial.begin(115200);
            Serial.println("Setting up");
        }
    };
    bool isSleeping() {
        return mode == SLEEPING_MODE;
    };

    bool isSettings() {
        return mode == SETTING_MODE;
    };

    bool isWorking() {
        return mode == WORKING_MODE;
    };

    void settings() {
        mode = SETTING_MODE;
    }

    void sleep() {
        mode = SLEEPING_MODE;
    }
};


static void dbg(const char* message) {
    if (CURRENT_MODE > DEBUG_MODE){
        Serial.println(message);
    }
};

static void dbg(const char* header, const char value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }   
};

static void dbg(const char* header, const char* value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
};

static void dbg(const char* header, bool value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
};


static void dbg(const char* header, unsigned long value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
};

static void dbg(const char* header, int value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
};

static void dbg(unsigned long value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.println(value);
    }
};


#endif