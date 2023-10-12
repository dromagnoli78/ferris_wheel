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

const char* settingsNames[]={
  "Settings",
  "Sleep",
  "Lights",
  "Stepper",
  "Music",
  "Mute", 
};


//#define LED_EYE_1 6
//#define LED_EYE_2 7

#define CONSOLE_TOTAL_LEDS 7
//#define LED_EYES_COLOR strip.ColorHSV(40 * 182.04, 255, CONSOLE_LIGHTS_INTENSITY)

class ConsoleLightsController {
private:
  // timing variables
  unsigned long timeOfLastCheck = 0;
  unsigned long timeOfBlinking = 0;
  unsigned long timeOfLastDebug = 0;
  bool blinkingSettings = false;
  int brightness = LED_BRIGHTNESS;
  bool leds[CONSOLE_TOTAL_LEDS];
  uint32_t colors[CONSOLE_TOTAL_LEDS];
  uint8_t settingsLed = 0; // Which led should be replicated by the settings
  bool sleepMode = false;
  Adafruit_NeoPixel strip;
  
public:
  MusicController* musicController;
  ModeController* modeController;
  ConsoleLightsController(ModeController* pModeController, MusicController* pMusicController) {
    musicController = pMusicController;
    modeController = pModeController;
    strip = Adafruit_NeoPixel(CONSOLE_TOTAL_LEDS, CONSOLE_LIGHTS_CONTROLLER_DATA_PIN, NEO_GRB + NEO_KHZ800);
  };
  void init();
  void begin();
  void operate();
  void adjustVolume(int);
  
  uint8_t getSettings() {
    return settingsLed;
  }

  const char* getSettingsName() {
    return settingsNames[settingsLed];
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
    brightness = value+10;
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

  void lights(bool isOn) {
    if (isOn) {
      setLed(LED_LIGHTS, true);
    } else {
      turnOff(LED_LIGHTS);
    }
  }

  void music(bool isOn) {
    if (isOn) {
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

  // Each led has its own specific color
  colors[LED_MUSIC] = LED_MUSIC_COLOR;
  colors[LED_MUTE] = LED_MUTE_COLOR;
  colors[LED_STEPPER] = LED_STEPPER_COLOR;
  colors[LED_LIGHTS] = LED_LIGHTS_COLOR;
  colors[LED_SLEEP] = LED_SLEEP_COLOR;
  colors[LED_SETTINGS] = BLACK_COLOR;
  settingsLed = LED_SETTINGS;
}

void ConsoleLightsController::operate() {
  if (CONTROL_CONSOLE_LIGHTS == CTL_DISABLED) return;
  unsigned long time = millis();
  uint32_t color = 0;
  if (time - timeOfLastCheck > DELTA_TIME_CONSOLE_UPDATES) {
    if (modeController->isSettings()) {
      if (time - timeOfBlinking > DELTA_TIME_BLINK) {
        blinkingSettings = !blinkingSettings;
        colors[LED_SETTINGS] = blinkingSettings ? BLUE_COLOR : YELLOW_COLOR;
        timeOfBlinking = time;
      }

    } 
    if (modeController->isSleeping()) {
      // Implement transition
      for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
        strip.setPixelColor(i, BLACK_COLOR);
      }
      strip.setPixelColor(LED_SLEEP, LED_SLEEP_COLOR);
    } else {
      for (int i = 0; i < CONSOLE_TOTAL_LEDS; i++) {
        color = leds[i] ? colors[i] : BLACK_COLOR;
        strip.setPixelColor(i, color);
      }
      strip.setPixelColor(LED_SETTINGS, colors[settingsLed]);
      int volume = musicController->getAnalogVolume();
      adjustVolume(volume);
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