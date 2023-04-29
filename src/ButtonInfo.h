#ifndef ButtonInfo_h
#define ButtonInfo_h

/*
Each Button can be associated to a ButtonInfo instance
*/
class ButtonInfo {
private:
  int buttonPin;                          // The PIN associated to the button
  unsigned long lastTimeOnButtonControl;  // The millis() of the last time the button state has been taken into account
  bool pressed;                           // True if the button has been long pressed
  bool clicked = false;                   // True if the button has been clicked
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

  bool isPressed() {
    return pressed;
  };

  bool isClicked() {
    return clicked;
  };

  void click() {
    dbg("Button Clicked: ", name);
    clicked = true;
  };

  void press() {
    pressed = true;
  };

  void reset() {
    // Reset the status of the button so it results not clicked anymore
    clicked = false;
    dbg("Button Reset: ", name);
  };
};

ButtonInfo::ButtonInfo(int iPin, char cName, bool bStandalone)
  : buttonPin(iPin), name(cName), standalone(bStandalone){};

void ButtonInfo::operate() {
  if (standalone) {
    
    long time = millis();
    // Read the status of the button
    int button = digitalRead(buttonPin);
    if (button == BUTTON_PRESSED) {
      if ((time - lastTimeOnButtonControl) > BUTTON_CONTROLS_DELTA_TIME) {
        // Set the button to clicked and set the time
        dbg("Button Clicked:", name);
        clicked = true;
        lastTimeOnButtonControl = time;
      }
    }
  }
};

void ButtonInfo::debug() {
  Serial.print("Button:");
  Serial.print(name);
  Serial.print(" clicked:");
  Serial.print(clicked);
  Serial.print(" pressed:");
  Serial.println(pressed);
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