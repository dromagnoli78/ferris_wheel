#ifndef ConsoleLightsController_h
#define ConsoleLightsController_h

#include "Constants.h"
#include <Adafruit_NeoPixel.h>

#define LED_VOLUME 6
#define LED_MUTE 5
#define LED_MUSIC 4
#define LED_STEPPER 3
#define LED_LIGHTS 2
#define LED_SLEEP 1
#define LED_SETTINGS 0

//#define LED_EYE_1 6
//#define LED_EYE_2 7

#define CONSOLE_TOTAL_LEDS 7
#define LED_MUTE_COLOR strip.ColorHSV(0, 255, CONSOLE_LIGHTS_INTENSITY)
#define LED_MUSIC_COLOR strip.ColorHSV(330 * 182.04, 255, CONSOLE_LIGHTS_INTENSITY)
#define LED_SLEEP_COLOR strip.ColorHSV(240 * 182.04, 255, SLEEP_LED_INTENTISY)
#define LED_LIGHTS_COLOR strip.ColorHSV(54 * 182.04, 255, CONSOLE_LIGHTS_INTENSITY)
#define LED_STEPPER_COLOR strip.ColorHSV(124 * 182.04, 255, CONSOLE_LIGHTS_INTENSITY)
//#define LED_EYES_COLOR strip.ColorHSV(40 * 182.04, 255, CONSOLE_LIGHTS_INTENSITY)

class ConsoleLightsController {
private:
  // timing variables
  unsigned long timeOfLastCheck = 0;
  unsigned long timeOfLastDebug = 0;
  bool leds[CONSOLE_TOTAL_LEDS];
  uint32_t colors[CONSOLE_TOTAL_LEDS];
  uint8_t settingsLed = 0; // Which led should be replicated by the settings
  bool sleepMode = false;
  Adafruit_NeoPixel strip;
  MusicController* musicController;
public:
  ConsoleLightsController(MusicController* pMusicController) {
    musicController = pMusicController;
    strip = Adafruit_NeoPixel(CONSOLE_TOTAL_LEDS, CONSOLE_LIGHTS_CONTROLLER_DATA_PIN, NEO_GRB + NEO_KHZ800);
  };
  void init();
  void begin();
  void operate();
  void adjustVolume(int);
  
  uint8_t getSettings() {
    return settingsLed;
  }
  
  void setSettings(uint8_t settings) {
    settingsLed = settings;
  }

  void turnOff(int ledPosition) {
    if (CONTROL_CONSOLE_LIGHTS == CTL_DISABLED) return;
    leds[ledPosition] = false;
  };

  bool isItOn(int ledPosition) {
    return leds[ledPosition];
  };

  /** 
  * Set the specified led element.
  * if syncSettings, set the SETTINGS led to that led position.
  */
  void setLed(int ledPosition, bool syncSettings) {
    if (CONTROL_CONSOLE_LIGHTS == CTL_DISABLED) return;
    leds[ledPosition] = true;
    if (syncSettings) {
      dbg("Setting led", (unsigned long) ledPosition);
      settingsLed = ledPosition;
    }
  };

  void shutdown() {
    for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  };

  void adjustBrightness(uint8_t value) {
    strip.setBrightness(value);
  };

  void mute(bool isMuted) {
    if (isMuted) {
      setLed(LED_MUTE, false);
      //turnOff(LED_MUSIC);
    } else {
      turnOff(LED_MUTE);
    }
  };

  void sleeping(bool isSleeping) {
    sleepMode = isSleeping;
    if (isSleeping) {
      setLed(LED_SLEEP, false);
    } else {
      turnOff(LED_SLEEP);
    }
  }

  void stepper(bool isStepping) {
    if (isStepping) {
      setLed(LED_STEPPER, true);
      dbg("set Stepper");
    } else {
      turnOff(LED_STEPPER);
    }
  }

  void lights(bool onOff) {
    if (onOff) {
      setLed(LED_LIGHTS, true);
    } else {
      turnOff(LED_LIGHTS);
    }
  }

  void music(bool onOff) {
    if (onOff) {
      setLed(LED_MUSIC, true);
    } else {
      turnOff(LED_MUSIC);
    }
  }
};

void ConsoleLightsController::init() {
  if (CONTROL_CONSOLE_LIGHTS == CTL_DISABLED) return;
  dbg("ConsoleLightsController init");
  for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
    leds[i] = false;
  }
  //leds[LED_EYE_1] = true;
  //leds[LED_EYE_2] = true;

  // Each led has its own specific color
  colors[LED_MUSIC] = LED_MUSIC_COLOR;
  colors[LED_MUTE] = LED_MUTE_COLOR;
  colors[LED_STEPPER] = LED_STEPPER_COLOR;
  colors[LED_LIGHTS] = LED_LIGHTS_COLOR;
  colors[LED_SLEEP] = LED_SLEEP_COLOR;
  colors[LED_SETTINGS] = BLACK_COLOR;
  //colors[LED_EYE_1] = LED_EYES_COLOR;
  //colors[LED_EYE_2] = LED_EYES_COLOR;
  settingsLed = LED_SETTINGS;
}

void ConsoleLightsController::operate() {
  if (CONTROL_CONSOLE_LIGHTS == CTL_DISABLED) return;
  unsigned long time = millis();
  uint32_t color = 0;
  if (time - timeOfLastCheck > DELTA_TIME_CONSOLE_UPDATES) {
    if (!sleepMode) {
      for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
        color = leds[i] ? colors[i] : BLACK_COLOR;
        strip.setPixelColor(i, color);
      }
      strip.setPixelColor(LED_SETTINGS, colors[settingsLed]);
      int volume = musicController->getAnalogVolume();
      adjustVolume(volume);
    } else {
      // Implement transition
      for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
        strip.setPixelColor(i, BLACK_COLOR);
      }
      strip.setPixelColor(LED_SLEEP, LED_SLEEP_COLOR);
    }
    strip.show();
    timeOfLastCheck = time;
  }
  if (CURRENT_MODE > DEBUG_MODE) {
    if (time - timeOfLastDebug > 30000) {
      Serial.println("ConsoleLightsController operate");
      timeOfLastDebug = time;
    }
  }
}

void ConsoleLightsController::adjustVolume(int volume){
  uint16_t h = map(volume, 0, 4095, 180, 359) * 182.04;
  uint8_t intensity = map (volume, 0, 4095, 8, 100);
  strip.setPixelColor(LED_VOLUME, strip.ColorHSV(h, 255, intensity));
}

void ConsoleLightsController::begin() {
  if (CONTROL_CONSOLE_LIGHTS == CTL_DISABLED) return;
  dbg("ConsoleLightsController begin");
  strip.begin();
  delay(SETUP_DELAY);  // 3 second delay for recovery
  strip.setBrightness(LED_BRIGHTNESS);
  for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
    strip.setPixelColor(i, BLACK_COLOR);
  }
  strip.show();
}

#endif