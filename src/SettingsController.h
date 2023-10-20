#ifndef settingsController_h
#define settingsController_h

#include <EEPROM.h>
#include <Preferences.h>
#include "SettingInfo.h"
#include "DisplayController.h"
#include "MusicController.h"
#include "LedController.h"
#include "ModeController.h"
#include "Constants.h"
#define EEPROM_SIZE 1

#define SHOW_SETTING_NAME 0
#define READ_VALUE 1
#define SHOW_VALUE 2
#define UPDATE_VALUE 3
#define STORE_VALUE 4
#define STANDBY 9

#define HELLO_MODE_TIME_INDEX 0
#define SLEEP_TIME_INDEX 1
#define NUM_SETTINGS 2

#define NOT_INITIALIZED -999

class SettingsController {
private:
  int currentSetting = -1;
  int phase = DO_NOTHING;
  int intValue = 0;
  unsigned long lastTimeSettingCheck;
  int first = 0;
  unsigned long standbyTime = 0;
  unsigned long lastCheckStanbyTime = 0;
  int nextPhase = DO_NOTHING;
  SettingInfo settings[NUM_SETTINGS];
public:
  void operate();
  void init();
  void standby(int standbyTime, int iNextPhase);
  void begin(DisplayController* pDisplayController, MusicController* pMusicController, LedController* pLedController);
  DisplayController* displayController;
  MusicController* musicController;
  LedController* ledController;
  Preferences preferences;
  SettingsController() {
    settings[HELLO_MODE_TIME_INDEX] = SettingInfo(NOT_INITIALIZED, "Hello T(s)", "hellotime", 1000);
    settings[SLEEP_TIME_INDEX] = SettingInfo(NOT_INITIALIZED, "Sleep T(s)", "sleeptime", 1000);
  };
  void saveValue();
  void changeSetting(int direction);
  void changeValue(int change);
  void setSleepToControllers();
  void updateValue(int value);
  //const char* getSetting(int i) {return settingNames[i];}
  
  SettingInfo* helloSettings() {
      return &settings[HELLO_MODE_TIME_INDEX];
    };

  SettingInfo* sleepSettings() {
      return &settings[SLEEP_TIME_INDEX];
    };
};

void SettingsController::changeSetting(int direction){
  currentSetting+=direction;
  if (currentSetting >= NUM_SETTINGS & direction > 0) currentSetting = 0;
  if (currentSetting < 0 & direction < 0) currentSetting = NUM_SETTINGS - 1;
  standby(1000, SHOW_SETTING_NAME);
};

void SettingsController::saveValue(){
  standby(100, STORE_VALUE);
};

void SettingsController::updateValue(int value){
    switch (currentSetting) {
      case HELLO_MODE_TIME_INDEX:
        displayController->setHelloModeTime(value);
        break;
    }
  };

void SettingsController::standby(int iStandbyTime, int iNextPhase){
  phase = STANDBY;
  standbyTime = iStandbyTime;
  nextPhase = iNextPhase;
  lastCheckStanbyTime = millis();
};

void SettingsController::changeValue(int change){
  intValue+=change;
  standby(300, SHOW_VALUE);
};

void SettingsController::begin(DisplayController* pDisplayController, MusicController* pMusicController, LedController* pLedController){
  displayController = pDisplayController;
  musicController = pMusicController;
  ledController = pLedController;

  lastTimeSettingCheck = 0;
  preferences.begin("wheel", false);
  SettingInfo* hello = helloSettings();
  hello->reload(&preferences);
  displayController->setHelloModeTime(hello->getMachineValue());
  setSleepToControllers();
  
};

void SettingsController::setSleepToControllers(){
  SettingInfo* sleep = sleepSettings();
  sleep->reload(&preferences);
  int sleepShutdownTime = sleep->getMachineValue();
  displayController->setSleepShutdownTime(sleepShutdownTime);
  musicController->setSleepShutdownTime(sleepShutdownTime);
  ledController->setSleepShutdownTime(sleepShutdownTime);
};

void SettingsController::init(){
  currentSetting = 0;
  lastTimeSettingCheck = millis();
  standby(3000, SHOW_SETTING_NAME);
};

void SettingsController::operate(){
  unsigned long time = millis();
  if (time - lastTimeSettingCheck < 50) {
    return;
  }
  SettingInfo setting = settings[currentSetting];
  switch (phase) {
    case DO_NOTHING:
      break;
    case STANDBY:
      if (time - lastCheckStanbyTime > standbyTime) {
        phase = nextPhase;
      }
      break;
    case SHOW_SETTING_NAME:
      //displayController->sendSetting(setting.getName(), 1, true, 2000, SETTINGS_MODE);
      displayController->displayNames(setting.getName(), 2000, REMOTE_MODE);
      standby(2000, READ_VALUE);
      break;
    case READ_VALUE:
      intValue = setting.getValue();
      if (intValue == NOT_INITIALIZED){
        intValue = settings->reload(& preferences);
        updateValue(setting.getMachineValue());
      }
      standby(400, SHOW_VALUE);
      break;
    case SHOW_VALUE:
      //displayController->sendSetting(toText(intValue), 1, false, 2000, SETTINGS_MODE);
      displayController->displayNames(toText(intValue), 2000, REMOTE_MODE);
      //displayController->displayMessage(toText(intValue), 2, false);
      standby(200, UPDATE_VALUE);
      break;
    case UPDATE_VALUE:
      standby(1000, UPDATE_VALUE);
      break;
    case STORE_VALUE:
      setting.store(&preferences, intValue);
      updateValue(setting.getMachineValue());
      standby(1000, UPDATE_VALUE);
      break;

  }
  lastTimeSettingCheck = time;
};

#endif
