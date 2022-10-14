#ifndef ConsoleController_h
#define ConsoleController_h

#include "Constants.h"
#include "ButtonController.h"
#include "LedController.h"
#include "MusicController.h"
#include "StepperController.h"
#include "DisplayController.h"
#include "ConsoleLedController.h"



class ConsoleController {
  private:
    ButtonController* buttonsController;
    LedController* ledController;
    MusicController* musicController;
    StepperController *stepperController;
    DisplayController *displayController;
    ConsoleLedController* consoleLedController;

  public:
    ConsoleController(
      LedController* pLedController,
      ConsoleLedController* pConsoleLedController, 
      MusicController* pMusicController, 
      StepperController* pStepperController, 
      DisplayController* pDisplayController, 
      ButtonController* pButtonsController){
      
      ledController = pLedController;
      buttonsController = pButtonsController;
      consoleLedController = pConsoleLedController;
      musicController = pMusicController;
      stepperController = pStepperController;
      displayController = pDisplayController;
      };
  void init();  
  void begin();
  void operate();
};


#endif ConsoleController_h