#ifndef ModeController_h
#define ModeController_h
#include "Constants.h"
#include <Arduino.h>

class ModeController {
private:
    int mode;
    int previousMode;
public:
    void begin() {
        if (CURRENT_MODE > DEBUG_MODE) {
            Serial.begin(115200);
            Serial.println("Setting up");
        }
        mode = START_MODE;
        previousMode = START_MODE;
    };
    bool isStart(){return mode == START_MODE;}
    bool isSleeping() {return mode == SLEEPING_MODE;}
    bool isSettings() {return mode == SETTING_MODE;}
    bool isWorking() {return mode == WORKING_MODE;}
    bool isOff() {return mode == OFF_MODE;}
    void settings() {previousMode = mode, mode = SETTING_MODE;}
    void sleep() {previousMode = mode, mode = SLEEPING_MODE;}
    void working() {previousMode = mode, mode = WORKING_MODE;}
    void off(){mode = OFF_MODE;}
    void back(){mode = previousMode;}
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

static const char* toText(int number){
    char buffer[30]; 
    sprintf(buffer, "%d", number);
    const char* charText = buffer;
    return charText;
}

static const char* toText(u_int16_t number){
    char buffer[30]; 
    sprintf(buffer, "%d", number);
    const char* charText = buffer;
    return charText;
}

static const char* toText(unsigned int number){
    char buffer[30]; 
    sprintf(buffer, "%d", number);
    const char* charText = buffer;
    return charText;
}
#endif