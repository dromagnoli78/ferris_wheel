#ifndef ButtonInfo_h
#define ButtonInfo_h

/*
Each Button can be associated to a ButtonInfo instance class
*/
class ButtonInfo {
private:
  int buttonPin;                 // The PIN associated to the button
  unsigned long lastTimeOnButtonControl;  // The millis() of the last time the button state has been taken into account
  bool pressed;                  // True if the button has been long pressed
  bool clicked = false;                  // True if the button has been clicked
  char name;

public:
  ButtonInfo()
    : buttonPin(1){};
  ButtonInfo(int iPin, char cName);
  void operate();
  void debug();
  void init(){
     if (CURRENT_MODE == DEBUG_MODE) {
      Serial.print("Button Init:"),
        Serial.print(buttonPin);
      Serial.print(": ");
      Serial.println(name);
    }
    lastTimeOnButtonControl = millis();
  }
;
  void begin() {
    pinMode(buttonPin, INPUT);
    
    if (CURRENT_MODE == DEBUG_MODE) {
      Serial.print("Button setup:"),
        Serial.print(buttonPin);
      Serial.print(": ");
      Serial.println(name);
    }
  };

  bool isPressed() {
    return pressed;
  };

  bool isClicked() {
    return clicked;
  };

  void reset() {
    // Reset the status of the button so it results not clicked anymore
    clicked = false;
    if (CURRENT_MODE == DEBUG_MODE) {
      Serial.print("Button Reset:");
      Serial.println(name);
    }
  };
};

ButtonInfo::ButtonInfo(int iPin, char cName)
  : buttonPin(iPin), name(cName){};

void ButtonInfo::operate() {
  long time = millis();
  // Read the status of the button
  int button = digitalRead(buttonPin);
  if (button == BUTTON_PRESSED) {
    if ((time - lastTimeOnButtonControl) > BUTTON_CONTROLS_DELTA_TIME) {
      // Set the button to clicked and set the time
      if (CURRENT_MODE == DEBUG_MODE) {
        Serial.print("Button Clicked:");
        Serial.println(name);
      }
      clicked = true;
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
  Serial.println(pressed);
};


#endif