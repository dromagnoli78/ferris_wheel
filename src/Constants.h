#ifndef Constant_h
#define Constant_h

// Disabled
#define ENABLED 1
#define CTL_DISABLED 0
#define CONTROL_STEPPER ENABLED
#define CONTROL_MUSIC ENABLED
#define CONTROL_DISPLAY ENABLED
#define CONTROL_BUTTON ENABLED
#define CONTROL_LIGHTS ENABLED
#define CONTROL_CONSOLE_LIGHTS ENABLED
#define CONTROL_CONSOLE ENABLED
#define CONTROL_RTC ENABLED
#define CONTROL_SLEEP_MODE ENABLED

#define DEBUG_MODE 1
#define CLEAN_MODE -1
#define DETAILS_MODE 2
#define CURRENT_MODE DEBUG_MODE +1
#define SLEEP_SONG 5

#define VOLUME_PIN 15
#define MP3_RX_PIN 16
#define MP3_TX_PIN 17
#define SENSOR_PIN 35



# define SDApin 21
# define SCLpin 22
 
// Buttons Signals
#define EXPANDER_MASK 0b00000000
#define BUTTON_MUSIC 34
#define BUTTON_MUTE 34
#define BUTTON_LED_SEQUENCE 34
#define BUTTON_STEPPER 34
#define BUTTON_SLEEPING 39

#define BUTTON_SETTINGS 5
//#define BUTTON_SETTINGS 27
#define BUTTON_ARROW_LEFT 34
#define BUTTON_ARROW_RIGHT 34

#define BUTTON_ARROW_UP 34
#define BUTTON_ARROW_DOWN 34
#define BUTTONS_INTERRUPT_PIN 34

#define BUTTON_CONTROLS_DELTA_TIME 200
#define BUTTON_PRESSED HIGH

// Stepper
#define STEPPER_INCREMENT 1
#define STEPPER_IN_1 25
#define STEPPER_IN_2 33
#define STEPPER_IN_3 32
#define STEPPER_IN_4 13
#define STEPS_PER_REVOLUTION 4096
#define STEPPER_SPEEDS 3
#define STEPPER_SPEED_SLOW 200
#define STEPPER_SPEED_MED 90
#define STEPPER_SPEED_FAST 30
#define STEPPER_INIT_SPEED STEPPER_SPEED_MED


// DELAYS
#define SETUP_DELAY 2000
#define DELAY_SOFT_START_SLEEP 5000
#define DELAY_AFTER_SONG 10000
#define DELAY_AFTER_HELLO 10000

//#define SLEEP_SHUTDOWN 600000
#define SLEEP_SHUTDOWN 60*1000

/////////////////////// Leds
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 140

// Music
#define DELTA_MUSIC_TIME 100
#define DELTA_VOLUME_TIME 30

// Wheel
#define WHEEL_NUM_LEDS 8
#define WHEEL_LIGHTS_DATA_PIN 23
#define PATTERN_PERIOD 12
#define ENABLE_POWER_LED 19

// LED SEQUENCE TIMING
#define LIGHTS_DELAY 150
#define DELTA_RGB_LOOP 10
#define DELTA_TIME_RAINBOW 40
#define DELTA_SLEEPING_SLOW 80
#define DELTA_SLEEPING_FAST 20
#define MAX_VALUE_LIGHT_FOR_SLEEP 50
#define MIN_VALUE_LIGHT_FOR_SLEEP 5
#define SLEEP_THRESHOLD 5
#define WHEEL_LOOPS 44

#define LED_BRIGHTNESS 96

// Console
#define EYES_LED_DATA_PIN 26
#define CONSOLE_LIGHTS_CONTROLLER_DATA_PIN 18
//#define CONSOLE_LIGHTS_CONTROLLER_DATA_PIN 14

#define DELTA_TIME_CONSOLE_UPDATES 200
#define CONSOLE_LIGHTS_INTENSITY 100
#define SLEEP_LED_INTENTISY 10

static void dbg(const char* message) {
    if (CURRENT_MODE > DEBUG_MODE)
        Serial.println(message);
}

static void dbg(const char* header, const char value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
}

static void dbg(const char* header, const char* value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
}

static void dbg(const char* header, bool value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
}

static void dbg(const char* header, unsigned long value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
}

static void dbg(const char* header, int value) {
    if (CURRENT_MODE > DEBUG_MODE) {
        Serial.print(header);
        Serial.println(value);
    }
}

static void dbg(unsigned long value) {
    if (CURRENT_MODE > DEBUG_MODE)
        Serial.println(value);
}
#endif