#ifndef ButtonController_h
#define ButtonController_h

#include "Arduino.h"
#include "Constants.h"
#include "ButtonInfo.h"

class ButtonController {
private:

  ButtonInfo* buttonLights;
  ButtonInfo* buttonMute;
  ButtonInfo* buttonStepper;
  ButtonInfo* buttonSleeping;

public:
  ButtonController() {
    buttonLights = &ButtonInfo(BUTTON_LED_SEQUENCE);
    buttonMute = &ButtonInfo(BUTTON_MUTE);
    buttonStepper = &ButtonInfo(BUTTON_WHEEL_ROTATION);
    buttonSleeping = &ButtonInfo(BUTTON_SLEEPING);
  };
  void init();
  void begin();
  void operate();
  ButtonInfo* lights() {
    return buttonLights;
  };
  ButtonInfo* mute() {
    return buttonMute;
  };
  ButtonInfo* stepper(){return buttonStepper;};
  ButtonInfo* sleeping() {
    return buttonSleeping;
  };
};



void ButtonController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ButtonController begin");
  buttonLights->begin();
  buttonMute->begin();
  buttonSleeping->begin();
  buttonStepper->begin();
}

void ButtonController::init() {
}

void ButtonController::operate() {
  buttonSleeping->operate();
  buttonStepper->operate();
  buttonMute->operate();
  buttonLights->operate();
}


#endif