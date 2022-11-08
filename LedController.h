#ifndef LedController_h
#define LedController_h

#include "Constants.h"
#include <Adafruit_NeoPixel.h>

// How many seconds to show each temperature before switching
#define DISPLAYTIME 20

// How many seconds to show black between switches
#define BLACKTIME 3

#define FRAMES_PER_SECOND 45
#define LIGHT_SENSOR_PIN 99999



#define DO_NOTHING -1
#define PATTERN_SINGLE 0
#define PATTERN_GROWING 1
#define PATTERN_BOUNCING 2
#define PATTERN_WHIPE 3
#define PATTERN_FULL 4
#define PATTERN_RAINBOW 5
#define PATTERN_THEATHER 6
#define PATTERN_RGB_LOOP 7
#define PATTERN_SLEEPING 8
#define PATTERN_SHUTDOWN 9


#define SEQUENCE_PATTERNS 8
#define COLOR_PATTERNS 6

#define BLACK_COLOR strip.Color(0, 0, 0)
#define WHITE_COLOR strip.Color(255, 255, 255)

class LedController {
private:
  // Timing variables
  unsigned long timeLastCheckOnLedTriggers;
  unsigned long timeLastSequenceUpdate;
  unsigned long timeLastDebug = 0;
  unsigned long timeLastSleepCheck;
  unsigned long timeSleepHasStarted;

  int deltaTimeOnLedChange = 100;
  int deltaTimeWhenSequence = LIGHTS_DELAY;
  int deltaTimeOnSleepBreath = DELTA_SLEEPING_SLOW;

  uint32_t colorPattern[COLOR_PATTERNS];
  uint32_t currentColorPattern = 0;

  uint8_t previousPattern = 0;
  uint8_t pattern = 0;
  uint8_t ledInSequence = 0;
  uint8_t rgbSequence = 0;

  bool sequenceUp = true;
  bool sequenceTriggered = false;
  bool fadeIn = true;
  bool shrink = false;
  bool isOff = false;
  bool sleepMode = false;

  int rgb_intensity = 0;

    Adafruit_NeoPixel strip;

public:
  LedController() {
    strip = Adafruit_NeoPixel(WHEEL_NUM_LEDS, WHEEL_LIGHTS_DATA_PIN, NEO_GRB + NEO_KHZ800);
  };
  void init();
  void begin();
  void operate();
  void ledSequence();
  void nextSequence();
  byte* wheel(byte wheelPos);
  void singleLedSequence();
  void growingLedSequence();
  void rgbLoop();
  void rainbow();
  void sleeping(bool isSleeping);
  void sleepingSequence();
  void shutdownSequence();
  void setAll(byte red, byte green, byte blue) {
    for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
  };
};


//ColorWhipe
//bouncingBall
//FullColor
//singleSequence
//growingSequence
//rainbow
//theather(slow)
//RGBLoop

void LedController::nextSequence() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController nextSequence");
  sequenceTriggered = true;
  ledInSequence = 0;
  fadeIn = true;
  sequenceUp = true;
}

void LedController::sleeping(bool isSleeping) {
  sleepMode = isSleeping;
  if (sleepMode) {
    previousPattern = pattern;
    pattern = PATTERN_SLEEPING;
    rgb_intensity = MAX_VALUE_LIGHT_FOR_SLEEP;
    timeSleepHasStarted = millis();
    if (CURRENT_MODE == DEBUG_MODE) {
      Serial.print("LedController going to sleep with sleepTime:");
      Serial.println(timeSleepHasStarted);
    }
    fadeIn = false;
  }
}

void LedController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController begin");

  delay(SETUP_DELAY);  // 3 second delay for recovery

  strip.setBrightness(BRIGHTNESS);
  for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
    strip.setPixelColor(i, BLACK_COLOR);
  }
  strip.show();
}

void LedController::init() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController init");
  unsigned long time = millis();
  timeLastSequenceUpdate = time;
  timeLastSleepCheck = time;
  colorPattern[0] = strip.Color(255, 0, 0);    //RED
  colorPattern[1] = strip.Color(255, 0, 255);  //PINK
  colorPattern[2] = strip.Color(0, 0, 255);    //BLUE
  colorPattern[3] = strip.Color(0, 255, 255);  //CYAN
  colorPattern[4] = strip.Color(0, 255, 0);    //GREEN
  colorPattern[5] = strip.Color(255, 255, 0);  //YELLOW
  pattern = DO_NOTHING;
}

void LedController::operate() {
  if (CONTROL_LIGHTS == DISABLED) return;
  unsigned long time = millis();
  if (time - timeLastCheckOnLedTriggers > deltaTimeOnLedChange) {
    if (sequenceTriggered && !sleepMode) {
      pattern++;
      timeLastCheckOnLedTriggers = time;
      pattern = pattern % SEQUENCE_PATTERNS;
      if (CURRENT_MODE == DEBUG_MODE) {
        Serial.print("LedController changing sequence:");
        Serial.println(pattern);
      }
      sequenceTriggered = false;
    }
  }

  if (CURRENT_MODE == DEBUG_MODE) {
    if (time - timeLastDebug > 10000) {
      Serial.println("LedController operate");
      timeLastDebug = time;
    }
  }

  ledSequence();
}

void LedController::ledSequence() {

  switch (pattern) {
    case DO_NOTHING:
      break;
    case PATTERN_SINGLE:
      singleLedSequence();
      break;
    case PATTERN_GROWING:
      growingLedSequence();
      break;
    case PATTERN_RGB_LOOP:
      rgbLoop();
      break;
    case PATTERN_RAINBOW:
      rainbow();
      break;
    case PATTERN_SLEEPING:
      sleepingSequence();
      break;
    case PATTERN_SHUTDOWN:
      shutdownSequence();
      break;
  }
}

void LedController::sleepingSequence() {
  unsigned long time = millis();
  if (time - timeLastSleepCheck > deltaTimeOnSleepBreath) {
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor(i, BLACK_COLOR);
    }

    for (int i = 4; i < WHEEL_NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, rgb_intensity));
    }

    if (fadeIn) {
      rgb_intensity++;
      if (rgb_intensity == (MIN_VALUE_LIGHT_FOR_SLEEP + SLEEP_THRESHOLD)) {
        deltaTimeOnSleepBreath = DELTA_SLEEPING_SLOW;
      }
      if (rgb_intensity == MAX_VALUE_LIGHT_FOR_SLEEP) {
        fadeIn = false;
      }
    } else {
      rgb_intensity--;
      if (rgb_intensity == (MIN_VALUE_LIGHT_FOR_SLEEP + SLEEP_THRESHOLD)) {
        deltaTimeOnSleepBreath = DELTA_SLEEPING_FAST;
      }
      if (rgb_intensity == MIN_VALUE_LIGHT_FOR_SLEEP) {
        fadeIn = true;
      }
    }
    timeLastSleepCheck = time;
    if (time - timeSleepHasStarted > (SLEEP_SHUTDOWN - 3000)) {
      pattern = PATTERN_SHUTDOWN;
    }
    strip.show();
  }
}

void LedController::shutdownSequence() {
  unsigned long time = millis();
  if (time - timeLastSleepCheck > deltaTimeOnSleepBreath) {
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor(i, BLACK_COLOR);
    }

    for (int i = 4; i < WHEEL_NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, rgb_intensity));
    }

    rgb_intensity--;
    if (rgb_intensity == (MIN_VALUE_LIGHT_FOR_SLEEP + SLEEP_THRESHOLD)) {
      deltaTimeOnSleepBreath = DELTA_SLEEPING_FAST;
    }
    if (rgb_intensity == 0) {
      pattern = DO_NOTHING;
    }

    timeLastSleepCheck = time;
    strip.show();
  }
}

void LedController::singleLedSequence() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeWhenSequence) {
    for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
      strip.setPixelColor(i, BLACK_COLOR);
    }
    strip.setPixelColor(ledInSequence, colorPattern[currentColorPattern]);
    if (sequenceUp) {
      ledInSequence++;
      if (ledInSequence == WHEEL_NUM_LEDS) {
        ledInSequence--;
        sequenceUp = false;
      }
    } else {
      ledInSequence--;
      if (ledInSequence == -1) {
        ledInSequence++;
        sequenceUp = true;
        currentColorPattern++;
        currentColorPattern %= COLOR_PATTERNS;
      }
    }

    timeLastSequenceUpdate = time;
    strip.show();
  }
}

void LedController::growingLedSequence() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeWhenSequence) {
    if (sequenceUp) {
      if (!shrink) {
        for (int i = 0; i <= ledInSequence; i++) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
        }
        for (int i = ledInSequence + 1; i < WHEEL_NUM_LEDS; i++) {
          strip.setPixelColor(i, BLACK_COLOR);
        }
        ledInSequence++;
        if (ledInSequence == WHEEL_NUM_LEDS) {
          shrink = true;
          ledInSequence = 0;
        }
      } else {
        for (int i = ledInSequence; i < WHEEL_NUM_LEDS; i++) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
        }
        for (int i = 0; i < ledInSequence; i++) {
          strip.setPixelColor(i, BLACK_COLOR);
        }
        ledInSequence++;
        if (ledInSequence == WHEEL_NUM_LEDS) {
          shrink = false;
          sequenceUp = false;
          ledInSequence--;
        }
      }

    } else {
      if (!shrink) {
        for (int i = WHEEL_NUM_LEDS - 1; i >= ledInSequence; i--) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
        }
        for (int i = ledInSequence - 1; i >= 0; i--) {
          strip.setPixelColor(i, BLACK_COLOR);
        }
        ledInSequence--;
        if (ledInSequence == 0) {
          shrink = true;
          ledInSequence = WHEEL_NUM_LEDS - 1;
        }
      } else {
        for (int i = ledInSequence; i >= 0; i--) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
        }
        for (int i = WHEEL_NUM_LEDS - 1; i > ledInSequence; i--) {
          strip.setPixelColor(i, BLACK_COLOR);
        }
        ledInSequence--;
        if (ledInSequence == -1) {
          shrink = false;
          sequenceUp = true;
          ledInSequence++;
          currentColorPattern++;
          currentColorPattern %= COLOR_PATTERNS;
        }
      }
    }

    timeLastSequenceUpdate = time;
    strip.show();
  }
}

void LedController::rgbLoop() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > DELTA_RGB_LOOP) {

    // Fade IN
    if (fadeIn) {
      switch (rgbSequence) {
        case 0: setAll(rgb_intensity, 0, 0); break;
        case 1: setAll(0, rgb_intensity, 0); break;
        case 2: setAll(0, 0, rgb_intensity); break;
      }

      rgb_intensity++;
      if (rgb_intensity == 256) {
        fadeIn = false;
        rgb_intensity--;
      }
    } else {

      // Fade OUT
      //for(int k = 255; k >= 0; k--) {
      switch (rgbSequence) {
        case 0: setAll(rgb_intensity, 0, 0); break;
        case 1: setAll(0, rgb_intensity, 0); break;
        case 2: setAll(0, 0, rgb_intensity); break;
      }
      rgb_intensity--;
      if (rgb_intensity == 0) {
        fadeIn = true;
        rgbSequence++;
        rgbSequence = rgbSequence % 3;
      }
    }
    timeLastSequenceUpdate = time;
  }
}


void LedController::rainbow() {
  byte* c;
  uint16_t i, j;
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > DELTA_TIME_RAINBOW) {
    for (i = 0; i < WHEEL_NUM_LEDS; i++) {
      c = wheel(((i * 256 / WHEEL_NUM_LEDS) + ledInSequence) & 255);
      strip.setPixelColor(i, *c, *(c + 1), *(c + 2));
    }
    strip.show();
    ledInSequence++;
    if (ledInSequence == 256 * 5) {
      ledInSequence = 0;
    }
    timeLastSequenceUpdate = time;
  }
}

// used by rainbowCycle and theaterChaseRainbow
byte* LedController::wheel(byte wheelPos) {
  static byte c[3];

  if (wheelPos < 85) {
    c[0] = wheelPos * 3;
    c[1] = 255 - wheelPos * 3;
    c[2] = 0;
  } else if (wheelPos < 170) {
    wheelPos -= 85;
    c[0] = 255 - wheelPos * 3;
    c[1] = 0;
    c[2] = wheelPos * 3;
  } else {
    wheelPos -= 170;
    c[0] = 0;
    c[1] = wheelPos * 3;
    c[2] = 255 - wheelPos * 3;
  }

  return c;
}

#endif