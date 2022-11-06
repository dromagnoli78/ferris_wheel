#ifndef ConsoleController_h
#define ConsoleController_h

#include "Constants.h"
#include "ButtonController.h"
#include "ButtonInfo.h"
#include "LedController.h"
#include "MusicController.h"
#include "StepperController.h"
#include "DisplayController.h"
#include "ConsoleLightsController.h"



class ConsoleController {
private:
  ButtonController* buttonsController;
  LedController* ledController;
  MusicController* musicController;
  StepperController* stepperController;
  DisplayController* displayController;
  ConsoleLightsController* consoleLightsController;
  bool isSleeping = false;
  bool isMuted = false;
  long debugTime = 0;

public:
  ConsoleController(
    LedController* pLedController,
    MusicController* pMusicController,
    StepperController* pStepperController,
    DisplayController* pDisplayController,
    ButtonController* pButtonsController,
    ConsoleLightsController* pConsoleLightsController) {

    ledController = pLedController;
    buttonsController = pButtonsController;
    musicController = pMusicController;
    stepperController = pStepperController;
    displayController = pDisplayController;
    consoleLightsController = pConsoleLightsController;
  };
  void init();
  void begin();
  void operate();
};

void ConsoleController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ConsoleController begin");
}

void ConsoleController::init() {
    if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("ConsoleController init");
}

void ConsoleController::operate() {
  isMuted = false;
  long time = millis();
  
/*
  // First let's check the mute
  ButtonInfo muted = buttonsController->mute();
  if (muted.isClicked()) {
    if (CURRENT_MODE == DEBUG_MODE) Serial.println("ConsoleController: Mute is clicked");
    isMuted = musicController->triggerMute();
    muted.reset();
    consoleLightsController->mute(isMuted);
    if (!isMuted && musicController->isPlaying()) {
      consoleLightsController->music();
    }
  }
  */

  // Then let's check the sleeping
  ButtonInfo* sleeping = buttonsController->sleeping();
  bool triggerSleeping = false;
  if (sleeping->isClicked()) {
    if (CURRENT_MODE == DEBUG_MODE) Serial.println("ConsoleController: Sleeping is clicked");
    isSleeping = !isSleeping;
    sleeping->reset();
    consoleLightsController->sleeping(isSleeping);
    triggerSleeping = true;
    
  }


  ButtonInfo* music = buttonsController->music();
  if (!isMuted && !isSleeping && music->isClicked()) {
    if (CURRENT_MODE == DEBUG_MODE) Serial.println("ConsoleController: Music is clicked");
    int songIndex = musicController->nextSong();
    music->reset();
    consoleLightsController->music();
  }
 

  ButtonInfo* stepper = buttonsController->stepper();
  if (!isSleeping && stepper->isClicked()) {

    bool isStopped = stepperController->triggerMovement();
    stepper->reset();
    if (CURRENT_MODE == DEBUG_MODE) {
      Serial.print("ConsoleController: Stepper is clicked!");
      Serial.print("Is stopped:");
      Serial.println(isStopped);
    }

    consoleLightsController->stepper(isStopped);
  }


  ButtonInfo* lights = buttonsController->lights();
  if (!isSleeping && lights->isClicked()) {
    if (CURRENT_MODE == DEBUG_MODE) Serial.println("ConsoleController: Lights is clicked");
    ledController->nextSequence();
    lights->reset();
    consoleLightsController->lights(true);
  }

  if (triggerSleeping) {
    if (CURRENT_MODE == DEBUG_MODE) Serial.println("ConsoleController: Forcing sleepmode");
    displayController->sleeping(isSleeping);
    musicController->sleeping(isSleeping);
    consoleLightsController->sleeping(isSleeping);
  }

  if (CURRENT_MODE == DEBUG_MODE) {
     if (time - debugTime > 20000) {
      Serial.println("ConsoleController operate");
      //stepper->debug();
      //music->debug();
      debugTime = time;
      }
   }

}

#endif