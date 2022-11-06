#ifndef ConsoleLightsController_h
#define ConsoleLightsController_h

#include "Constants.h"
#include <Adafruit_NeoPixel.h>

#define LED_MUSIC 0
#define LED_MUTE 1
#define LED_STEPPER 2
#define LED_LIGHTS 3
#define LED_SLEEP 4
#define LED_SETTINGS 5
#define LED_EYE_1 6
#define LED_EYE_2 7

#define CONSOLE_TOTAL_LEDS 8
#define LED_MUTE_COLOR strip.ColorHSV(0, 255, 100)
#define LED_MUSIC_COLOR strip.ColorHSV(330 * 182.04, 255, 100)
#define LED_SLEEP_COLOR strip.ColorHSV(240 * 182.04, 255, 30)
#define LED_LIGHTS_COLOR strip.ColorHSV(54 * 182.04, 255, 100)
#define LED_STEPPER_COLOR strip.ColorHSV(124 * 182.04, 255, 100)
#define LED_EYES_COLOR strip.ColorHSV(40 * 182.04, 255, 100)
#define BRIGHTNESS 96

class ConsoleLightsController {
private:
  //CRGB consoleLeds[CONSOLE_TOTAL_LEDS];
  //CRGBArray<CONSOLE_TOTAL_LEDS> consoleLedsArr;
  Adafruit_NeoPixel strip;
  bool leds[CONSOLE_TOTAL_LEDS];
  uint32_t colors[CONSOLE_TOTAL_LEDS];
  uint8_t settingLed=0;
  unsigned long lastTimeCheck = 0;
  bool sleepMode = false;
public:
  ConsoleLightsController(){
 strip = Adafruit_NeoPixel(CONSOLE_TOTAL_LEDS, CONSOLE_LIGHTS_CONTROLLER_DATA_PIN, NEO_GRB + NEO_KHZ800);
  };
  void init();
  void begin();
  void operate();

  void turnOff(int ledPosition) {
    if (CONTROL_CONSOLE_LIGHTS == DISABLED) return;
    leds[ledPosition]=false;
    //strip.setPixelColor(ledPosition, BLACK_COLOR);
    //strip.show();
  };

  /** 
  * Set the specified led element with the provided color.
  * if syncSettings set the SETTINGS led to the same color.
  */
  void setLed(int ledPosition, uint32_t color, bool syncSettings) {
    if (CONTROL_CONSOLE_LIGHTS == DISABLED) return;
    //strip.setPixelColor(ledPosition, color);
    leds[ledPosition] = true;
    if (syncSettings) {
      settingLed = ledPosition;
      //strip.setPixelColor(LED_SETTINGS, color);
    }
    //strip.show();
  };

  void adjustBrightness(uint8_t value) {
    strip.setBrightness(value);
  };

  void mute(bool isMuted) {
    if (isMuted) {
      setLed(LED_MUTE, LED_MUTE_COLOR, false);
      turnOff(LED_MUSIC);
    } else {
      turnOff(LED_MUTE);
    }
  };

  void sleeping(bool isSleeping) {
    sleepMode = isSleeping;
    if (isSleeping) {
      setLed(LED_SLEEP, LED_SLEEP_COLOR, false);
    } else {
      turnOff(LED_SLEEP);
    }
  };

  void stepper(bool isStepping) {
    if (isStepping) {
      setLed(LED_STEPPER, LED_STEPPER_COLOR, true);
    } else {
      turnOff(LED_STEPPER);
    }
  };

  void lights(bool onOff) {
    if (onOff) {
      setLed(LED_LIGHTS, LED_LIGHTS_COLOR, true);
    } else {
      turnOff(LED_LIGHTS);
    }
  };
  void music() {
    setLed(LED_MUSIC, LED_MUSIC_COLOR, true);
  }
};

void ConsoleLightsController::init() {
    if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ConsoleLightsController init");
    for (int i=0; i<CONSOLE_TOTAL_LEDS;i++) {
      leds[i]=false;
    }
    leds[LED_EYE_1]= true;
    leds[LED_EYE_2]= true;
    colors[LED_MUSIC] = LED_MUSIC_COLOR;
    colors[LED_MUTE] = LED_MUTE_COLOR;
    colors[LED_STEPPER] = LED_STEPPER_COLOR;
    colors[LED_LIGHTS] = LED_LIGHTS_COLOR;
    colors[LED_SLEEP] = LED_SLEEP_COLOR;
    colors[LED_SETTINGS] = BLACK_COLOR;
    colors[LED_EYE_1] = LED_EYES_COLOR;
    colors[LED_EYE_2] = LED_EYES_COLOR;
    settingLed = LED_SETTINGS;

   
}

void ConsoleLightsController::operate() {
  unsigned long time = millis();
  uint32_t color = 0;
  if (time - lastTimeCheck > DELTA_TIME_CONSOLE_UPDATES) {
    if (CURRENT_MODE == DEBUG_MODE) {
        Serial.println("ConsoleLightsController operate");
    }
    if (!sleepMode) {
      for (int i=0; i<CONSOLE_TOTAL_LEDS; i++) {
        color = leds[i] ? colors[i] : BLACK_COLOR;
        strip.setPixelColor(i, color);
      }
      strip.setPixelColor(LED_SETTINGS, colors[settingLed]);
    } else {
      // Implement transition
      for (int i=0; i<CONSOLE_TOTAL_LEDS; i++) {
        strip.setPixelColor(i, BLACK_COLOR);
      }
      strip.setPixelColor(LED_SLEEP, LED_SLEEP_COLOR);
    }
    strip.show();
    lastTimeCheck = time;
  }

}

void ConsoleLightsController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ConsoleLightsController begin");
 
  delay(SETUP_DELAY);  // 3 second delay for recovery
  strip.setBrightness(BRIGHTNESS);
  for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
    strip.setPixelColor(i, BLACK_COLOR);
  }
  strip.show();
}

#endif