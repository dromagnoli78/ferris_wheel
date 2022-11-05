#ifndef ButtonController_h
#define ButtonController_h

#include "Arduino.h"
#include "Constants.h"
#include "ButtonInfo.h"

#define NUM_BUTTONS 10

/**
Class holding all the buttons and controlling them
*/
class ButtonController {
private:

  // Action buttons
  ButtonInfo buttonMusic;
  ButtonInfo buttonLights;
  ButtonInfo buttonMute;

  ButtonInfo buttonStepper;
  ButtonInfo buttonSleeping;

  // Control buttons
  ButtonInfo buttonSettings;
  ButtonInfo buttonArrowLeft;
  ButtonInfo buttonArrowRight;
  ButtonInfo buttonArrowUp;
  ButtonInfo buttonArrowDown;

  ButtonInfo buttons[NUM_BUTTONS];


public:
  ButtonController() {
    buttonMusic = ButtonInfo(BUTTON_MUSIC,'M');
    buttonMute = ButtonInfo(BUTTON_MUTE,'m');
    buttonStepper = ButtonInfo(BUTTON_STEPPER,'P');
    buttonLights = ButtonInfo(BUTTON_LED_SEQUENCE,'Q');

    buttonSleeping = ButtonInfo(BUTTON_SLEEPING,'S');
    buttonSettings = ButtonInfo(BUTTON_SETTINGS,'T');

    buttonArrowLeft = ButtonInfo(BUTTON_ARROW_LEFT,'l');
    buttonArrowRight = ButtonInfo(BUTTON_ARROW_RIGHT,'r');
    buttonArrowUp = ButtonInfo(BUTTON_ARROW_UP,'u');
    buttonArrowDown = ButtonInfo(BUTTON_ARROW_DOWN,'d');

    buttons[0] = buttonMusic;
    buttons[1] = buttonMute;
    buttons[2] = buttonStepper;
    buttons[3] = buttonLights;
    buttons[4] = buttonSleeping;
    buttons[5] = buttonSettings;
    buttons[6] = buttonArrowLeft;
    buttons[7] = buttonArrowRight;
    buttons[8] = buttonArrowUp;
    buttons[9] = buttonArrowDown;
  };
  void init();
  void begin();
  void operate();

  ButtonInfo* music() {
    return &buttons[0];
  };

  ButtonInfo* lights() {
    return &buttonLights;
  };
  ButtonInfo& mute() {
    return buttonMute;
  };
  ButtonInfo* stepper() {
    return &buttons[2];
  };
  ButtonInfo& sleeping() {
    return buttonSleeping;
  };
  ButtonInfo& settings() {
    return buttonSettings;
  };
  ButtonInfo& up() {
    return buttonArrowUp;
  };

  ButtonInfo& down() {
    return buttonArrowDown;
  };

  ButtonInfo& left() {
    return buttonArrowLeft;
  };

  ButtonInfo& right() {
    return buttonArrowRight;
  };
};

void ButtonController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ButtonController begin");
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].begin();
  }
}

void ButtonController::init() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ButtonController init");
}

void ButtonController::operate() {
  if (CONTROL_BUTTON == DISABLED) return;
  int lastClickedButton = -1;

  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].operate();
    if (buttons[i].isClicked()) {
      lastClickedButton = i;
    }
  }
}


#endif