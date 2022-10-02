#include "Arduino.h"
#ifndef CommandsController_h
#define CommandsController_h


#define COLOR_SWITCH_PIN 2
#define SONG_SWITCH_PIN 3
#define SEQUENCE_SWITCH_PIN 4
#define LONG_PRESS_TIME 300
#define SHORT_PRESS_TIME 80

class ButtonStatus {
  //int currentState;
  //int lastState = LOW;
  unsigned long lastCheckTime;
  const int pin;  
  const String name;
  bool state;
  public:
  ButtonStatus(const String sName, const int iPin):pin(iPin), name(sName){};
  void operate();
  bool getState(){return state;}
  bool setState(bool state) {this->state = state;}
  
};

void ButtonStatus::operate() {
 long time = millis();
 int colorSwitch = digitalRead(COLOR_SWITCH_PIN);
 if (colorSwitch == HIGH) {
    if ((time - lastCheckTime) > SHORT_PRESS_TIME) {
        setState(true);
        lastCheckTime = time;
    }
  }
}

class CommandsController {
  private:
    ButtonStatus* colorStatus;
    ButtonStatus* musicStatus;
    ButtonStatus* sequenceStatus;

  public:
    CommandsController(){};
  void init();
  void begin();
  void operate();
  bool isNextSongRequested();
  bool isColorChanged();
  bool isSequenceChanged();
  void setColorChanged(bool isChanged);
  void setSequenceChanged(bool isChanged);
  void setNextSongRequested(bool setNext);
};

void CommandsController::begin() {
  Serial.println("ButtonController begin");
  pinMode(COLOR_SWITCH_PIN, INPUT);
  //pinMode(SEQUENCE_SWITCH_PIN, INPUT);
  //pinMode(SONG_SWITCH_PIN, INPUT);
}

void CommandsController::init(){
  colorStatus = new ButtonStatus("colors", COLOR_SWITCH_PIN);
  sequenceStatus = new ButtonStatus("sequence", SEQUENCE_SWITCH_PIN);
  musicStatus = new ButtonStatus("music", SONG_SWITCH_PIN);
  
}

bool CommandsController::isNextSongRequested(){
  return musicStatus->getState();
}

void CommandsController::setNextSongRequested(bool setNext){
  musicStatus->setState(setNext);
}

bool CommandsController::isColorChanged(){
  return colorStatus->getState();
}

void CommandsController::setColorChanged(bool isChanged) {
  colorStatus->setState(isChanged);
}

bool CommandsController::isSequenceChanged(){
  return sequenceStatus->getState();
}

void CommandsController::setSequenceChanged(bool isChanged) {
  sequenceStatus->setState(isChanged);
}

void CommandsController::operate(){
    colorStatus->operate();
    //musicStatus->operate();
    //sequenceStatus->operate();
}


#endif
