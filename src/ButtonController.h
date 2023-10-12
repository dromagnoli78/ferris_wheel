#ifndef ButtonController_h
#define ButtonController_h

#include "Arduino.h"
#include "Constants.h"
#include "ButtonInfo.h"
#include <Wire.h>

volatile int expanderStatus = LOW;
volatile bool up = LOW;


//#define NUM_BUTTONS 2
#define NUM_BUTTONS 10

/**
Class holding all the buttons and controlling them
*/
class ButtonController {
private:
  ButtonInfo buttons[NUM_BUTTONS];
  unsigned long interruptCheckedTime;


public:
  ButtonController() {
    // Action buttons
    buttons[MUSIC_INDEX] = ButtonInfo(BUTTON_MUSIC, 'M', false);
    buttons[MUTE_INDEX] = ButtonInfo(BUTTON_MUTE, 'm', false);
    buttons[STEPPER_INDEX] = ButtonInfo(BUTTON_STEPPER, 'P', false);
    buttons[LIGHT_INDEX] = ButtonInfo(BUTTON_LED_SEQUENCE, 'Q', false);
    
    // Control buttons
    buttons[LEFT_INDEX] = ButtonInfo(BUTTON_ARROW_LEFT, 'l', false);
    buttons[RIGHT_INDEX] = ButtonInfo(BUTTON_ARROW_RIGHT, 'r', false);
    buttons[UP_INDEX] = ButtonInfo(BUTTON_ARROW_UP, 'u', false);
    buttons[DOWN_INDEX] = ButtonInfo(BUTTON_ARROW_DOWN, 'd', false);

    buttons[SLEEP_INDEX] = ButtonInfo(BUTTON_SLEEPING, 'S', true);
    buttons[SETTINGS_INDEX] = ButtonInfo(BUTTON_SETTINGS, 'T', true);
  };
  void init();
  void begin();
  void operate();

  static void readPortExpander() {
   expanderStatus = HIGH;
}

  ButtonInfo* music() {
    return &buttons[MUSIC_INDEX];
  };
  ButtonInfo* mute() {
    return &buttons[MUTE_INDEX];
  };
  ButtonInfo* stepper() {
    return &buttons[STEPPER_INDEX];
  };
  ButtonInfo* lights() {
    return &buttons[LIGHT_INDEX];
  };
  ButtonInfo* up() {
    return &buttons[UP_INDEX];
  };
  ButtonInfo* down() {
    return &buttons[DOWN_INDEX];
  };
  ButtonInfo* left() {
    return &buttons[LEFT_INDEX];
  };
  ButtonInfo* right() {
    return &buttons[RIGHT_INDEX];
  };
  ButtonInfo* sleeping() {
    return &buttons[SLEEP_INDEX];
  };
  ButtonInfo* settings() {
    return &buttons[SETTINGS_INDEX];
  };
};
void ButtonController::begin() {
  if (CONTROL_BUTTON == CTL_DISABLED) return;
  dbg("ButtonController begin");
  Wire.begin (SDApin, SCLpin);
  delay(1000);
  pinMode(BUTTONS_INTERRUPT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTONS_INTERRUPT_PIN), readPortExpander, FALLING);
  Wire.requestFrom(0x20, 1);
  byte b = Wire.read();
  Serial.println(b, BIN);
  Serial.println("Port Expander read");
  
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].begin();
  }
}

void ButtonController::init() {
  if (CONTROL_BUTTON == CTL_DISABLED) return;
  dbg("ButtonController init");
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].init();
  }
  interruptCheckedTime = millis();
}

void ButtonController::operate() {
  if (CONTROL_BUTTON == CTL_DISABLED) return;
  int lastClickedButton = -1;

  // Reading from the Port expander
  unsigned long time = millis();
  if (expanderStatus && (time - interruptCheckedTime) > BUTTON_CONTROLS_DELTA_TIME) {
    Wire.requestFrom(0x20, 1);
    if (Wire.available()) {
      byte b = Wire.read();
      byte changed = EXPANDER_MASK ^ b;
      expanderStatus = LOW;
      interruptCheckedTime = time;
      if (changed > 0) {
        // Get the last clicked button from the changed bit
        lastClickedButton = log2(changed);
        buttons[lastClickedButton].click();
      }
    }
  } else {
    // Let's cycle on all the buttons
    for (int i = 0; i < NUM_BUTTONS; i++) {
      buttons[i].operate();
      if (buttons[i].isClicked()) {
        lastClickedButton = i;
      }
    }
  }
}

int log2(int x) {
  int result = 0;
  while (x >>= 1) {
    result++;
  }
  return result;
}

#endif