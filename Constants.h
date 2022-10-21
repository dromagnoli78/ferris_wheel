#ifndef Constant_h
#define Constant_h

#include <FastLED.h>

#define DEBUG_MODE 1
#define CURRENT_MODE DEBUG_MODE

// Buttons Signals
#define BUTTON_LED_SEQUENCE 4
#define BUTTON_WHEEL_ROTATION 5
#define BUTTON_ARROW_LEFT 6
#define BUTTON_ARROW_RIGHT 7
#define BUTTON_ARROW_UP 8
#define BUTTON_ARROW_DOWN 9
#define BUTTON_MUTE 10
#define BUTTON_SETTINGS 11
#define BUTTON_SLEEPING 12

#define BUTTON_CONTROLS_DELTA_TIME 200
#define BUTTON_PRESSED HIGH

// Stepper
#define STEPPER_INCREMENT 3
#define STEPPER_RPM 20


#define SETUP_DELAY 3000

/////////////////////// Leds
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  96

// Wheel
#define WHEEL_NUM_LEDS 8
#define WHEEL_LIGHTS_DATA_PIN 12
#define PATTERN_PERIOD 12

// Console
#define CONSOLE_LIGHTS_CONTROLLER_DATA_PIN 13
#define CONSOLE_LIGHTS_NUM_LEDS 8

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#endif