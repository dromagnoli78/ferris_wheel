#ifndef settingsController_h
#define settingsController_h

#include <EEPROM.h>
#include <Preferences.h>
#include "SettingInfo.h"
#include "DisplayController.h"
#include "MusicController.h"
#include "LedController.h"
#include "ModeController.h"
#include "ConsoleLightsController.h"
#include "Constants.h"
#define EEPROM_SIZE 1

#define SHOW_SETTING_NAME 0
#define READ_VALUE 1
#define SHOW_VALUE 2
#define UPDATE_VALUE 3
#define STORE_VALUE 4
#define STANDBY 9

#define SETTING_HELLO_MODE_TIME_IDX 0
#define SETTING_SLEEP_TIME_IDX 1
#define SETTING_SLEEP_MIN_LIGHT_IDX 2
#define SETTING_SLEEP_MAX_LIGHT_IDX 3
#define SETTING_LIGHTS_MIN_DELAY_IDX 4
#define SETTING_LEDS_INITIAL_BRIGHTNESS_IDX 5
#define SETTING_LEDS_MIN_BRIGHTNESS_IDX 6
#define SETTING_LEDS_MAX_BRIGHTNESS_IDX 7
#define NUM_SETTINGS 8


class SettingsController {
private:
  int currentSetting = -1;
  int phase = DO_NOTHING;
  unsigned int intValue = 0;
  unsigned long lastTimeSettingCheck;
  int first = 0;
  unsigned long standbyTime = 0;
  unsigned long lastCheckStanbyTime = 0;
  int nextPhase = DO_NOTHING;
  SettingInfo settings[NUM_SETTINGS];
  DisplayController* displayController;
  MusicController* musicController;
  LedController* ledController;
  ConsoleLightsController* consoleLightsController;
  
public:
  void operate();
  void init();
  void standby(int standbyTime, int iNextPhase);
  void begin(DisplayController* pDisplayController, MusicController* pMusicController, 
    LedController* pLedController, ConsoleLightsController* pConsoleLightsController);
 Preferences preferences;

  
  SettingsController() {
    settings[SETTING_HELLO_MODE_TIME_IDX] = SettingInfo(NOT_INITIALIZED, DELAY_AFTER_HELLO, "Hello T[s]", "hellotime", 1000U);
    settings[SETTING_SLEEP_TIME_IDX] = SettingInfo(NOT_INITIALIZED, SLEEP_SHUTDOWN_TIME, "Sleep T[s]", "sleeptime", 1000U);
    settings[SETTING_SLEEP_MIN_LIGHT_IDX] = SettingInfo(NOT_INITIALIZED, MIN_VALUE_LIGHT_FOR_SLEEP, "Sleep minLIT", "sleepminlight", 1U);
    settings[SETTING_SLEEP_MAX_LIGHT_IDX] = SettingInfo(NOT_INITIALIZED, MAX_VALUE_LIGHT_FOR_SLEEP, "Sleep maxLIT", "sleepmaxlight", 1U);
    settings[SETTING_LIGHTS_MIN_DELAY_IDX] = SettingInfo(NOT_INITIALIZED, LIGHTS_DELAY_MIN, "Led MinDLY", "lightsmindelay", 1U);
    settings[SETTING_LEDS_INITIAL_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_INITIAL_BRIGHTNESS, "Led initLIT", "ledstartlit", 1U);
    settings[SETTING_LEDS_MIN_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_MIN_BRIGHTNESS, "Led minLIT", "ledminlit", 1U);
    settings[SETTING_LEDS_MAX_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_MAX_BRIGHTNESS, "Led maxLIT", "ledmaxlit", 1U);
  };
  void saveValue();
  void changeSetting(int direction);
  void changeValue(int change);
  SettingInfo* prepareSettings(boolean reload, int settingIndex);
  void notifySleepShutdownTime(boolean reload);
  void notifyHelloTime(boolean reload);
  void notifyUpdates();
  void notifySleepMaxLight(boolean reload);
  void notifySleepMinLight(boolean reload);
  void notifyLightsMinDelay(boolean reload);
  void notifyLedInitialBrightness(boolean reload);
  void notifyLedMinBrightness(boolean reload);
  void notifyLedMaxBrightness(boolean reload);
 };

void SettingsController::changeSetting(int direction){
  currentSetting+=direction;
  if (currentSetting >= NUM_SETTINGS & direction > 0) currentSetting = 0;
  if (currentSetting < 0 & direction < 0) currentSetting = NUM_SETTINGS - 1;
  intValue = settings[currentSetting].reload(&preferences);
  standby(1000, SHOW_SETTING_NAME);
};

void SettingsController::saveValue(){
  standby(100, STORE_VALUE);
};

void SettingsController::notifyUpdates(){
    switch (currentSetting) {
      case SETTING_HELLO_MODE_TIME_IDX:
        notifyHelloTime(false);
        break;
      case SETTING_SLEEP_TIME_IDX:
        notifySleepShutdownTime(false);
        break;
      case SETTING_SLEEP_MAX_LIGHT_IDX:
        notifySleepMaxLight(false);
        break;
      case SETTING_SLEEP_MIN_LIGHT_IDX:
        notifySleepMinLight(false);
        break;
      case SETTING_LIGHTS_MIN_DELAY_IDX:
        notifyLightsMinDelay(false);
        break;
      case SETTING_LEDS_INITIAL_BRIGHTNESS_IDX:
        notifyLedInitialBrightness(false);
        break;
      case SETTING_LEDS_MIN_BRIGHTNESS_IDX:
        notifyLedMinBrightness(false);
        break;
      case SETTING_LEDS_MAX_BRIGHTNESS_IDX:
        notifyLedMaxBrightness(false);
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

void SettingsController::begin(DisplayController* pDisplayController, MusicController* pMusicController, 
    LedController* pLedController, ConsoleLightsController* pConsoleLightsController){
  displayController = pDisplayController;
  musicController = pMusicController;
  ledController = pLedController;
  consoleLightsController = pConsoleLightsController;

  lastTimeSettingCheck = 0;
  preferences.begin("wheel", false);


  notifyHelloTime(true);
  notifySleepShutdownTime(true);
  notifySleepMaxLight(true);
  notifySleepMinLight(true);
  notifyLightsMinDelay(true);
  notifyLedInitialBrightness(true);
  notifyLedMinBrightness(true);
  notifyLedMaxBrightness(true);
  
};

void SettingsController::init(){
  currentSetting = 0;
  lastTimeSettingCheck = millis();
  standby(3000, SHOW_SETTING_NAME);
  



/*
    displayController->displayNames("----INIT---", 2, 2000, REMOTE_MODE);
    delay(1000);
    preferences.putUInt("hellotime", 4);
    preferences.putUInt("sleeptime", 80);
    preferences.putUInt("sleepminlight", 10);
    preferences.putUInt("sleepmaxlight", 20);
    displayController->displayNames("----DONE---", 2, 2000, REMOTE_MODE);
    delay(1000);

    displayController->displayNames("----READBACK---", 2, 2000, REMOTE_MODE);

    delay(1000);
    displayController->displayNames("hellotime", 1, 2000, REMOTE_MODE);
    delay(1000);
    int v = preferences.getUInt("hellotime",0U);
    displayController->displayNames(toText(v), 1, 2000, REMOTE_MODE);
    delay(1000);
    displayController->displayNames("sleeptime", 1, 2000, REMOTE_MODE);
    delay(1000);
    v = preferences.getUInt("sleeptime",0U);
    displayController->displayNames(toText(v), 1, 2000, REMOTE_MODE);
    delay(1000);
    displayController->displayNames("sleepminlight", 1, 2000, REMOTE_MODE);
    delay(1000);
    v = preferences.getUInt("sleepminlight",0U);
    displayController->displayNames(toText(v), 1, 2000, REMOTE_MODE);
    delay(1000);
    displayController->displayNames("sleepmaxlight", 1, 2000, REMOTE_MODE);
    delay(1000);
    v = preferences.getUInt("sleepmaxlight",0U);
    displayController->displayNames(toText(v), 1, 2000, REMOTE_MODE);
    delay(1000);
    displayController->displayNames("----DONE---", 2, 2000, REMOTE_MODE);
    delay(1000);
*/
  

};

SettingInfo* SettingsController::prepareSettings(boolean reload, int settingIndex) {
  SettingInfo* setting = &settings[settingIndex];
  if (reload) setting->reload(&preferences);
  return setting;
}

void SettingsController::notifyHelloTime(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_HELLO_MODE_TIME_IDX);
  displayController->updateHelloModeTime(setting->getMachineValue());
}

void SettingsController::notifyLightsMinDelay(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_LIGHTS_MIN_DELAY_IDX);
  ledController->updateLightsMinDelay(setting->getMachineValue());
}

void SettingsController::notifySleepShutdownTime(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_SLEEP_TIME_IDX);
  int sleepShutdownTime = setting->getMachineValue();
  displayController->updateSleepShutdownTime(sleepShutdownTime);
  musicController->updateSleepShutdownTime(sleepShutdownTime);
  ledController->updateSleepShutdownTime(sleepShutdownTime);
};

void SettingsController::notifyLedInitialBrightness(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_LEDS_INITIAL_BRIGHTNESS_IDX);
  int brightness = setting->getMachineValue();
  ledController->updateLigthsInitialBrightness(brightness);
  consoleLightsController->updateLigthsInitialBrightness(brightness);
};

void SettingsController::notifyLedMinBrightness(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_LEDS_MIN_BRIGHTNESS_IDX);
  int brightness = setting->getMachineValue();
  ledController->updateLedMinBrightness(brightness);
  consoleLightsController->updateLedMinBrightness(brightness);
};

void SettingsController::notifyLedMaxBrightness(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_LEDS_MAX_BRIGHTNESS_IDX);
  int brightness = setting->getMachineValue();
  ledController->updateLedMaxBrightness(brightness);
  consoleLightsController->updateLedMaxBrightness(brightness);
};

void SettingsController::notifySleepMaxLight(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_SLEEP_MAX_LIGHT_IDX);
  ledController->setSleepMaxLight(setting->getMachineValue());
}

void SettingsController::notifySleepMinLight(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_SLEEP_MIN_LIGHT_IDX);
  ledController->setSleepMinLight(setting->getMachineValue());
}



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
      displayController->displayNames(setting.getName(), 2, 2000, REMOTE_MODE);
      standby(2000, READ_VALUE);
      break;
    case READ_VALUE:
      intValue = setting.reload(& preferences);
      standby(400, SHOW_VALUE);
      break;
    case SHOW_VALUE:
      displayController->displayNames(toText(intValue), 1, 2000, REMOTE_MODE);
      standby(200, UPDATE_VALUE);
      break;
    case UPDATE_VALUE:
      standby(1000, UPDATE_VALUE);
      break;
    case STORE_VALUE:
      setting.store(&preferences, intValue);
      notifyUpdates();
      standby(1000, UPDATE_VALUE);
      break;

  }
  lastTimeSettingCheck = time;
};

#endif
