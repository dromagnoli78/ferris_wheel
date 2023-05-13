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

#define DO_NOTHING 99
#define REQUEST_OFF 88
#define REQUEST_ON 77
#define PATTERN_SINGLE 0
#define PATTERN_WHEEL PATTERN_SINGLE + 1
#define PATTERN_GROWING PATTERN_WHEEL + 1
#define PATTERN_BOUNCING PATTERN_GROWING +1 
#define PATTERN_TAILS PATTERN_BOUNCING +1 
//#define PATTERN_WHIPE 5
//#define PATTERN_FULL 6
#define PATTERN_RAINBOW PATTERN_TAILS +1
//#define PATTERN_THEATHER 8
#define PATTERN_RGB_LOOP PATTERN_RAINBOW +1
#define PATTERN_SLEEPING 100
#define PATTERN_SHUTDOWN 101


#define SEQUENCE_PATTERNS 9
#define COLOR_PATTERNS 6

#define BLACK_COLOR strip.Color(0, 0, 0)
#define WHITE_COLOR strip.Color(255, 255, 255)
#define BLUE_COLOR strip.Color(0, 0, 255)


class LedController {
private:
  // Timing variables
  unsigned long timeLastCheckOnLedTriggers;
  unsigned long timeLastSequenceUpdate;
  unsigned long timeLastDebug = 0;
  unsigned long timeLastSleepCheck;
  unsigned long timeSleepHasStarted;
  unsigned long timeSleepStartEffect = 0;
  
  int currentDeltaTime = LIGHTS_DELAY;
  int deltaTimeOnLedChange = currentDeltaTime;
  int deltaTimeInLedSequences = currentDeltaTime;
  int deltaTimeOnSleepBreath = DELTA_SLEEPING_SLOW;

  uint32_t colorPattern[COLOR_PATTERNS];
  uint32_t currentColorPattern = 0;

  int previousPattern = 0;
  int pattern = 0;
  int ledInSequence = 0;
  int previousLedInSequence = 0;
  int rgbSequence = 0;

  bool sequenceUp = true;
  bool sequenceTriggered = false;
  bool fadeIn = true;
  bool shrink = false;
  bool off = false;
  bool requestOff = false;
  bool sleepMode = false;
  int rgb_intensity = 0;
  int wheelLoops = 0;
  bool speedUp = true;
  bool isSpinning = false;
  int hue = 0;
  float breathPhase = 0;
  int hueIncrement = 65536/(WHEEL_LOOPS * 4);
  Adafruit_NeoPixel strip;
  Adafruit_NeoPixel stripEye;

public:
  LedController() {
    strip = Adafruit_NeoPixel(WHEEL_NUM_LEDS, WHEEL_LIGHTS_DATA_PIN, NEO_GRB + NEO_KHZ800);
    stripEye = Adafruit_NeoPixel(2, EYES_LED_DATA_PIN, NEO_GRB + NEO_KHZ800);
  };
  void init();
  void begin();
  void operate();
  void ledSequence();
  void nextSequence();
  void speedChange(u_char c);
  byte* wheel(byte wheelPos);
  void singleLedSequence();
  void wheelSpinning();
  void growingLedSequence();
  void tails();
  void rgbLoop();
  void rainbow();
  void turnItOff();
  void turnItOn();
  void sleeping(bool isSleeping);
  void sleepingSequence();
  void sleepingSequence2();
  void shutdownSequence();
  bool isOff() {return off;}
  void resetAllLeds(Adafruit_NeoPixel* theStrip);
  byte getMin(byte red, byte green, byte blue);
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
//single led to the wheel perimeter and make it run on the perimeter for a while

void LedController::nextSequence() {
  dbg("LedController nextSequence");
  sequenceTriggered = true;
  off = false;
  requestOff = false;
  ledInSequence = 0;
  fadeIn = true;
  sequenceUp = true;
  isSpinning = false;
  wheelLoops = 0;
  speedUp = true;
  hue = 0;
  deltaTimeInLedSequences = currentDeltaTime;
}

void LedController::speedChange(u_char c) {
  switch (c) {
    case 'U':
      currentDeltaTime-=LIGHTS_DELAY_INCREMENT;
      dbg("Lights speed-up", currentDeltaTime);
      if (currentDeltaTime < LIGHTS_DELAY_MIN)
        currentDeltaTime = LIGHTS_DELAY_MIN;
      break;
    case 'D':
      currentDeltaTime+=LIGHTS_DELAY_INCREMENT;
      dbg("ligths speed-down", currentDeltaTime);
      if (currentDeltaTime > LIGHTS_DELAY_MAX)
        currentDeltaTime = LIGHTS_DELAY_MAX;
      break;
  }
  deltaTimeInLedSequences = currentDeltaTime;
  deltaTimeOnLedChange = currentDeltaTime;
  dbg("Final speed", deltaTimeInLedSequences);

}

void LedController::sleeping(bool isSleeping) {
  sleepMode = isSleeping;
  if (sleepMode) {
    previousPattern = pattern;
    pattern = PATTERN_SLEEPING;
    rgb_intensity = MAX_VALUE_LIGHT_FOR_SLEEP;
    timeSleepHasStarted = millis();
    dbg("LedController going to sleep with sleepTime:", timeSleepHasStarted);
    
    fadeIn = false;
  }
}

void LedController::begin() {
  if (CONTROL_LIGHTS == CTL_DISABLED) return;
  dbg("LedController begin");
  pinMode(ENABLE_POWER_LED, OUTPUT);
  delay(SETUP_DELAY);  // 3 second delay for recovery

  strip.setBrightness(LED_BRIGHTNESS);
  stripEye.setBrightness(LED_BRIGHTNESS-10);
  resetAllLeds(&strip);
  delay(50);
  strip.show();
  resetAllLeds(&stripEye);
  delay(50);
  stripEye.show();
  stripEye.setPixelColor(0, BLUE_COLOR);
  stripEye.setPixelColor(1, BLUE_COLOR);
  delay(50);
  stripEye.show();
}

void LedController::init() {
  if (CONTROL_LIGHTS == CTL_DISABLED) return;
  dbg("LedController init");
  off = true;
  digitalWrite(ENABLE_POWER_LED, HIGH);
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
  if (CONTROL_LIGHTS == CTL_DISABLED) return;
  unsigned long time = millis();
  if (time - timeLastCheckOnLedTriggers > deltaTimeOnLedChange) {
    if (sequenceTriggered && !sleepMode) {
      pattern++;
      timeLastCheckOnLedTriggers = time;
      pattern = pattern % SEQUENCE_PATTERNS;
      dbg("LedController changing sequence:", (unsigned long) pattern);
      sequenceTriggered = false;
    }
  }

  if (CURRENT_MODE > DEBUG_MODE) {
    if (time - timeLastDebug > 10000) {
      Serial.println("LedController operate");
      timeLastDebug = time;
    }
  }

  ledSequence();
}

void LedController::turnItOff() {
    requestOff = true;
    previousPattern = pattern;
    pattern = REQUEST_OFF;
    dbg("LEDController: Turned Off");
}

void LedController::turnItOn() {
    requestOff = false;
    pattern = previousPattern;
}

void LedController::ledSequence() {

  switch (pattern) {
    case REQUEST_OFF:
      setAll(0,0,0);
      off = true;
      requestOff = false;
      break;
    case REQUEST_ON:
      off = false;
      requestOff = false;
      break;
    case DO_NOTHING:
      break;
    case PATTERN_SINGLE:
      singleLedSequence();
      break;
    case PATTERN_WHEEL:
      wheelSpinning();
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
    case PATTERN_TAILS:
      tails();
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
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - timeSleepStartEffect;

  if (elapsed >= DELTA_TIME_SLEEP_BREATH) {
    timeSleepStartEffect = currentTime;
    breathPhase = 0.0;
  } else {
    breathPhase = 2 * PI * (float)elapsed / (float)DELTA_TIME_SLEEP_BREATH;
  }

  float brightness = 0.5 * (MAX_VALUE_LIGHT_FOR_SLEEP - MIN_VALUE_LIGHT_FOR_SLEEP) * (1.0 + sin(breathPhase)) + MIN_VALUE_LIGHT_FOR_SLEEP;
  if (currentTime - timeLastSleepCheck >= 20) {
    for (int i = 0; i < 4; i++) {
      strip.setPixelColor(i, BLACK_COLOR);
    }
    for (int i = 4; i < WHEEL_NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, (uint8_t)(brightness)));
    }
    strip.show();
    timeLastSleepCheck = currentTime;
  }
}

void LedController::sleepingSequence2() {
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
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    resetAllLeds(&strip);
    strip.setPixelColor(ledInSequence, colorPattern[currentColorPattern]);
    if (sequenceUp) {
      ledInSequence++;
      if (ledInSequence == WHEEL_NUM_LEDS) {
        ledInSequence--;
        sequenceUp = false;
      }
    } else {
      ledInSequence--;
      if (ledInSequence == 0) {
        sequenceUp = true;
        currentColorPattern++;
        currentColorPattern %= COLOR_PATTERNS;
      }
    }

    timeLastSequenceUpdate = time;
    strip.show();
  }
}

void LedController::wheelSpinning() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    if (sequenceUp) {
      if (CURRENT_MODE > DETAILS_MODE) {
        Serial.println("Sequence Up");
      }
      resetAllLeds(&strip);
      strip.setPixelColor(ledInSequence, colorPattern[currentColorPattern]);
      ledInSequence++;
      if (ledInSequence == WHEEL_NUM_LEDS) {
        ledInSequence = 4;
        sequenceUp = false;
        isSpinning = true;
      }
    } else if (isSpinning) {
      if (CURRENT_MODE > DETAILS_MODE) {
        Serial.println("Is Spinning");
      }
      resetAllLeds(&strip);
      int hsvC = hue;//map(wheelLoops, 0, WHEEL_LOOPS, 0, 65536);
      hue += hueIncrement;
      uint32_t colorHigh = strip.ColorHSV(hsvC, 255, 255);
      strip.setPixelColor(ledInSequence, colorHigh);
      if (deltaTimeInLedSequences < (currentDeltaTime * 0.8)) {

        uint32_t colorMed = strip.ColorHSV(hsvC, 255, 120);
        uint32_t colorLow = strip.ColorHSV(hsvC, 255, 20);
        byte medLed = ledInSequence - 1;
        if (medLed < 4) {
          medLed = WHEEL_NUM_LEDS - 1;
        }
        byte lowLed = ledInSequence - 2;
        if (lowLed < 4) {
          lowLed = WHEEL_NUM_LEDS - 1;
        }

        strip.setPixelColor(medLed, colorMed);
        strip.setPixelColor(lowLed, colorLow);
      }

      ledInSequence++;
      if (ledInSequence == WHEEL_NUM_LEDS) {
        wheelLoops++;
        if (wheelLoops == (WHEEL_LOOPS / 2)) {
          speedUp = false;
        }
        if ((wheelLoops % 4) == 0) {
          if (speedUp) {
            deltaTimeInLedSequences = (int)((deltaTimeInLedSequences * 7) / 10);
            Serial.println(deltaTimeInLedSequences);
          } else {
            deltaTimeInLedSequences = (int)((deltaTimeInLedSequences * 10) / 7);
            Serial.println(deltaTimeInLedSequences);
          }
          currentColorPattern++;
          currentColorPattern %= COLOR_PATTERNS;
        }
        ledInSequence = 4;
        if (wheelLoops == WHEEL_LOOPS) {
          isSpinning = false;
          wheelLoops = 0;
          speedUp = true;
          sequenceUp = false;
          deltaTimeInLedSequences = currentDeltaTime;
          currentColorPattern = 0;
        }
      }
    } else {
      resetAllLeds(&strip);
      strip.setPixelColor(ledInSequence, colorPattern[currentColorPattern]);
      if (CURRENT_MODE > DETAILS_MODE) {
        Serial.println("Sequence down");
      }
      ledInSequence--;
      if (ledInSequence == 0) {
        sequenceUp = true;
        currentColorPattern++;
        currentColorPattern %= COLOR_PATTERNS;
      }
    }

    timeLastSequenceUpdate = time;
    strip.show();
  }
}

void LedController::tails() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    if (CURRENT_MODE > DETAILS_MODE) {
      Serial.println("Sequence Up");
    }
    for (int i = WHEEL_NUM_LEDS - previousLedInSequence -1; i>ledInSequence; i--) {
      strip.setPixelColor(i, BLACK_COLOR);
    }
    strip.setPixelColor(ledInSequence, colorPattern[currentColorPattern]);
    if (CURRENT_MODE > DETAILS_MODE) {
      Serial.println("Is Spinning");
    }
    strip.show();

    int hsvC = hue;
    hue += hueIncrement;
    uint32_t colorHigh = strip.ColorHSV(hsvC, 255, 255);
    strip.setPixelColor(ledInSequence, colorHigh);
    uint32_t colorMed = strip.ColorHSV(hsvC, 255, 120);
    uint32_t colorLow = strip.ColorHSV(hsvC, 255, 20);
    byte medLed = ledInSequence - 1;
    if (medLed > 0) {
      strip.setPixelColor(medLed, colorMed);
    }
    byte lowLed = ledInSequence - 2;
    if (lowLed > 0) {
      strip.setPixelColor(lowLed, colorLow);
    }
    
    ledInSequence++;
    if (ledInSequence == WHEEL_NUM_LEDS - previousLedInSequence -1) {
      wheelLoops++;
      if ((wheelLoops % 4) == 0) {
        currentColorPattern++;
        currentColorPattern %= COLOR_PATTERNS;
      }
      previousLedInSequence++;
      ledInSequence = previousLedInSequence;
    }
    
    timeLastSequenceUpdate = time;
    strip.show();
  } 

  
}

byte LedController::getMin(byte r, byte g, byte b) {
  if (r == 0) {
    r = g;
  }
  if (g == 0) {
    r = b;
    g = b;
  }
  byte min = r < g ? r : g;
  min = min < b ? min : b;
  return min;
}

void LedController::growingLedSequence() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
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
        if (ledInSequence == 0) {
          shrink = false;
          sequenceUp = true;
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

void LedController::resetAllLeds(Adafruit_NeoPixel* theStrip) {
  for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
    theStrip->setPixelColor(i, BLACK_COLOR);
  }
}

#endif