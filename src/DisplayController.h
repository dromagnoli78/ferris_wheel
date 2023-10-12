#ifndef DisplayController_h
#define DisplayController_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTCController.h"
#include "Images.h"
#include "TracksController.h"
#include "ModeController.h"
#include "CherryCreamSoda_20.h"
#include "DSeg7_Classic_12.h"
#include "DSEG7_Classic_Mini_Regular_20.h"
#include "Roboto_Condensed_18.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define CMD_ONE_COLUMN_SCROLL_H_RIGHT       0x2C
#define CMD_ONE_COLUMN_SCROLL_H_LEFT        0x2D

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TIME_MODE 0
#define MUSIC_MODE 1
#define PRE_SLEEP_MODE 2
#define SLEEP_MODE 3
#define EMPTY_MODE 4
#define HELLO_MODE 5
#define FUNCTION_MODE 6
#define SEQUENCE_MODE 7
#define SETTINGS_MODE 8
#define FOLDER_MODE 9

#define SLEEP_FRAMES 5
class DisplayController {
private:

  // Timing variables
  unsigned long timeOfLastDebug = 0;
  unsigned long timeLastDisplayUpdate;
  unsigned long timeLastSleepFrame = 0;       // Timer for sleepFramesCheck
  unsigned long timeMessageBeingShown = 0;  // Timer for song title being shown initially (used to switch to clock after a while)
  unsigned long timeSleepModeStarted = 0;     // When sleep Mode started
  unsigned long timeLastRTCUpdate = 0;
  unsigned long timeOfHello=0;

  int deltaTimeForNextFrame = 200;
  int deltaTime = 300;

  // Control variables
  uint8_t mode = HELLO_MODE;
  uint8_t lastMode;    // Last mode before sleep (for recovery)
  int trackIndex;       // Index of the track currently playing
  int sleepFrame = 0;  // Sleep animation is composed of different frames
  const char * text;
  bool preSleep = false;  // Pre-sleep phase. Needed for different message
  int x;
  int minX;
  const char* currentFolder = "";
  RTCController rtc;

public:
  void init();
  void begin();
  void operate();
  void displayMessage(const char*, int fontsize, bool scroll);
  void displayHello();
  void displayTime();
  void displaySleep();
  void displayPreSleep();
  void displayTrack(int index);
  void displayFolder(int index);
  void displayText(const char* text, int delay, bool scroll, int fontsize, int nextMode);
  void displaySettings(const char* text, int delay);
  void displayNone();
  void sendMessage(const char* text, uint8_t messageMode);
  void requestNewTrack(int trackIndex);
  void initDisplayTrack(const char* title);
  void sleeping(bool isSleeping);
  TracksController* tracksController;  // Object used to get song titles.
  ModeController* modeController;
  DisplayController(ModeController* pModeController) {
    timeLastDisplayUpdate = millis();
    tracksController = new TracksController();
    modeController = pModeController;
    rtc = RTCController();
  };
};

void DisplayController::begin() {
  dbg("DisplayController begin");
  if (CONTROL_RTC == ENABLED) rtc.begin();
  if (CONTROL_DISPLAY == CTL_DISABLED) return;
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
      Serial.println(F("SSD1306 allocation failed"));
}

void DisplayController::init() {
  dbg("DisplayController init");
  if (CONTROL_DISPLAY == CTL_DISABLED) return;
  display.clearDisplay();
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.setCursor(14, 16);
  display.drawBitmap(0, 0, ciaoNadia, 128, 32, SSD1306_WHITE);
  display.display();
  x = display.width();
  minX = -8 * strlen(tracksController->getLongestTrackName());
  const char* currentFolder = tracksController->getFolder(tracksController->getNumFolders()-1);
}

void DisplayController::operate() {
  if (CONTROL_DISPLAY == CTL_DISABLED) return;

  unsigned long time = millis();
  if (CURRENT_MODE > DEBUG_MODE) {
    if (time - timeOfLastDebug > 20000) {
      Serial.println("DisplayController operate");
      timeOfLastDebug = time;
    }
  }

  // Only update the display every deltaTime intervals
  if (time - timeLastDisplayUpdate > deltaTime) {
    switch (mode) {
      case TIME_MODE:
        displayTime();
        break;
      case PRE_SLEEP_MODE:
        displayPreSleep();
        break;
      case SLEEP_MODE:
        displaySleep();
        break;
      case EMPTY_MODE:
        displayNone();
        break;
      case MUSIC_MODE:
        displayTrack(trackIndex);
        break;
      case FOLDER_MODE:
        displayFolder(trackIndex);
        break;
      case FUNCTION_MODE:
        displayText(text, DELAY_AFTER_MESSAGE, false, 1, TIME_MODE);
        break;
      case SETTINGS_MODE:
        displaySettings(text, DELAY_AFTER_MESSAGE);
        break;
      case HELLO_MODE:
        displayHello();
        break;
    }
  }
}

void DisplayController::displayTime() {
  unsigned long time = millis();
  if (CONTROL_RTC == ENABLED) {
    if (time - timeLastRTCUpdate > 1000) {
      if (CONTROL_DISPLAY == ENABLED) {
        display.clearDisplay();
        display.drawBitmap(96, 0, rtc.getHourImage(), 32, 32, SSD1306_WHITE);
        display.drawBitmap(96, 0, rtc.getMinuteImage(), 32, 32, SSD1306_WHITE);
        // Set special font for the clock
        
        display.setFont(&DSEG7_Classic_Mini_Regular_20);
        display.setCursor(0, 25);
        display.println(rtc.getTimeString());
        display.display();
        display.setFont(&DSEG7_Classic_Regular_12);
        display.setCursor(72, 15);
        display.println(rtc.getSecondsString());
        display.display();
      } else {
        dbg(rtc.getTimeString());
      }
      timeLastRTCUpdate = time;
    }
  }
}

void DisplayController::displaySettings(const char* message, int delay){
  bool doIt = CONTROL_DISPLAY == ENABLED;
  const char* title = message;
  unsigned long time = millis();
  if (CONTROL_DISPLAY == ENABLED) {
    display.clearDisplay();
    display.setFont(&Cherry_Cream_Soda_Regular_20);
    display.setCursor(0, 24);
    display.println(title);
    display.display();
  }
  if (time - timeMessageBeingShown > delay) {
    mode = TIME_MODE;
  }
  display.stopscroll();
}

void DisplayController::displayNone() {
  display.clearDisplay();
  display.display();
}

void DisplayController::displayHello() {
  display.clearDisplay();
  display.drawBitmap(0, 0, ciaoNadia, 128, 32, SSD1306_WHITE);
  display.display();
  unsigned long time = millis();
  if (timeOfHello == 0) {
    timeOfHello = time;
  }
  if (time - timeOfHello > DELAY_AFTER_HELLO) {
    mode = TIME_MODE;
  }
}

void DisplayController::displayTrack(int index) {
  const char* title = tracksController->getTrack(index);
  displayText(title, DELAY_AFTER_SONG, true, 1, TIME_MODE);
}

void DisplayController::displayFolder(int index) {
  int folderIndex = tracksController->computeFolderByTrackNumber(index);
  const char* folderName = tracksController->getFolder(folderIndex+1);
  displayText(folderName, DELAY_AFTER_FOLDER, true, 1, MUSIC_MODE);
}

void DisplayController::displayText(const char* message, int delay, bool scroll, int fontsize, int nextMode) {
  bool doIt = CONTROL_DISPLAY == ENABLED;
  const char* title = message;

  if (doIt) 
    displayMessage(title, fontsize, scroll);
  unsigned long time = millis();
  if (time - timeMessageBeingShown > delay) {
    mode = nextMode;
    timeMessageBeingShown = millis();
    if (doIt) display.stopscroll();
  }
}

void DisplayController::displayMessage(const char* message, int fontsize, bool scroll) {
  if (CONTROL_DISPLAY == ENABLED) {
    display.clearDisplay();
    display.setCursor(0, 2);
    display.setTextSize(fontsize);
    display.setFont(&Roboto_Condensed_18);
    //display.println(message);
    //display.display();
   display.setCursor(x,16); 
   display.print(message);
   display.display();
   x = x - 1; // scroll speed, make more positive to slow down the scroll
   if(x < minX) x = display.width();
  }
}

void DisplayController::requestNewTrack(int index) {
  dbg("DisplayController: NextTrack");
  mode = MUSIC_MODE;
  trackIndex = index-1;
  timeMessageBeingShown = millis();
  const char* folder = tracksController->getFolder(tracksController->computeFolderByTrackNumber(index));
  if (strcmp(folder, currentFolder) != 0){
    currentFolder = folder;
    mode = FOLDER_MODE;
  }
}

void DisplayController::initDisplayTrack(const char* title) {
  dbg("DisplayController: initDisplayTrack");
  x = display.width();
  minX = -8 * strlen(title);
}

void DisplayController::sendMessage(const char* message, uint8_t messageMode) {
  dbg("DisplayController: show Message");
  mode = messageMode;
  timeMessageBeingShown = millis();
  text = message;
}


void DisplayController::displayPreSleep() {
  unsigned long time = millis();
  long deltaTime = time - timeSleepModeStarted;

  // Display buonaNotte once and then go to sleep mode
  if (preSleep && (deltaTime > 100 && deltaTime < 1000)) {
    if (CONTROL_DISPLAY == ENABLED) {
      display.stopscroll();
      display.clearDisplay();
      display.drawBitmap(0, 0, buonaNotte, 128, 32, SSD1306_WHITE);
      display.display();
    }
    preSleep = false;
  }

  if (time - timeSleepModeStarted > DELAY_SOFT_START_SLEEP) {
    // Switch to SLEEP MODE after the DELAY
    mode = SLEEP_MODE;
  }
}

void DisplayController::displaySleep() {
  unsigned long time = millis();
  // Sleep animation is made of different frames (phases)
  bool doIt = CONTROL_DISPLAY == ENABLED;
  if (time - timeLastSleepFrame > deltaTimeForNextFrame) {
    if (doIt) display.clearDisplay();

    switch (sleepFrame) {
      case 0:
        if (doIt) display.drawBitmap(0, 0, sleepf1, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 400;
        break;
      case 1:
        if (doIt) display.drawBitmap(0, 0, sleepf2, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 500;
        break;
      case 2:
        if (doIt) display.drawBitmap(0, 0, sleepf3, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 400;
        break;
      case 3:
        if (doIt) display.drawBitmap(0, 0, sleepf4, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 300;
        break;
      case 4:
      default:
        deltaTimeForNextFrame = 400;
        if (doIt) display.drawBitmap(0, 0, sleepf5, 128, 32, SSD1306_WHITE);
        break;
    }

    sleepFrame++;
    sleepFrame %= SLEEP_FRAMES;
    if (doIt) display.display();
    timeLastSleepFrame = time;

    if (time - timeSleepModeStarted > SLEEP_SHUTDOWN - DELAY_SOFT_START_SLEEP) {
      mode = EMPTY_MODE;
    }
  }
}

void DisplayController::sleeping(bool sleeping) {
  dbg("DisplayController: SleepMode triggered");
  if (sleeping) {
    lastMode = mode;
    mode = PRE_SLEEP_MODE;
    timeSleepModeStarted = millis();
    preSleep = true;
  } else {
    mode = lastMode;
  }
}

#endif