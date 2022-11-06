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
#define LED_MUTE_COLOR strip.ColorHSV(0, 255, 255)
#define LED_MUSIC_COLOR strip.ColorHSV(330 * 182.04, 255, 255)
#define LED_SLEEP_COLOR strip.ColorHSV(240 * 182.04, 255, 255)
#define LED_LIGHTS_COLOR strip.ColorHSV(54 * 182.04, 255, 255)
#define LED_STEPPER_COLOR strip.ColorHSV(124 * 182.04, 255, 255)
#define BRIGHTNESS 96

class ConsoleLightsController {
private:
  //CRGB consoleLeds[CONSOLE_TOTAL_LEDS];
  //CRGBArray<CONSOLE_TOTAL_LEDS> consoleLedsArr;
  Adafruit_NeoPixel strip;

public:
  ConsoleLightsController(){};
  void init();
  void begin();
  void operate();

  void turnOff(int ledPosition) {
    if (CONTROL_CONSOLE_LIGHTS == DISABLED) return;
    strip.setPixelColor(ledPosition, BLACK_COLOR);
  };

  /** 
  * Set the specified led element with the provided color.
  * if syncSettings set the SETTINGS led to the same color.
  */
  void setLed(int ledPosition, uint32_t color, bool syncSettings) {
    if (CONTROL_CONSOLE_LIGHTS == DISABLED) return;
    strip.setPixelColor(ledPosition, color);
    if (syncSettings) {
      strip.setPixelColor(LED_SETTINGS, color);
    }
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
}

void ConsoleLightsController::operate() {

  //FastLED.show();
  //delay(1000);
}

void ConsoleLightsController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ConsoleLightsController begin");
  delay(SETUP_DELAY);  // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE, CONSOLE_LIGHTS_CONTROLLER_DATA_PIN, COLOR_ORDER>(this->consoleLeds, CONSOLE_TOTAL_LEDS).setCorrection(TypicalLEDStrip);
  strip = Adafruit_NeoPixel(CONSOLE_TOTAL_LEDS, CONSOLE_LIGHTS_CONTROLLER_DATA_PIN, NEO_GRB + NEO_KHZ800);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);
  strip.setBrightness(BRIGHTNESS);
}

#endif