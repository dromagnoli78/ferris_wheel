#ifndef LedController_h
#define LedController_h

#include "Constants.h"
#include <Adafruit_NeoPixel.h>

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

// How many seconds to show each temperature before switching
#define DISPLAYTIME 20

// How many seconds to show black between switches
#define BLACKTIME 3

#define FRAMES_PER_SECOND 45
#define LIGHT_SENSOR_PIN 99999



#define PATTERN_SINGLE 0
#define PATTERN_GROWING 1
#define PATTERN_BOUNCING 2
#define PATTERN_WHIPE 3
#define PATTERN_FULL 4
#define PATTERN_RAINBOW 5
#define PATTERN_THEATHER 6

#define SEQUENCE_PATTERNS 7

#define COLOR_PATTERNS 6

#define BLACK_COLOR strip.Color(0,0,0)
#define WHITE_COLOR strip.Color(255,255,255)
  

class LedController {
private:
  //CRGBPalette16 currentPalette;
  //TBlendType currentBlending;
  long lastTimeOnButtonControl;
  long deltaTimeOnButtonControl = 50;
  long deltaTimeOnLedChange = 100;
  long deltaTimeWhenSequence = 200;
  //CRGB leds[WHEEL_NUM_LEDS];
  //CRGBArray<WHEEL_NUM_LEDS> ledsArr;
  uint32_t colorPattern[COLOR_PATTERNS];
  int currentColorPattern = 0;
  long lastTimeOnLedUpdate;
  long lastTimeOnSequenceUpdate;
  int pattern;
  int ledInSequence = 0;
  bool sequenceUp = true;
  Adafruit_NeoPixel strip;
  long debugTime = 0;
public:
  LedController(/*ButtonController pButtonController, DisplayController* pDisplayController*/){
    //buttonController = pButtonController;
    //displayController = pDisplayController;
  };
  void init();
  void begin();
  void operate();
  void ledSequence();
  void nextSequence(){};
  void singleLedSequence();
  void growingLedSequence();
};


//ColorWhipe
//bouncingBall
//FullColor
//singleSequence
//growingSequence
//rainbow
//theather(slow)
//RGBLoop


void LedController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController begin");
  delay(3000);  // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE, WHEEL_LIGHTS_DATA_PIN, COLOR_ORDER>(leds, WHEEL_NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);
  strip = Adafruit_NeoPixel(WHEEL_NUM_LEDS, WHEEL_LIGHTS_DATA_PIN, NEO_GRB + NEO_KHZ800);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);
  strip.setBrightness(BRIGHTNESS);

}

void LedController::init() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController init");
  long time = millis();
  lastTimeOnButtonControl = time;
  lastTimeOnSequenceUpdate = time;
  colorPattern[0] = strip.Color(255,0,0); //RED
  colorPattern[1] = strip.Color(255,0,255); //PINK
  colorPattern[2] = strip.Color(0,0,255);// BLUE
  colorPattern[3] = strip.Color(0,255,255);//CYAN
  colorPattern[4] = strip.Color(0,255,0); //GREEN
  colorPattern[5] = strip.Color(255,255,0); //YELLOW
  pattern = 1;
}

void LedController::operate() {
  if (CONTROL_LIGHTS == DISABLED) return;
  long time = millis();
  if (time - lastTimeOnLedUpdate > deltaTimeOnLedChange) {
    bool isColorChanged = false;
    if (isColorChanged) {
      //displayController -> displayMessage("Verde");
      currentColorPattern++;
      lastTimeOnLedUpdate = time;
      currentColorPattern = currentColorPattern % COLOR_PATTERNS;
      //  buttonController -> setColorChanged(false);
    }

    
    //FastLED.show();
  }
  if (CURRENT_MODE == DEBUG_MODE) {
     if (time - debugTime > 10000) {
      Serial.println("LedController operate");
      debugTime = time;
      }
   }

  ledSequence();
}

void LedController::ledSequence(){
  switch (pattern) {
      case PATTERN_SINGLE:
        singleLedSequence();
        break;
      case PATTERN_GROWING:
        growingLedSequence();
        break;
      case PATTERN_FULL:
        growingLedSequence();
        break;
    }
}

void LedController::singleLedSequence() {
  long time = millis();
  if (time - lastTimeOnSequenceUpdate > deltaTimeWhenSequence) {
    for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
      strip.setPixelColor(i, BLACK_COLOR);
    }
    strip.setPixelColor(ledInSequence, colorPattern[currentColorPattern]);
    ledInSequence++;
    ledInSequence %= WHEEL_NUM_LEDS;
    lastTimeOnSequenceUpdate = time;
  }
}
void LedController::growingLedSequence() {
  long time = millis();
  if (time - lastTimeOnSequenceUpdate > deltaTimeWhenSequence) {
    if (sequenceUp) {
      for (int i = 0; i <= ledInSequence; i++) {
        strip.setPixelColor(i, colorPattern[currentColorPattern]);
      }
      for (int i = ledInSequence + 1; i < WHEEL_NUM_LEDS; i++) {
        strip.setPixelColor(i,BLACK_COLOR);
      }
      ledInSequence++;
      if (ledInSequence == WHEEL_NUM_LEDS) {
        sequenceUp = false;
        ledInSequence--;
      }

    } else {
      for (int i = WHEEL_NUM_LEDS - 1; i >= ledInSequence; i--) {
        strip.setPixelColor(i, colorPattern[currentColorPattern]);
      }
      for (int i = ledInSequence; i >= 0; i--) {
        strip.setPixelColor(i,BLACK_COLOR);
      }
      ledInSequence--;


      if (ledInSequence == -1) {
        sequenceUp = true;
        ledInSequence = 0;
      }
    }

    lastTimeOnSequenceUpdate = time;
  }
}


#endif