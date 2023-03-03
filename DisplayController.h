#ifndef DisplayController_h
#define DisplayController_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTCController.h"
#include "Images.h"
#include "Songs.h"
#include "CherryCreamSoda_24.h"
#include "CherryCreamSoda_20.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

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

#define SLEEP_FRAMES 5
class DisplayController {
private:

  // Timing variables
  unsigned long timeOfLastDebug = 0;
  unsigned long timeLastDisplayUpdate;
  unsigned long timeLastSleepFrame = 0;       // Timer for sleepFramesCheck
  unsigned long timeSongTitleBeingShown = 0;  // Timer for song title being shown initially (used to switch to clock after a while)
  unsigned long timeSleepModeStarted = 0;     // When sleep Mode started
  unsigned long timeLastRTCUpdate = 0;
  unsigned long timeOfHello=0;

  int deltaTimeForNextFrame = 200;
  int deltaTime = 300;

  // Control variables
  uint8_t mode = HELLO_MODE;
  uint8_t lastMode;    // Last mode before sleep (for recovery)
  int songIndex;       // Index of the song currently playing
  int sleepFrame = 0;  // Sleep animation is composed of different frames
  bool isScrolling = false;
  bool preSleep = false;  // Pre-sleep phase. Needed for different message

  // Objects
  SongGetter songGetter;  // Object used to get song titles.
  RTCController rtc;

public:
  void init();
  void begin();
  void operate();
  void displayMessage(const char*, int fontsize);
  void displayHello();
  void displayTime();
  void displaySleep();
  void displayPreSleep();
  void displaySong(int index);
  void displayNone();
  void nextSong(int songIndex);
  void sleeping(bool isSleeping);

  DisplayController() {
    timeLastDisplayUpdate = millis();
    songGetter = SongGetter();
    rtc = RTCController();
  };
};

void DisplayController::begin() {
  if (CONTROL_DISPLAY == DISABLED) return;
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  rtc.begin();
}

void DisplayController::init() {
   if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("DisplayController init");
  if (CONTROL_DISPLAY == DISABLED) return;
  display.clearDisplay();
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.setCursor(14, 16);
  display.drawBitmap(0, 0, ciaoNadia, 128, 32, SSD1306_WHITE);
  display.display();
}

void DisplayController::operate() {
  if (CONTROL_DISPLAY == DISABLED) return;

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
        displaySong(songIndex);
        break;
      case HELLO_MODE:
        displayHello();
        break;
    }
  }
}

void DisplayController::displayTime() {
  unsigned long time = millis();
  if (time - timeLastRTCUpdate > 1000) {
    display.clearDisplay();
    display.drawBitmap(96, 0, rtc.getHourImage(), 32, 32, SSD1306_WHITE);
    display.drawBitmap(96, 0, rtc.getMinuteImage(), 32, 32, SSD1306_WHITE);
    // Set special font for the clock
    
    display.setFont(&Cherry_Cream_Soda_Regular_20);
    display.setCursor(0, 24);
    display.println(rtc.getTimeString());
    display.display();
    timeLastRTCUpdate = time;
  }
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

void DisplayController::displaySong(int index) {
  if (!isScrolling)
    displayMessage(songGetter.getTrack(index), 1);
  unsigned long time = millis();
  if (time - timeSongTitleBeingShown > DELAY_AFTER_SONG) {
    mode = TIME_MODE;
    isScrolling = false;
    display.stopscroll();
  }
}

void DisplayController::displayMessage(const char* message, int fontsize) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(fontsize);
  display.setFont();
  display.println(message);
  display.display();
  display.startscrollright(0x00, 0x0F);
  isScrolling = true;
}

void DisplayController::nextSong(int index) {
  if (CURRENT_MODE > DEBUG_MODE) Serial.println("DisplayController: Requested NextSong");
  mode = MUSIC_MODE;
  songIndex = index;
  timeSongTitleBeingShown = millis();
  isScrolling = false;
}


void DisplayController::displayPreSleep() {
  unsigned long time = millis();
  long deltaTime = time - timeSleepModeStarted;
  // Display buonaNotte once and then go to sleep mode
  if (preSleep && (deltaTime > 100 && deltaTime < 1000)) {
    display.stopscroll();
    display.clearDisplay();
    display.drawBitmap(0, 0, buonaNotte, 128, 32, SSD1306_WHITE);
    display.display();
    preSleep = false;
  }

  if (time - timeSleepModeStarted > DELAY_SOFT_START_SLEEP) {
    // Switch to SLEEP MODE after the DELAY
    mode = SLEEP_MODE;
    isScrolling = false;
  }
}

void DisplayController::displaySleep() {
  unsigned long time = millis();
  // Sleep animation is made of different frames (phases)
  if (time - timeLastSleepFrame > deltaTimeForNextFrame) {
    display.clearDisplay();

    switch (sleepFrame) {
      case 0:
        display.drawBitmap(0, 0, sleepf1, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 400;
        break;
      case 1:
        display.drawBitmap(0, 0, sleepf2, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 500;
        break;
      case 2:
        display.drawBitmap(0, 0, sleepf3, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 400;
        break;
      case 3:
        display.drawBitmap(0, 0, sleepf4, 128, 32, SSD1306_WHITE);
        deltaTimeForNextFrame = 300;
        break;
      case 4:
      default:
        deltaTimeForNextFrame = 400;
        display.drawBitmap(0, 0, sleepf5, 128, 32, SSD1306_WHITE);
        break;
    }

    sleepFrame++;
    sleepFrame %= SLEEP_FRAMES;
    display.display();
    timeLastSleepFrame = time;

    if (time - timeSleepModeStarted > SLEEP_SHUTDOWN - DELAY_SOFT_START_SLEEP) {
      mode = EMPTY_MODE;
    }
  }
}

void DisplayController::sleeping(bool sleeping) {
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("DisplayController: SleepMode triggered");
  if (sleeping) {
    lastMode = mode;
    mode = PRE_SLEEP_MODE;
    timeSleepModeStarted = millis();
    isScrolling = false;
    preSleep = true;
  } else {
    mode = lastMode;
  }
}




#endif