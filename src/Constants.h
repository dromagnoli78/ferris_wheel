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

/**
 * Operational modes
 */
#define DEBUG_MODE 1
#define CLEAN_MODE -1
#define DETAILS_MODE 2
#define CURRENT_MODE DEBUG_MODE +1

#define START_MODE 100
#define INITMODE 101
#define TEST_MODE 103
#define DIAGNOSE_MODE 104
#define WORKING_MODE 105
#define SETTING_MODE 106
#define PRE_SLEEPING_MODE 199
#define SLEEPING_MODE 200
#define OFF_MODE 900

#define DO_NOTHING 99

#define SLEEP_TRACK 5

// Pins
#define VOLUME_PIN 15
#define MP3_RX_PIN 16
#define MP3_TX_PIN 17
#define SENSOR_PIN 4
#define SDApin 21
#define SCLpin 22

// Buttons Signals
#define EXPANDER_MASK 0b00000000
#define BUTTON_MUSIC 35
#define BUTTON_MUTE 35
#define BUTTON_LED_SEQUENCE 35
#define BUTTON_STEPPER 35
#define BUTTON_SLEEPING 18

#define BUTTON_SETTINGS 34
#define BUTTON_ARROW_LEFT 35
#define BUTTON_ARROW_RIGHT 35
#define BUTTON_ARROW_UP 35
#define BUTTON_ARROW_DOWN 35
#define BUTTONS_INTERRUPT_PIN 35

#define BUTTON_POWER_ON 23

#define BUTTON_CONTROLS_DELTA_TIME 200
#define BUTTON_PRESSED HIGH
#define BUTTON_LONG_PRESSED_DELTA_TIME 1000

// Buttons positions
#define SETTINGS_INDEX 9
#define SLEEP_INDEX 8
#define MUTE_INDEX 7
#define MUSIC_INDEX 6
#define STEPPER_INDEX 5
#define LIGHT_INDEX 4
#define LEFT_INDEX 3
#define DOWN_INDEX 2
#define RIGHT_INDEX 1
#define UP_INDEX 0

// Stepper
#define STEPPER_INCREMENT 1
#define STEPPER_IN_1 26
#define STEPPER_IN_2 25
#define STEPPER_IN_3 33
#define STEPPER_IN_4 32
#define STEPS_PER_REVOLUTION 4096
#define STEPPER_SPEEDS 3
#define STEPPER_SPEED_SLOW 80
#define STEPPER_SPEED_MED 35
#define STEPPER_SPEED_FAST 12
#define STEPPER_INIT_SPEED STEPPER_SPEED_MED

// DELAYS
#define SETUP_DELAY 800
#define DELAY_SOFT_START_SLEEP 5000
#define DELAY_AFTER_SONG 7000
#define DELAY_AFTER_FOLDER 1500
#define DELAY_AFTER_MESSAGE 4000
#define DELAY_AFTER_HELLO 3 // SETTINGS
#define DELTA_TIME_SENSOR_CHECK 200
#define DELTA_BLINK_TIME 100

//#define SLEEP_SHUTDOWN 600000
#define SLEEP_SHUTDOWN_TIME 120 //SETTINGS
#define SLEEP_SHUTDOWN_DISPLAY_TIME 60 //SETTINGS

/////////////////////// Leds
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Music
#define DELTA_MUSIC_TIME 100
#define DELTA_VOLUME_TIME 30

// Wheel
#define WHEEL_NUM_LEDS 8
#define WHEEL_LIGHTS_DATA_PIN 14
#define PATTERN_PERIOD 12
#define ENABLE_POWER_LED 19
#define POWER_LED_KEEP_ALIVE_INTERVAL 1000
#define DELTA_TIME_SLEEP_BREATH 7000
#define SPINNING_ACCELERATION 0.002

// LED SEQUENCE TIMING
#define LIGHTS_DELAY 180
#define LIGHTS_DELAY_MIN 30 // SETTINGS
#define LIGHTS_DELAY_MAX 2000
#define LIGHTS_DELAY_INCREMENT 50
#define DELTA_RGB_LOOP 10
#define DELTA_TIME_RAINBOW 40
#define DELTA_TIME_CIRCLE 100
#define DELTA_SLEEPING_SLOW 80
#define DELTA_SLEEPING_FAST 20
#define MAX_VALUE_LIGHT_FOR_SLEEP 50 // SETTINGS
#define MIN_VALUE_LIGHT_FOR_SLEEP 12 // SETTINGS
#define SLEEP_THRESHOLD 5
#define WHEEL_LOOPS 74

//#define LED_BRIGHTNESS 96
#define LED_INITIAL_BRIGHTNESS 80 // SETTINGS
#define LED_MIN_BRIGHTNESS 15  // SETTINGS
#define LED_MAX_BRIGHTNESS 120 // SETTINGS
#define LED_DELTA 7 // SETTINGS
#define EQUALIZER DFPLAYER_EQ_NORMAL

// Console
#define EYES_LED_DATA_PIN 27
#define CONSOLE_LIGHTS_CONTROLLER_DATA_PIN 13
#define DELTA_TIME_CONSOLE_UPDATES 100
#define CONSOLE_LIGHTS_BRIGHTNESS 100
#define DELTA_TIME_BLINK_SETTINGS 500
#define DELTA_TIME_BLINK_SAVING 200

#define DURATION_BLINK_SETTINGS -1
#define DURATION_BLINK_SAVING 1000

#define SLEEP_LED_BRIGHTNESS 10

#define LED_MUTE_COLOR strip.ColorHSV(0, 255, CONSOLE_LIGHTS_BRIGHTNESS)
#define LED_MUSIC_COLOR strip.ColorHSV(330 * 182.04, 255, CONSOLE_LIGHTS_BRIGHTNESS)
#define LED_SLEEP_COLOR strip.ColorHSV(242 * 182.04, 255, SLEEP_LED_BRIGHTNESS)
#define LED_LIGHTS_COLOR strip.ColorHSV(90 * 182.04, 255, CONSOLE_LIGHTS_BRIGHTNESS)
#define LED_STEPPER_COLOR strip.ColorHSV(180 * 182.04, 255, CONSOLE_LIGHTS_BRIGHTNESS)

// Display Controls Modes
#define TIME_MODE 0
#define MUSIC_MODE 1
#define PRE_SLEEP_MODE 2
#define SLEEP_MODE 3
#define EMPTY_MODE 4
#define HELLO_MODE 5
#define FUNCTION_MODE 6
#define SEQUENCE_MODE 7
#define NAMES_MODE 8
#define FOLDER_MODE 9
#define SETTINGS_MODE 10
#define REMOTE_MODE 88
#endif