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
    StepperController *stepperController;
    DisplayController *displayController;
    ConsoleLightsController* consoleLightsController;
    bool isSleeping;

  public:
    ConsoleController(
      LedController* pLedController,
      MusicController* pMusicController, 
      StepperController* pStepperController, 
      DisplayController* pDisplayController, 
      ButtonController* pButtonsController,
      ConsoleLightsController* pConsoleLightsController){
      
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
}

void ConsoleController::init() {
}

void ConsoleController::operate() {
  bool isMuted = false;
  
  ButtonInfo* muted = buttonsController->mute();
  if (muted->isClicked()) {
    isMuted = musicController->triggerMute();
    muted->reset();
    consoleLightsController -> mute(isMuted);
  }

  ButtonInfo* sleeping = buttonsController->sleeping();
  if (sleeping->isClicked()) {
    isSleeping = !isSleeping;
    consoleLightsController->sleeping(isSleeping);
    sleeping->reset();
  }
/*
  ButtoInfo* music = buttonsController->music();
  if (music->isClicked()) {
    musicController->nextSong();
    music->reset();
    displayController->nextSong();
  }
*/
  ButtonInfo* stepper = buttonsController->stepper();
  if (stepper->isClicked()) {
    bool isStopped = stepperController -> triggerMovement();
    stepper->reset();
    consoleLightsController -> stepper(isStopped);
  }


  ButtonInfo* lights = buttonsController->lights();
  if (lights->isClicked()) {
    ledController->nextSequence();
    lights->reset();
    consoleLightsController -> lights(isMuted);
  }


 
  
}

#endif