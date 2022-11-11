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
  ButtonInfo buttons[NUM_BUTTONS];


public:
  ButtonController() {
    // Action buttons
    buttons[0] = ButtonInfo(BUTTON_MUSIC, 'M');
    buttons[1] = ButtonInfo(BUTTON_MUTE, 'm');
    buttons[2] = ButtonInfo(BUTTON_STEPPER, 'P');
    buttons[3] = ButtonInfo(BUTTON_LED_SEQUENCE, 'Q');
    buttons[4] = ButtonInfo(BUTTON_SLEEPING, 'S');

    // Control buttons
    buttons[5] = ButtonInfo(BUTTON_SETTINGS, 'T');
    buttons[6] = ButtonInfo(BUTTON_ARROW_LEFT, 'l');
    buttons[7] = ButtonInfo(BUTTON_ARROW_RIGHT, 'r');
    buttons[8] = ButtonInfo(BUTTON_ARROW_UP, 'u');
    buttons[9] = ButtonInfo(BUTTON_ARROW_DOWN, 'd');
  };
  void init();
  void begin();
  void operate();

  ButtonInfo* music() {
    return &buttons[0];
  };

  ButtonInfo* lights() {
    return &buttons[3];
  };

  ButtonInfo* mute() {
    return &buttons[1];
  };
  ButtonInfo* stepper() {
    return &buttons[2];
  };

  ButtonInfo* sleeping() {
    return &buttons[4];
  };

  ButtonInfo* settings() {
    return &buttons[5];
  };

  ButtonInfo* up() {
    return &buttons[8];
  };

  ButtonInfo* down() {
    return &buttons[9];
  };

  ButtonInfo* left() {
    return &buttons[6];
  };

  ButtonInfo* right() {
    return &buttons[7];
  };
};

void ButtonController::begin() {
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("ButtonController begin");
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].begin();
  }
}

void ButtonController::init() {
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("ButtonController init");
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].init();
  }
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