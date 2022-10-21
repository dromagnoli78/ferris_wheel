#ifndef LedController_h
#define LedController_h

#include "Constants.h"

#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

// How many seconds to show each temperature before switching
#define DISPLAYTIME 20
// How many seconds to show black between switches
#define BLACKTIME   3

#define FRAMES_PER_SECOND  45
#define LIGHT_SENSOR_PIN 99999

#define COLOR_PATTERNS  6


class LedController {
  private:
    CRGBPalette16 currentPalette;
    TBlendType    currentBlending;
    long lastTimeOnButtonControl;
    long deltaTimeOnButtonControl = 50;
    long deltaTimeOnLedChange = 100;
    long deltaTimeWhenSequence = 200;
    CRGB leds[WHEEL_NUM_LEDS];
    CRGBArray<WHEEL_NUM_LEDS> ledsArr;
    CRGB colorPattern[6];
    int currentColorPattern = 0;
    long lastTimeOnLedUpdate;
    long lastTimeOnSequenceUpdate;
    int pattern;
    int ledInSequence=0;
    bool sequenceUp = true;
  public:
    LedController(/*ButtonController pButtonController, DisplayController* pDisplayController*/)
    {
      //buttonController = pButtonController;
      //displayController = pDisplayController;
      };
  void init();  
  void begin();
  void operate();
  void nextSequence(){};
  void singleLedSequence();
  void growingLedSequence();
};

void LedController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("LedController begin");
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,WHEEL_LIGHTS_DATA_PIN,COLOR_ORDER>(leds, WHEEL_NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void LedController::init() {
  long time = millis();
  lastTimeOnButtonControl = time;
  lastTimeOnSequenceUpdate = time;
  colorPattern[0]=CRGB::Red;
  colorPattern[1]=CRGB::Purple;
  colorPattern[2]=CRGB::Blue;
  colorPattern[3]=CRGB::Cyan;
  colorPattern[4]=CRGB::Green;
  colorPattern[5]=CRGB::Yellow;
  pattern = 1;
}

void LedController::operate()
{
  long time = millis();
  if (time - lastTimeOnLedUpdate > deltaTimeOnLedChange) {
   // bool isColorChanged = buttonController -> isColorChanged();
   bool isColorChanged = true;
    if (isColorChanged) {
      //displayController -> displayMessage("Verde");
      currentColorPattern++;
      lastTimeOnLedUpdate=time;
      currentColorPattern = currentColorPattern % COLOR_PATTERNS;
    //  buttonController -> setColorChanged(false);
    }

    switch(pattern){
      case 0:
        singleLedSequence();
        break;
      case 1:
        growingLedSequence();
        break;
    }
    FastLED.show();

  }
}

void LedController::singleLedSequence() {
  long time = millis();
  if (time - lastTimeOnSequenceUpdate > deltaTimeWhenSequence) {
     for(int i=0; i<WHEEL_NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
     }
     leds[ledInSequence] = colorPattern[currentColorPattern];
     ledInSequence++;
     ledInSequence %= WHEEL_NUM_LEDS;
     lastTimeOnSequenceUpdate = time;
  
  }
}
  void LedController::growingLedSequence() {
  long time = millis();
  if (time - lastTimeOnSequenceUpdate > deltaTimeWhenSequence) {
    if (sequenceUp){
      for(int i=0; i<=ledInSequence; i++) {
        leds[i] = colorPattern[currentColorPattern];
      }
      for(int i=ledInSequence+1; i<WHEEL_NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
       ledInSequence++;
     if (ledInSequence == WHEEL_NUM_LEDS) {
       sequenceUp = false;
       ledInSequence--;
     }

    } else {
      for(int i=WHEEL_NUM_LEDS-1; i>=ledInSequence; i--) {
        leds[i] = colorPattern[currentColorPattern];
      }
      for(int i=ledInSequence; i>=0; i--) {
        leds[i] = CRGB::Black;
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


  /*
    // Waves for LED position
  uint8_t posBeat  = beatsin8(30, 0, NUM_LEDS - 1, 0, 0);
  uint8_t posBeat2 = beatsin8(60, 0, NUM_LEDS - 1, 0, 0);
  uint8_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 127);
  uint8_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 127);


  // In the video I use beatsin8 for the positions. For longer strips,
  // the resolution isn't high enough for position and can lead to some
  // LEDs not lighting. If this is the case, use the 16 bit versions below
  // uint16_t posBeat  = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  // uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);
  // uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  // uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  // Wave for LED color
  uint8_t colBeat  = beatsin8(45, 0, 255, 0, 0);

  leds[(posBeat + posBeat2) / 2]  = CHSV(colBeat, 255, 255);
  leds[(posBeat3 + posBeat4) / 2]  = CHSV(colBeat, 255, 255);

  fadeToBlackBy(leds, NUM_LEDS, 10);

  FastLED.show();
  }




// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

*/




#endif
