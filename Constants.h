#ifndef Constant_h
#define Constant_h

#include <FastLED.h>

#define DEBUG_MODE 1
#define CURRENT_MODE DEBUG_MODE

#define VOLUME_PIN 27
#define MP3_RX_PIN 16
#define MP3_TX_PIN 17

// Buttons Signals
#define BUTTON_MUSIC 12
#define BUTTON_MUTE 13
#define BUTTON_LED_SEQUENCE 14
#define BUTTON_STEPPER 15
#define BUTTON_SLEEPING 18

#define BUTTON_SETTINGS 19
#define BUTTON_ARROW_LEFT 35
#define BUTTON_ARROW_RIGHT 34

#define BUTTON_ARROW_UP 36
#define BUTTON_ARROW_DOWN 39

#define BUTTON_CONTROLS_DELTA_TIME 300
#define BUTTON_PRESSED HIGH

// Stepper
#define STEPPER_INCREMENT 1
#define STEPPER_RPM 5
#define STEPPER_IN_1 26
#define STEPPER_IN_2 25
#define STEPPER_IN_3 33
#define STEPPER_IN_4 32
#define STEPS_PER_REVOLUTION 4096
#define STEPPER_DELTA_TIME 50

#define SETUP_DELAY 3000

/////////////////////// Leds
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 96

// Wheel
#define WHEEL_NUM_LEDS 8
#define WHEEL_LIGHTS_DATA_PIN 16
#define PATTERN_PERIOD 12

// Console
#define CONSOLE_LIGHTS_CONTROLLER_DATA_PIN 17


// Disabled
#define ENABLED 1
#define DISABLED 0
#define CONTROL_STEPPER ENABLED
#define CONTROL_MUSIC ENABLED
#define CONTROL_DISPLAY DISABLED
#define CONTROL_BUTTON ENABLED
#define CONTROL_LIGHTS DISABLED

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#endif