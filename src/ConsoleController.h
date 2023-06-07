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

// Timing variables
  unsigned long timeOfLastDebug = 0;
  unsigned long timeSleepingHasStarted = 0; 
  unsigned long timeOfLastCheck = 0;
  unsigned long timeOfLastSensorCheck = 0;

  ModeController* modeController;
  ButtonController* buttonsController;
  LedController* ledController;
  MusicController* musicController;
  StepperController* stepperController;
  DisplayController* displayController;
  ConsoleLightsController* consoleLightsController;
  int mode = WORKING_MODE;
  bool isSleeping = false;
  bool isMuted = false;
  bool readyForSleep = false;
  int sensorValue = 0;
  int previousSensorValue = 0;


public:
  ConsoleController(
    ModeController* pModeController,
    LedController* pLedController,
    MusicController* pMusicController,
    StepperController* pStepperController,
    DisplayController* pDisplayController,
    ButtonController* pButtonsController,
    ConsoleLightsController* pConsoleLightsController) {

    modeController = pModeController;
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
  bool isReadyForSleep(){return readyForSleep;};
  void settingsMode();
};

void ConsoleController::begin() {
  if (CONTROL_CONSOLE == CTL_DISABLED) return;
  dbg("ConsoleController begin");
  analogReadResolution(12);
}

void ConsoleController::init() {
  if (CONTROL_CONSOLE == CTL_DISABLED) return;
  dbg("ConsoleController init");
  int sValue = 0;
  for (int i=0; i<10;i++) {
    sValue+= analogRead(SENSOR_PIN);
    delay(5);
  }
  sensorValue = sValue / 10;
}

void ConsoleController::operate() {
  if (CONTROL_CONSOLE == CTL_DISABLED) return;
  isMuted = false;
  unsigned long time = millis();
  if (time - timeOfLastCheck < 30) {
    return;
  }
  if (time - timeOfLastSensorCheck > DELTA_TIME_SENSOR_CHECK) {
    previousSensorValue = sensorValue;
    sensorValue = analogRead(SENSOR_PIN);
    if (abs(sensorValue - previousSensorValue) > 100) {
      int brightness = map(sensorValue,0,4096,8,120);
      ledController->adjustBrightness(brightness);
      consoleLightsController->adjustBrightness(brightness);
      timeOfLastSensorCheck = time;
    }
  }
 
  

  // First let's check the mute
  ButtonInfo* muted = buttonsController->mute();
  if (muted->isClicked()) {
    dbg("ConsoleController: Mute is clicked");
    isMuted = musicController->requestMute();
    muted->reset();
    consoleLightsController->mute(!isMuted);
  }

  // Next is the settings
  ButtonInfo* settings = buttonsController->settings();
  if (settings->isLongPressed()) {
    modeController->settings();
    return;
  }
  if (settings->isClicked()) {
    dbg("ConsoleController: Setting is clicked");
    settings->reset();
    u_int8_t settings = consoleLightsController->getSettings();
    u_int8_t ledSetting;
    // Iterate on the ledSettings buttons
    for (u_int8_t i = 1; i < LED_MUSIC - LED_LIGHTS + 1; i++) {
        // Check if the current index is on
        ledSetting = settings + i;
        if (ledSetting > LED_MUSIC) {
          ledSetting = LED_LIGHTS;
        }
        if (consoleLightsController->isItOn(ledSetting)) {
          break;
        } 
    }
    if (consoleLightsController->isItOn(ledSetting)) {
      consoleLightsController->setSettings(ledSetting);
    }
  }

   // Then let's check the sleeping
  ButtonInfo* sleeping = buttonsController->sleeping();
  bool triggerSleeping = false;
  if (sleeping->isClicked()) {
    dbg("ConsoleController: Sleeping is clicked");
    sleeping->reset();
    if (CONTROL_SLEEP_MODE == ENABLED) {
      isSleeping = !isSleeping;
      modeController->sleep();
      consoleLightsController->sleeping(isSleeping);
      triggerSleeping = true;
    }
   }


  ButtonInfo* right = buttonsController->right();
  if (!isSleeping && right->isClicked()) {
    u_int8_t settings = consoleLightsController->getSettings();
    switch (settings) {
      case LED_MUSIC:
        if (!isMuted && !isSleeping) {
          int skip = musicController->computeSkip('N');
          dbg("ConsoleController: Next Song:",  skip);
          musicController->requestNewSong(skip);
          consoleLightsController->music(true);
        }
        break;
      case LED_LIGHTS:
        dbg("ConsoleController: NextSequence");
        ledController->nextSequence(+1);
        displayController->sendMessage(ledController->getNextSequenceName());
        consoleLightsController->lights(true);
        break;
      case LED_STEPPER:
        dbg("ConsoleController: Step Right");
        stepperController->speedChange('R');
        consoleLightsController->stepper(!stepperController->isItStopped());
        break;
    }
    right->reset();
  }

  ButtonInfo* left = buttonsController->left();
  if (!isSleeping && left->isClicked()) {
    u_int8_t settings = consoleLightsController->getSettings();
    switch (settings) {
      case LED_MUSIC:
        if (!isMuted && !isSleeping) {
          int skip = musicController->computeSkip('P');
          dbg("ConsoleController: Next Song:", skip);
          musicController->requestNewSong(skip);
          consoleLightsController->music(true);
        }
        break;
      case LED_LIGHTS:
        dbg("ConsoleController: prevSequence");
        ledController->nextSequence(-1);
        displayController->sendMessage(ledController->getNextSequenceName());
        consoleLightsController->lights(true);
        break;
      case LED_STEPPER:
        dbg("ConsoleController: Step Left");
        stepperController->speedChange('L');
        consoleLightsController->stepper(!stepperController->isItStopped());
        break;
    }
    left->reset();
  }

  ButtonInfo* up = buttonsController->up();
  if (!isSleeping && up->isClicked()) {
    u_int8_t settings = consoleLightsController->getSettings();
    switch (settings) {
      case LED_MUSIC:
        if (!isMuted && !isSleeping) {
          int skip = musicController->computeSkip('U');
          dbg("ConsoleController: Next Song:", skip);
          musicController->requestNewSong(skip);
          consoleLightsController->music(true);
        }
        break;
      case LED_STEPPER:
        dbg("ConsoleController: Stepper+");
        stepperController->speedChange('U');
        consoleLightsController->stepper(!stepperController->isItStopped());
        break;
      case LED_LIGHTS:
        dbg("ConsoleController: Lights+");
        ledController->speedChange('U');
        break;
    }
    up->reset();
  }

  ButtonInfo* down = buttonsController->down();
  if (!isSleeping && down->isClicked()) {
    u_int8_t settings = consoleLightsController->getSettings();
    switch (settings) {
      case LED_MUSIC:
        if (!isMuted && !isSleeping) {
          int skip = musicController->computeSkip('D');
          dbg("ConsoleController: Next Song:", skip);
          musicController->requestNewSong(skip);
          consoleLightsController->music(true);
        }
        break;
     case LED_STEPPER:
        dbg("ConsoleController: Stepper-");
        stepperController->speedChange('D');
        consoleLightsController->stepper(!stepperController->isItStopped());
        break;
     case LED_LIGHTS:
        dbg("ConsoleController: Lights-");
        ledController->speedChange('D');
        break;

    }
    down->reset();
  }


  ButtonInfo* music = buttonsController->music();
  if (!isSleeping && music->isClicked()) {
    dbg("ConsoleController: Music is clicked");
    bool doAction = true;
    bool isPlaying = musicController->isPlaying();
    if (!isMuted){
      if (!isPlaying) {
        dbg("ConsoleController: Request New Song");
        musicController->unpause();
        consoleLightsController->music(true);
        doAction = false;
      } 
    }
    if (isPlaying && doAction) {
      musicController->pause();
      consoleLightsController->music(false);
    }
    music->reset();
    
  }
 

  // Actions on stepper
  ButtonInfo* stepper = buttonsController->stepper();
  if (!isSleeping && stepper->isClicked()) {

    bool isStopped = stepperController->triggerMovement();
    stepper->reset();
    dbg("ConsoleController: Stepper is clicked!", isStopped ? "STOP" : "START");
    consoleLightsController->stepper(!isStopped);
  }


  ButtonInfo* lights = buttonsController->lights();
  if (!isSleeping && lights->isClicked()) {
    dbg("ConsoleController: Lights is clicked");
    bool isOff = ledController->isOff();
    if (isOff){
      ledController->turnItOn();
      ledController->nextSequence(+1);
    }
    else
      ledController->turnItOff();
    lights->reset();
    consoleLightsController->lights(isOff);
  }

  if (triggerSleeping) {
    dbg("ConsoleController: Forcing sleepmode");
    displayController->sleeping(isSleeping);
    musicController->sleeping(isSleeping);
    consoleLightsController->sleeping(isSleeping);
    ledController->sleeping(isSleeping);
    if (isSleeping) {
      if (!stepperController->isItStopped()) {
        stepperController->triggerMovement();
      }
      timeSleepingHasStarted = time;
      dbg("ConsoleController Sleeping time is:", timeSleepingHasStarted);
    }  
  }

  if (CURRENT_MODE > DEBUG_MODE) {
     if (time - timeOfLastDebug > 30000) {
      Serial.println("ConsoleController operate");
      timeOfLastDebug = time;
      }
   }

   if (isSleeping && (time - timeSleepingHasStarted > (SLEEP_SHUTDOWN+5000))) {
     Serial.println("ConsoleController ready for sleep");
     consoleLightsController->shutdown();
     readyForSleep = true;
   }
   timeOfLastCheck = time;
}

#endif