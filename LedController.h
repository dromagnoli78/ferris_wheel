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



#define DO_NOTHING -1
#define PATTERN_SINGLE 0
#define PATTERN_GROWING 1
#define PATTERN_BOUNCING 2
#define PATTERN_WHIPE 3
#define PATTERN_FULL 4
#define PATTERN_RAINBOW 5
#define PATTERN_THEATHER 6
#define PATTERN_RGB_LOOP 6

#define SEQUENCE_PATTERNS 8

#define COLOR_PATTERNS 6


#define BLACK_COLOR strip.Color(0, 0, 0)
#define WHITE_COLOR strip.Color(255, 255, 255)


class LedController {
private:
  //CRGBPalette16 currentPalette;
  //TBlendType currentBlending;
  long lastTimeOnButtonControl;
  long deltaTimeOnLedChange = 100;
  long deltaTimeWhenSequence = LIGHTS_DELAY;
  //CRGB leds[WHEEL_NUM_LEDS];
  //CRGBArray<WHEEL_NUM_LEDS> ledsArr;
  uint32_t colorPattern[COLOR_PATTERNS];
  int currentColorPattern = 0;
  long lastTimeOnLedUpdate;
  long lastTimeOnSequenceUpdate;
  int pattern;
  int ledInSequence = 0;
  bool sequenceUp = true;
  bool sequenceTriggered = false;
  Adafruit_NeoPixel strip;
  long debugTime = 0;
  int k_rgb_loop = 0;
  int colorSequence = 0;
  bool fadeIn = true;
  bool shrink = false;
public:
  LedController(/*ButtonController pButtonController, DisplayController* pDisplayController*/) {
    //buttonController = pButtonController;
    //displayController = pDisplayController;
    strip = Adafruit_NeoPixel(WHEEL_NUM_LEDS, WHEEL_LIGHTS_DATA_PIN, NEO_GRB + NEO_KHZ800);
  };
  void init();
  void begin();
  void operate();
  void ledSequence();
  void nextSequence() {
    if (CURRENT_MODE == DEBUG_MODE)
      Serial.println("LedController nextSequence");
    sequenceTriggered = true;
    ledInSequence = 0;
    fadeIn = true;
    sequenceUp = true;
    //strip.setPixelColor(4, strip.Color(100,0,0));
    //strip.show();
  };
  void singleLedSequence();
  void growingLedSequence();
  void rgbLoop();
  void rainbow();
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


void LedController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController begin");
  delay(3000);  // 3 second delay for recovery

  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE, WHEEL_LIGHTS_DATA_PIN, COLOR_ORDER>(leds, WHEEL_NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);
  //strip = Adafruit_NeoPixel(WHEEL_NUM_LEDS, WHEEL_LIGHTS_DATA_PIN, NEO_GRB + NEO_KHZ800);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);
  strip.setBrightness(BRIGHTNESS);
  for (int i = 0; i < WHEEL_NUM_LEDS; i++) {
    strip.setPixelColor(i, BLACK_COLOR);
  }
  strip.show();
}

void LedController::init() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController init");
  long time = millis();
  lastTimeOnButtonControl = time;
  lastTimeOnSequenceUpdate = time;
  colorPattern[0] = strip.Color(255, 0, 0);    //RED
  colorPattern[1] = strip.Color(255, 0, 255);  //PINK
  colorPattern[2] = strip.Color(0, 0, 255);    // BLUE
  colorPattern[3] = strip.Color(0, 255, 255);  //CYAN
  colorPattern[4] = strip.Color(0, 255, 0);    //GREEN
  colorPattern[5] = strip.Color(255, 255, 0);  //YELLOW
  pattern = DO_NOTHING;
}

void LedController::operate() {
  if (CONTROL_LIGHTS == DISABLED) return;
  long time = millis();
  if (time - lastTimeOnLedUpdate > deltaTimeOnLedChange) {
    if (sequenceTriggered) {

      //displayController -> displayMessage("Verde");
      //currentColorPattern++;
      pattern++;

      lastTimeOnLedUpdate = time;
      //currentColorPattern = currentColorPattern % COLOR_PATTERNS;
      pattern = pattern % SEQUENCE_PATTERNS;
      if (CURRENT_MODE == DEBUG_MODE) {
        Serial.print("LedController changing sequence:");
        Serial.println(pattern);
      }
      sequenceTriggered = false;
      //  buttonController -> setColorChanged(false);
    }
  }

  if (CURRENT_MODE == DEBUG_MODE) {
    if (time - debugTime > 10000) {
      Serial.println("LedController operate");
      debugTime = time;
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
  }
}

void LedController::singleLedSequence() {
  long time = millis();
  if (time - lastTimeOnSequenceUpdate > deltaTimeWhenSequence) {
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
      }
    }

    lastTimeOnSequenceUpdate = time;
    strip.show();
  }
}

void LedController::growingLedSequence() {
  long time = millis();
  if (time - lastTimeOnSequenceUpdate > deltaTimeWhenSequence) {
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
        for (int i = ledInSequence-1; i >= 0; i--) {
          strip.setPixelColor(i, BLACK_COLOR);
        }
        ledInSequence--;
        if (ledInSequence == 0) {
            shrink = true;
            ledInSequence = WHEEL_NUM_LEDS-1;
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
        }
      }

    }

    lastTimeOnSequenceUpdate = time;
    strip.show();
  }
}

void LedController::rgbLoop() {
  long time = millis();
  if (time - lastTimeOnSequenceUpdate > DELTA_RGB_LOOP) {

    // Fade IN
    if (fadeIn) {
      switch (colorSequence) {
        case 0: setAll(k_rgb_loop, 0, 0); break;
        case 1: setAll(0, k_rgb_loop, 0); break;
        case 2: setAll(0, 0, k_rgb_loop); break;
      }

      k_rgb_loop++;
      if (k_rgb_loop == 256) {
        fadeIn = false;
        k_rgb_loop--;
      }
    } else {


      // Fade OUT
      //for(int k = 255; k >= 0; k--) {
      switch (colorSequence) {
        case 0: setAll(k_rgb_loop, 0, 0); break;
        case 1: setAll(0, k_rgb_loop, 0); break;
        case 2: setAll(0, 0, k_rgb_loop); break;
      }
      k_rgb_loop--;
      if (k_rgb_loop == 0) {
        fadeIn = true;
        colorSequence++;
        colorSequence = colorSequence % 3;
      }
    }
    lastTimeOnSequenceUpdate = time;
  }
}


/*void LedController::rainbow() {
  byte *c;
  uint16_t i, j;
  if (time - lastTimeOnSequenceUpdate > DELTA_RGB_LOOP) {
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
      for(i=0; i< WHEEL_NUM_LEDS; i++) {
        c=Wheel(((i * 256 / WHEEL_NUM_LEDS) + j) & 255);
        setPixel(i, *c, *(c+1), *(c+2));
      }
      showStrip();
      
    }
  }
}

// used by rainbowCycle and theaterChaseRainbow
byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
*/


#endif