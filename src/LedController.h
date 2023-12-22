#ifndef LedController_h
#define LedController_h

#include "Constants.h"
#include "TailSequence.h"
#include <Adafruit_NeoPixel.h>

// How many seconds to show each temperature before switching
#define DISPLAYTIME 20

// How many seconds to show black between switches
#define BLACKTIME 3

#define FRAMES_PER_SECOND 45
#define RANDOM_PATTERN_DURATION 30000

#define REQUEST_OFF 88
#define REQUEST_ON 77

#define PATTERN_SINGLE 0
#define PATTERN_WHEEL PATTERN_SINGLE + 1
#define PATTERN_GROWING PATTERN_WHEEL + 1
#define PATTERN_BACK_AND_FORTH PATTERN_GROWING +1 
#define PATTERN_TAILS PATTERN_BACK_AND_FORTH +1 
#define PATTERN_RAINBOW PATTERN_TAILS +1
#define PATTERN_RGB_LOOP PATTERN_RAINBOW +1
#define PATTERN_CIRCLE PATTERN_RGB_LOOP +1
#define PATTERN_NEVER_END PATTERN_CIRCLE +1
#define PATTERN_FIXED_COLOR PATTERN_NEVER_END +1

#define PATTERN_RANDOM PATTERN_FIXED_COLOR +1
#define PATTERN_MAX_VALUE PATTERN_RANDOM

#define PATTERN_SLEEPING 100
#define PATTERN_SHUTDOWN 101

#define SEQUENCE_PATTERNS 11
#define COLOR_PATTERNS 6

#define BLACK_COLOR strip.Color(0, 0, 0)
#define WHITE_COLOR strip.Color(255, 255, 255)
#define BLUE_COLOR strip.Color(0, 0, 255)
#define YELLOW_COLOR strip.Color(255, 255, 0)
#define GREEN_COLOR strip.Color(0, 255, 0)
#define RED_COLOR strip.Color(255, 0, 0)


const char* sequenceNames[]={
  "Sequence",
  "Spinning",
  "Growing",
  "Dribbling",
  "Tails",
  "Rainbow",
  "RGB", 
  "Circle",
  "Looping",
  "Fixed",
  "Random"
};

class LedController {
private:
  // Timing variables
  unsigned long timeLastCheckOnLedTriggers;
  unsigned long timeLastSequenceUpdate;
  unsigned long timeLastDebug = 0;
  unsigned long timeLastSleepCheck;
  unsigned long timeLastRandomCheck=0;
  unsigned long timeSleepHasStarted;
  unsigned long timeSleepStartEffect = 0;

  int currentDeltaTime = LIGHTS_DELAY;
  int deltaTimeOnLedChange = currentDeltaTime;
  int deltaTimeInLedSequences = currentDeltaTime;
  int deltaTimeOnSleepBreath = DELTA_SLEEPING_SLOW;
  int currentRandomPattern = PATTERN_SINGLE;
  int randomPatternDuration = RANDOM_PATTERN_DURATION;
  

  uint32_t colorPattern[COLOR_PATTERNS];
  uint32_t currentColorPattern = 0;

  int previousPattern = 0;
  int pattern = -1;
  int ledInSequence = 0;
  int previousLedInSequence = 0;
  int rgbSequence = 0;
  int repeated = 0;
  int sequenceRequest = 0;
  int sleepStartPixel = 0;
  int sleepEndPixel = WHEEL_NUM_LEDS - 1;
  bool sequenceUp = true;
  const char* sequenceName = sequenceNames[0];
  
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
  int matrixSequenceStep = 0;
  int phase = 0;
  int hueIncrement = 65536/(WHEEL_LOOPS * 4);

  // Sleep lights
  int settingSleepMinLight = MIN_VALUE_LIGHT_FOR_SLEEP;
  int settingSleepMaxLight = MAX_VALUE_LIGHT_FOR_SLEEP;
  int settingLightsMinDelay = LIGHTS_DELAY_MIN;
  int settingSleepShutdownTime;
  int settingLedInitialBrightness = LED_INITIAL_BRIGHTNESS;
  int brightness = settingLedInitialBrightness;
  int settingLedMinBrightness;
  int settingLedMaxBrightness;
  int settingEyeLedMinBrightness;
  int settingEyeLedMaxBrightness;
  int settingSleepSequence;

  Adafruit_NeoPixel strip;
  Adafruit_NeoPixel stripEye;

public:
  LedController() {
    strip = Adafruit_NeoPixel(WHEEL_NUM_LEDS, WHEEL_LIGHTS_DATA_PIN, NEO_GRB + NEO_KHZ800);
    stripEye = Adafruit_NeoPixel(2, EYES_LED_DATA_PIN, NEO_GRB + NEO_KHZ800);
  };
  void init();
  void initStripEye();
  void begin();
  void operate();
  void ledSequence(int patternSequence);
  void nextSequence(int i);
  void settingsChange(u_char c);
  byte* wheel(byte wheelPos);
  void singleLedSequence();
  void wheelSpinning();
  void growingLedSequence();
  void matrixSequence(const char* sequence[], int sequenceLenght);
  void rgbLoop();
  void rainbow();
  void circle();
  void randomize();
  void neverEnd();
  void fixedColor();
  void turnItOff();
  void turnItOn();
  void sleeping(bool isSleeping);
  void sleepingSequence();
  void sleepingSequence2();
  void shutdownSequence();
  void updateSleepShutdownTime(int iSettingSleepShutdownTime){settingSleepShutdownTime = iSettingSleepShutdownTime;}
  void setSleepMaxLight(int iSettingSleepMaxLight){settingSleepMaxLight = iSettingSleepMaxLight;}
  void setSleepMinLight(int iSettingSleepMinLight){settingSleepMinLight = iSettingSleepMinLight;}
  void updateLightsMinDelay(int iSettingLightsMinDelay){settingLightsMinDelay = iSettingLightsMinDelay;}
  void updateLigthsInitialBrightness(int iSettingLedInitialBrightness){settingLedInitialBrightness = iSettingLedInitialBrightness;}
  void updateLedMinBrightness(int iMinBrightness){settingLedMinBrightness = iMinBrightness;}
  void updateLedMaxBrightness(int iMaxBrightness){settingLedMaxBrightness = iMaxBrightness;}
  void updateEyeLedMinBrightness(int iMinBrightness){settingEyeLedMinBrightness = iMinBrightness;}
  void updateEyeLedMaxBrightness(int iMaxBrightness){settingEyeLedMaxBrightness = iMaxBrightness;}
  void updateLedRandomPatternDuration(int iRandomDuration){randomPatternDuration = iRandomDuration;}
  void resetSleepSequence(){sleepEndPixel = WHEEL_NUM_LEDS -1; sleepStartPixel = 0;}
  void updateSleepSequence(int iSettingSleepSequence){
    settingSleepSequence = iSettingSleepSequence;
    sleepStartPixel = settingSleepSequence == 0 ? 0 : 4;
    sleepEndPixel = WHEEL_NUM_LEDS - 1;
    }

  const char* getNextSequenceName() {return sequenceName;};
  
  void adjustBrightness(int sensorValue) {
    int value = map(sensorValue,0,4096, settingLedMinBrightness, settingLedMaxBrightness);
    int eyeValue = map(sensorValue,0,4096, settingEyeLedMinBrightness, settingEyeLedMaxBrightness);
    brightness = value;
    strip.setBrightness(value);
    stripEye.setBrightness(eyeValue);
    stripEye.setPixelColor(0, BLUE_COLOR);
    stripEye.setPixelColor(1, BLUE_COLOR);
    stripEye.show();
  }
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

void LedController::nextSequence(int i) {
  dbg("LedController nextSequence");
  sequenceRequest = i;
  off = false;
  requestOff = false;
  ledInSequence = 0;
  fadeIn = true;
  sequenceUp = true;
  isSpinning = false;
  wheelLoops = 0;
  speedUp = true;
  hue = 0;
  int sequencePattern = pattern + i;
  sequencePattern = sequencePattern % SEQUENCE_PATTERNS;
  sequenceName = sequenceNames[sequencePattern];

  deltaTimeInLedSequences = currentDeltaTime;
}

void LedController::settingsChange(u_char c) {
  if (pattern != PATTERN_FIXED_COLOR) {
    switch (c) {
    case 'U':
      currentDeltaTime-=LIGHTS_DELAY_INCREMENT;
      dbg("Lights speed-up", currentDeltaTime);
      if (currentDeltaTime < settingLightsMinDelay)
        currentDeltaTime = settingLightsMinDelay;
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
  } else {
    switch (c) {
    case 'U':
      hue += 1024;
      break;
    case 'D':
      if (hue >= 1024) {
        hue-= 1024;
      } else {
        int step = 1024-hue;
        hue = 65536 - step;
      }
      break;
    }
    hue %= 65536;
  }

}

void LedController::sleeping(bool isSleeping) {
  sleepMode = isSleeping;
  if (sleepMode) {
    previousPattern = pattern;
    pattern = PATTERN_SLEEPING;
    rgb_intensity = settingSleepMaxLight;
    stripEye.setBrightness(settingLedInitialBrightness-10);
    stripEye.setPixelColor(0, BLACK_COLOR);
    stripEye.setPixelColor(1, BLACK_COLOR);
    delay(50);
    stripEye.show();
    timeSleepHasStarted = millis();
    dbg("LedController going to sleep with sleepTime:", timeSleepHasStarted);
    fadeIn = false;
  } else {
    initStripEye();
  }
}

void LedController::begin() {
  if (CONTROL_LIGHTS == CTL_DISABLED) return;
  dbg("LedController begin");
  pinMode(ENABLE_POWER_LED, OUTPUT);
  delay(SETUP_DELAY);  // 3 second delay for recovery
  randomSeed(analogRead(0));
  strip.setBrightness(0);
  stripEye.setBrightness(0);
  
}

void LedController::init() {
  if (CONTROL_LIGHTS == CTL_DISABLED) return;
  dbg("LedController init");
  off = true;
  digitalWrite(ENABLE_POWER_LED, HIGH);
  strip.setBrightness(settingLedInitialBrightness);
  for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
    strip.setPixelColor(i, BLACK_COLOR);
  }
  delay(50);
  strip.show();
  initStripEye();
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

void LedController::initStripEye() {
  stripEye.setBrightness(settingLedInitialBrightness-10);
  stripEye.setPixelColor(0, BLUE_COLOR);
  stripEye.setPixelColor(1, BLUE_COLOR);
  delay(50);
  stripEye.show();
}

void LedController::operate() {
  if (CONTROL_LIGHTS == CTL_DISABLED) return;
  unsigned long time = millis();
  if (time - timeLastCheckOnLedTriggers > deltaTimeOnLedChange) {
    if (sequenceRequest != 0 && !sleepMode) {
      pattern+=sequenceRequest;
      timeLastCheckOnLedTriggers = time;
      pattern = pattern % SEQUENCE_PATTERNS;
      dbg("LedController changing sequence:", (unsigned long) pattern);
      sequenceRequest = 0;
    }
  }

  if (CURRENT_MODE > DEBUG_MODE) {
    if (time - timeLastDebug > 10000) {
      Serial.println("LedController operate");
      timeLastDebug = time;
    }
  }

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
    case PATTERN_SLEEPING:
      sleepingSequence();
      break;
    case PATTERN_SHUTDOWN:
      shutdownSequence();
      break;
    default:
      resetSleepSequence();
      ledSequence(pattern);
  }
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

void LedController::ledSequence(int patternSequence) {
  switch (patternSequence) {
    case PATTERN_SINGLE:
      singleLedSequence();
      break;
    case PATTERN_WHEEL:
      wheelSpinning();
      break;
    case PATTERN_GROWING:
      growingLedSequence();
      break;
    case PATTERN_BACK_AND_FORTH:
      matrixSequence(backAndForthSequence, BACK_AND_FORTH_SEQUENCE_LENGTH);
      break;
    case PATTERN_TAILS:
      matrixSequence(tailSequence, TAIL_SEQUENCE_LENGTH);
      break;
    case PATTERN_RAINBOW:
      rainbow();
      break;
    case PATTERN_RGB_LOOP:
      rgbLoop();
      break;
    case PATTERN_CIRCLE:
      circle();
      break;
    case PATTERN_NEVER_END:
      neverEnd();
      break;
    case PATTERN_FIXED_COLOR:
      fixedColor();
      break;
     case PATTERN_RANDOM:
      randomize();
      break;
  }
}

void LedController::sleepingSequence() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - timeSleepStartEffect;
  unsigned long fromStart = currentTime - timeSleepHasStarted;

  if (elapsed >= DELTA_TIME_SLEEP_BREATH) {
    timeSleepStartEffect = currentTime;
    breathPhase = 0.0;
  } else {
    breathPhase = 2 * PI * (float)elapsed / (float)DELTA_TIME_SLEEP_BREATH;
  }

  float brightness = 0.5 * (settingSleepMaxLight - settingSleepMinLight) * (1.0 + sin(breathPhase)) + settingSleepMinLight;
  int i1 = settingSleepSequence == 0 ? 4 : sleepStartPixel;
  int i2 = settingSleepSequence == 0 ? 7 : sleepEndPixel;

  if (currentTime - timeLastSleepCheck >= 20) {
    for (int i = 0; i < i1; i++) {
      strip.setPixelColor(i, BLACK_COLOR);
    }
    for (int i = i1; i <= i2; i++) {

      strip.setPixelColor(i, strip.Color((uint8_t)(i < 4? brightness - 7 : brightness), 0, 0));
    }
    strip.show();
    timeLastSleepCheck = currentTime;
  }
  if (settingSleepSequence == 1) {
    long deltaSleepShutdownDecrement = settingSleepShutdownTime / 8;
    if (fromStart > deltaSleepShutdownDecrement*6) {
        sleepStartPixel = 5;
        sleepEndPixel = 6;
    } else if (fromStart > deltaSleepShutdownDecrement * 4){
        sleepStartPixel = 4;
        sleepEndPixel = 7;
    } else if (fromStart > deltaSleepShutdownDecrement * 3){
        sleepStartPixel = 3;
        sleepEndPixel = 7;
    } else if (fromStart > deltaSleepShutdownDecrement * 2){
        sleepStartPixel = 2;
        sleepEndPixel = 7;
    } else if (fromStart > deltaSleepShutdownDecrement){
        sleepStartPixel = 1;
        sleepEndPixel = 7;
    } 
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
      if (rgb_intensity == (settingSleepMinLight + SLEEP_THRESHOLD)) {
        deltaTimeOnSleepBreath = DELTA_SLEEPING_SLOW;
      }
      if (rgb_intensity == settingSleepMaxLight) {
        fadeIn = false;
      }
    } else {
      rgb_intensity--;
      if (rgb_intensity == (settingSleepMinLight + SLEEP_THRESHOLD)) {
        deltaTimeOnSleepBreath = DELTA_SLEEPING_FAST;
      }
      if (rgb_intensity == settingSleepMinLight) {
        fadeIn = true;
      }
    }
    timeLastSleepCheck = time;
    if (time - timeSleepHasStarted > (settingSleepShutdownTime - 3000)) {
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
    if (rgb_intensity == (settingSleepMinLight + SLEEP_THRESHOLD)) {
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
        if (repeated == 3) {
          ledInSequence = 3;
        } else {
          ledInSequence--;
        }
        sequenceUp = false;
        
      }
    } else {
      ledInSequence--;
      if (ledInSequence == 3) {
        if (repeated == 0) {
          repeated = 1;
        } else if (repeated == 2) {
          ledInSequence = 4;
          sequenceUp = true;
          repeated = 3;
        }
      }
      if (ledInSequence == 2 && repeated == 1) {
        ledInSequence = WHEEL_NUM_LEDS -1;
        repeated = 2;
      }
      if (ledInSequence == 0) {
        sequenceUp = true;
        currentColorPattern++;
        currentColorPattern %= COLOR_PATTERNS;
        repeated = 0;
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
      if (deltaTimeInLedSequences < 9) {
        for (int i=4; i<8; i++) {
          strip.setPixelColor(i, colorHigh);
        }
      } else {
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
      }

      ledInSequence++;
      if (ledInSequence == WHEEL_NUM_LEDS) {
        wheelLoops++;
        if (wheelLoops == (WHEEL_LOOPS / 2)) {
          speedUp = false;
        }
        if ((wheelLoops % 2) == 0) {
          if (speedUp) {
            deltaTimeInLedSequences = (int)((deltaTimeInLedSequences * 14) / 15);
            Serial.println(deltaTimeInLedSequences);
          } else {
            deltaTimeInLedSequences = (int)((deltaTimeInLedSequences * 15) / 14);
            Serial.println(deltaTimeInLedSequences);
          }
          if ((wheelLoops % 4) == 0) {
            currentColorPattern++;
            currentColorPattern %= COLOR_PATTERNS;
          }
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

void LedController::matrixSequence(const char* cSequence[], int fullSequenceLenght) {

  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    int hsvC = hue;
    hue += hueIncrement;
    int complHsv = (hsvC + 8192) % 65536;
    uint32_t colorHigh = strip.ColorHSV(hsvC, 255, 255);
    uint32_t colorMed = strip.ColorHSV(hsvC, 255, 80);
    uint32_t colorLow = strip.ColorHSV(hsvC, 255, 10);
    uint32_t complColorHigh = strip.ColorHSV(complHsv, 255, 255);
    uint32_t complColorMed = strip.ColorHSV(complHsv, 255, 80);
    uint32_t complColorLow = strip.ColorHSV(complHsv, 255, 10);
    byte medLed = ledInSequence - 1;
    byte lowLed = ledInSequence - 2;
    const char* sequence = cSequence[matrixSequenceStep];

    for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
      char ch = sequence[i];
      uint32_t pixelColor;
      switch (ch) {
        case '0':
          pixelColor = BLACK_COLOR;
          break;
        case '1':
          pixelColor = complColorLow;
          break;
        case '2':
          pixelColor = complColorMed;
          break;
        case '3':
          pixelColor = complColorHigh;
          break;
        case '4':
          pixelColor = colorHigh;
          break;
      }
      strip.setPixelColor(i, pixelColor);
    }
    matrixSequenceStep++;
    if (matrixSequenceStep == fullSequenceLenght -1) {
      matrixSequenceStep = 0;
      sequenceUp = true;
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

/** 
 * Special case for i==4 and i==5 since we are going to growing
 * symmetrically with respect to the arm
*/
void LedController::growingLedSequence() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    if (sequenceUp) {
      if (!shrink) {
        for (int i = 0; i <= ledInSequence; i++) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, colorPattern[currentColorPattern]);
          }
        }
        for (int i = ledInSequence + 1; i < WHEEL_NUM_LEDS - 1; i++) {
          strip.setPixelColor(i, BLACK_COLOR);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, BLACK_COLOR);
          }
        }
        ledInSequence++;
        if (ledInSequence == WHEEL_NUM_LEDS - 2) {
          shrink = true;
          ledInSequence = 0;
        }
      } else {
        for (int i = ledInSequence; i < WHEEL_NUM_LEDS - 1; i++) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, colorPattern[currentColorPattern]);
          }
        }
        for (int i = 0; i < ledInSequence; i++) {
          strip.setPixelColor(i, BLACK_COLOR);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, BLACK_COLOR);
          }
        }
        ledInSequence++;
        if (ledInSequence == WHEEL_NUM_LEDS - 2) {
          shrink = false;
          sequenceUp = false;
          ledInSequence--;
        }
      }

    } else {
      if (!shrink) {
        for (int i = WHEEL_NUM_LEDS - 3; i >= ledInSequence; i--) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, colorPattern[currentColorPattern]);
          }
        }
        for (int i = ledInSequence - 1; i >= 0; i--) {
          strip.setPixelColor(i, BLACK_COLOR);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, BLACK_COLOR);
          }
        }
        ledInSequence--;
        if (ledInSequence == 0) {
          shrink = true;
          ledInSequence = WHEEL_NUM_LEDS - 3;
        }
      } else {
        for (int i = ledInSequence; i >= 0; i--) {
          strip.setPixelColor(i, colorPattern[currentColorPattern]);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, colorPattern[currentColorPattern]);
          }
        }
        for (int i = WHEEL_NUM_LEDS - 3; i > ledInSequence; i--) {
          strip.setPixelColor(i, BLACK_COLOR);
          if (i == 4 || i == 5) {
            strip.setPixelColor(WHEEL_NUM_LEDS - i + 3, BLACK_COLOR);
          }
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


void LedController::neverEnd() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    int hsvC = hue;
    hue += hueIncrement;
    uint32_t color = strip.ColorHSV(hsvC, 255, 255);
    phase++;
    phase%=3;
    for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
      if ((i + phase) % 3 == 0) {
        strip.setPixelColor(i, color);
      } else {
        strip.setPixelColor(i, BLACK_COLOR);
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
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
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

void LedController::circle() {
  byte* c;
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    c = wheel(((256 / WHEEL_NUM_LEDS) + ledInSequence) & 255);
    for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
      if (i<4) {
      strip.setPixelColor(i, BLACK_COLOR);  
      } else {
        strip.setPixelColor(i, *c, *(c + 1), *(c + 2));
      }
    }
    ledInSequence++;
    if (ledInSequence == 256 * 5) {
      ledInSequence = 0;
    }
    strip.show();
    timeLastSequenceUpdate = time;
  }
}

void LedController::fixedColor() {
  unsigned long time = millis();
  if (time - timeLastSequenceUpdate > deltaTimeInLedSequences) {
    int hsvC = hue;
    uint32_t color = strip.ColorHSV(hsvC, 255, 255);
    for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
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

void LedController::randomize() {
  unsigned long time = millis();
  if (time - timeLastRandomCheck > randomPatternDuration) {
    currentRandomPattern = random(PATTERN_MAX_VALUE);
    timeLastRandomCheck = time;
  }
  ledSequence(currentRandomPattern);

}
#endif