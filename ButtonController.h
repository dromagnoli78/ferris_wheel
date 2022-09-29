#include "Arduino.h"
#ifndef ButtonController_h
#define ButtonController_h


#define COLOR_SWITCH_PIN 2


class ButtonController {
  private:
    // const int variousPins;
    bool colorChanged = true;
    long deltaTimeOnColorSwitch;
    long lastTimeOnColorSwitchCheck;
  public:
    ButtonController(){
    };
  void init();
  void begin();
  void operate();
  bool isNextSongRequested();
  bool isColorChanged();
  void setColorChanged(bool);
  void setNextSongRequested(int);
};



void ButtonController::begin() {
  Serial.println("ButtonController begin");
  pinMode(COLOR_SWITCH_PIN, INPUT);
}

void ButtonController::init(){
  deltaTimeOnColorSwitch = 200;
  long time = millis();
  lastTimeOnColorSwitchCheck = time;
  
}

bool ButtonController::isNextSongRequested(){
  return true;
}

void ButtonController::setNextSongRequested(int i){
  ;
}

bool ButtonController::isColorChanged(){
  return colorChanged;
}

void ButtonController::setColorChanged(bool isChanged) {
  colorChanged = isChanged;
}

void ButtonController::operate(){
  long time = millis();
  int colorSwitch = digitalRead(COLOR_SWITCH_PIN);
  if (colorSwitch == HIGH) {
    if ((time - lastTimeOnColorSwitchCheck) > deltaTimeOnColorSwitch) {
        setColorChanged(true);
        lastTimeOnColorSwitchCheck = time;
    }
  }
  
}


#endif
