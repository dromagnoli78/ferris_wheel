#ifndef ButtonInfo_h
#define ButtonInfo_h

#include "ModeController.h"

/*
Each Button can be associated to a ButtonInfo instance
*/
class ButtonInfo {
private:
  int buttonPin;                          // The PIN associated to the button
  unsigned long lastTimeOnButtonControl;  // The millis() of the last time the button state has been taken into account
  unsigned long buttonStartTime = 0;
  bool clicked = false;                   // True if the button has been clicked
  bool pressed = false;                   // True if the button has been simply pressed
  bool longPressed;                       // True if the button has been long pressed
  char name;                              // Signature of the button for quick reference during debug
  bool standalone = false;

public:
  ButtonInfo()
    : buttonPin(1){};
  ButtonInfo(int iPin, char cName, bool bStandalone);
  void operate();
  void debug();
  void describeMessage(char b);
  void init() {
    describeMessage('I');
    lastTimeOnButtonControl = millis();
  };
  void begin() {
    if (standalone) {
      pinMode(buttonPin, INPUT);
    }
    describeMessage('B');
  };

  bool isLongPressed() {
    return longPressed;
  };

  bool isClicked() {
    return clicked;
  };

  void click() {
    dbg("Button Clicked: ", name);
    clicked = true;
  };

  void press() {
    longPressed = true;
  };

  void reset() {
    // Reset the status of the button so it results not clicked anymore
    clicked = false;
    pressed = false;
    longPressed = false;
    dbg("Button Reset: ", name);
  };
};

ButtonInfo::ButtonInfo(int iPin, char cName, bool bStandalone)
  : buttonPin(iPin), name(cName), standalone(bStandalone){};

void ButtonInfo::operate() {
  if (standalone) {
    
    long time = millis();
    // Read the status of the button
    if ((time - lastTimeOnButtonControl) > BUTTON_CONTROLS_DELTA_TIME) {
      int button = digitalRead(buttonPin);
      if (button == BUTTON_PRESSED && !pressed) {
          // Set the button to clicked and set the time
          dbg("Button Pressed:", name);
          pressed = true;
          buttonStartTime = time;
      }
      if (button != BUTTON_PRESSED && pressed) {
        unsigned long buttonReleaseTime = millis();
        unsigned long buttonPressDuration = buttonReleaseTime - buttonStartTime;
        if (buttonPressDuration < 50) {
          // Ignore shorter button presses as noise
          return;
        }
        if (buttonPressDuration >= BUTTON_LONG_PRESSED_DELTA_TIME) {
          longPressed = true;
        } else {
          clicked = true;
        }
        pressed = false;
      }
      lastTimeOnButtonControl = time;
    }
  }
};

void ButtonInfo::debug() {
  Serial.print("Button:");
  Serial.print(name);
  Serial.print(" clicked:");
  Serial.print(clicked);
  Serial.print(" pressed:");
  Serial.println(longPressed);
};

void ButtonInfo::describeMessage(char c) {
  if (CURRENT_MODE > DEBUG_MODE) {
    Serial.print("Button ");
    switch (c) {
      case 'I':
        Serial.print("Initialization");
        break;
      case 'B':
        Serial.print("Begin");
        break;
    }
    Serial.print(" ");
    Serial.print(buttonPin);
    Serial.print(":");
    Serial.print(name);
    Serial.print(" Standalone:");
    Serial.println(standalone);
  }
};
#endif