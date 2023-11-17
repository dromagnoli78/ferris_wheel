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
#define UPDATE_LABEL 10
#define SHOW_LABEL 11
#define STANDBY 9

#define SETTING_HELLO_MODE_TIME_IDX 0
#define SETTING_SLEEP_TIME_IDX 1
#define SETTING_SLEEP_MIN_LIGHT_IDX 2
#define SETTING_SLEEP_MAX_LIGHT_IDX 3
#define SETTING_LIGHTS_MIN_DELAY_IDX 4
#define SETTING_LEDS_INITIAL_BRIGHTNESS_IDX 5
#define SETTING_LEDS_MIN_BRIGHTNESS_IDX 6
#define SETTING_LEDS_MAX_BRIGHTNESS_IDX 7
#define SETTING_CLEDS_MIN_BRIGHTNESS_IDX 8
#define SETTING_CLEDS_MAX_BRIGHTNESS_IDX 9
#define SETTING_EQUALIZER_IDX 10
#define SETTING_RANDOM_IDX 11
#define SETTING_CLOCK_HOUR_IDX 12
#define SETTING_CLOCK_MINUTE_IDX 13
#define NUM_SETTINGS 14

const char* EQUALIZERS[]={
  "NORMAL",
  "POP",
  "ROCK",
  "JAZZ",
  "CLASSIC",
  "BASS",
};


#define DFPLAYER_EQ_NORMAL 0
#define DFPLAYER_EQ_POP 1
#define DFPLAYER_EQ_ROCK 2
#define DFPLAYER_EQ_JAZZ 3
#define DFPLAYER_EQ_CLASSIC 4
#define DFPLAYER_EQ_BASS 5


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
  int showType = SHOW_VALUE;
  int sleepShutdownTime;
  const char* labelValue = "";
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
    settings[SETTING_HELLO_MODE_TIME_IDX] = SettingInfo(NOT_INITIALIZED, DELAY_AFTER_HELLO, "Hello T[s]", "hellotime", 1000U, 1, 0, nullptr);
    settings[SETTING_SLEEP_TIME_IDX] = SettingInfo(NOT_INITIALIZED, SLEEP_SHUTDOWN_TIME, "Sleep T[s]", "sleeptime", 1000U, 10, 0, nullptr);
    settings[SETTING_SLEEP_MIN_LIGHT_IDX] = SettingInfo(NOT_INITIALIZED, MIN_VALUE_LIGHT_FOR_SLEEP, "Sleep minLIT", "sleepminlight", 1U, 1, 0, nullptr);
    settings[SETTING_SLEEP_MAX_LIGHT_IDX] = SettingInfo(NOT_INITIALIZED, MAX_VALUE_LIGHT_FOR_SLEEP, "Sleep maxLIT", "sleepmaxlight", 1U, 1, 0, nullptr);
    settings[SETTING_LIGHTS_MIN_DELAY_IDX] = SettingInfo(NOT_INITIALIZED, LIGHTS_DELAY_MIN, "Led MinDLY", "lightsmindelay", 1U, 10, 0, nullptr);
    settings[SETTING_LEDS_INITIAL_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_INITIAL_BRIGHTNESS, "Led initLIT", "ledstartlit", 1U, 1, 0, nullptr);
    settings[SETTING_LEDS_MIN_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_MIN_BRIGHTNESS, "Led minLIT", "ledminlit", 1U, 1, 0, nullptr);
    settings[SETTING_LEDS_MAX_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_MAX_BRIGHTNESS, "Led maxLIT", "ledmaxlit", 1U, 1, 0, nullptr);
    settings[SETTING_CLEDS_MIN_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_MIN_BRIGHTNESS, "CLed minLIT", "cledminlit", 1U, 1, 0, nullptr);
    settings[SETTING_CLEDS_MAX_BRIGHTNESS_IDX] = SettingInfo(NOT_INITIALIZED, LED_MAX_BRIGHTNESS, "CLed maxLIT", "cledmaxlit", 1U, 1, 0, nullptr);
    settings[SETTING_EQUALIZER_IDX] = SettingInfo(NOT_INITIALIZED, EQUALIZER, "Equalizer", "equalizer", 1U, 1, 6, EQUALIZERS);
    settings[SETTING_RANDOM_IDX] = SettingInfo(NOT_INITIALIZED, RANDOM_PATTERN_DURATION, "Rnd T[s]", "randomt", 1U, 10, 0, nullptr);
    settings[SETTING_CLOCK_HOUR_IDX] = SettingInfo(NOT_INITIALIZED, 1, "hour", "clockh", 1U, 1, 0, nullptr);
    settings[SETTING_CLOCK_MINUTE_IDX] = SettingInfo(NOT_INITIALIZED, 60, "minute", "clockm", 1U, 1, 0, nullptr);

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
  void notifyCLedMinBrightness(boolean reload);
  void notifyCLedMaxBrightness(boolean reload);
  void notifyLedRandomDuration(boolean reload);
  void notifyEqualizer(boolean reload);

  int getSleepShutdownTime(){return sleepShutdownTime;};
 };

void SettingsController::changeSetting(int direction){
  currentSetting+=direction;
  if (currentSetting >= NUM_SETTINGS & direction > 0) currentSetting = 0;
  if (currentSetting < 0 & direction < 0) currentSetting = NUM_SETTINGS - 1;
  SettingInfo setting = settings[currentSetting];
  intValue = setting.reload(&preferences);
  showType = SHOW_VALUE;
  if (setting.getNumLabels() > 0) {
    showType = SHOW_LABEL;
    labelValue = setting.getLabel(intValue);
  }
  
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
       case SETTING_CLEDS_MIN_BRIGHTNESS_IDX:
        notifyCLedMinBrightness(false);
        break;
      case SETTING_CLEDS_MAX_BRIGHTNESS_IDX:
        notifyCLedMaxBrightness(false);
        break;
      case SETTING_EQUALIZER_IDX:
        notifyEqualizer(false);
        break;
      case SETTING_RANDOM_IDX:
        notifyLedRandomDuration(false);
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
  SettingInfo setting = settings[currentSetting];
  int delta = setting.getDelta();
  intValue+=(change*delta);
  int numLabels = setting.getNumLabels();
  if ( numLabels> 0) {
    if (intValue >= numLabels){
      intValue = 0;
    } else if (intValue < 0) {
      intValue = numLabels -1;
    }
    labelValue = setting.getLabel(intValue);
  }
  standby(300, showType);
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
  notifyCLedMinBrightness(true);
  notifyLedMaxBrightness(true);
  notifyCLedMaxBrightness(true);
  notifyEqualizer(true);
  
};



/** 
 Retrieve the settingsInfo based on the index.
 if reload is true, make also sure to reload the value from the preferences.
*/
SettingInfo* SettingsController::prepareSettings(boolean reload, int settingIndex) {
  SettingInfo* setting = &settings[settingIndex];
  if (reload) setting->reload(&preferences);
  return setting;
}

void SettingsController::notifyHelloTime(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_HELLO_MODE_TIME_IDX);
  displayController->updateHelloModeTime(setting->getMachineValue());
};

void SettingsController::notifyLightsMinDelay(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_LIGHTS_MIN_DELAY_IDX);
  ledController->updateLightsMinDelay(setting->getMachineValue());
};

void SettingsController::notifyEqualizer(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_EQUALIZER_IDX);
  int equalizer = setting->getMachineValue();
  musicController->updateEqualizer(equalizer);  
};

void SettingsController::notifyLedRandomDuration(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_RANDOM_IDX);
  int duration = setting->getMachineValue();
  ledController->updateLedRandomPatternDuration(duration);
}

void SettingsController::notifySleepShutdownTime(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_SLEEP_TIME_IDX);
  sleepShutdownTime = setting->getMachineValue();
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
};

void SettingsController::notifyLedMaxBrightness(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_LEDS_MAX_BRIGHTNESS_IDX);
  int brightness = setting->getMachineValue();
  ledController->updateLedMaxBrightness(brightness);
};

void SettingsController::notifyCLedMinBrightness(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_CLEDS_MIN_BRIGHTNESS_IDX);
  int brightness = setting->getMachineValue();
  consoleLightsController->updateLedMinBrightness(brightness+10);
};

void SettingsController::notifyCLedMaxBrightness(boolean reload){
  SettingInfo* setting = prepareSettings(reload, SETTING_CLEDS_MAX_BRIGHTNESS_IDX);
  int brightness = setting->getMachineValue();
  consoleLightsController->updateLedMaxBrightness(brightness+10);
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
      displayController->displayNames(setting.getName(), 2, 1000, REMOTE_MODE);
      standby(1000, READ_VALUE);
      break;
    case READ_VALUE:
      intValue = setting.reload(& preferences);
      standby(400, showType);
      break;
    case SHOW_VALUE:
      displayController->displayNames(toText(intValue), 1, 2000, REMOTE_MODE);
      standby(200, UPDATE_VALUE);
      break;
    case SHOW_LABEL:
      displayController->displayNames(labelValue, 1, 2000, REMOTE_MODE);
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

#endif
