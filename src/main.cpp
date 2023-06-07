#include "ButtonController.h"
#include "LedController.h"
#include "MusicController.h"
#include "StepperController.h"
#include "ConsoleController.h"
#include "ConsoleLightsController.h"
#include "DisplayController.h"
#include "ModeController.h"
#include "Constants.h"
#include <Arduino.h>


#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
#define TIME_TO_SLEEP 5

#define STOP 1
#define MODE WORKING_MODE

long t = 0;
bool debug = true;
int mode = START_MODE;
//SoftwareSerial mySerial(MP3_RX_PIN, MP3_TX_PIN);
HardwareSerial mySerial(1);
DFRobotDFPlayerMini mp3Player;
ModeController modeController = ModeController();
ButtonController buttonsController = ButtonController();
DisplayController displayController = DisplayController(&modeController);

LedController ledController = LedController();
StepperController stepperController = StepperController(STEPPER_INIT_SPEED);
MusicController musicController = MusicController(&displayController);
ConsoleLightsController consoleLightsController = ConsoleLightsController(&modeController, &musicController);

ConsoleController consoleController = ConsoleController(&modeController, &ledController, &musicController, &stepperController, &displayController, &buttonsController, &consoleLightsController);

void setup() {
  modeController.begin();
  delay(500);
  pinMode(BUTTON_POWER_ON, OUTPUT);
  delay(200);
  digitalWrite(BUTTON_POWER_ON, HIGH);
  if (CONTROL_MUSIC == ENABLED)
    mySerial.begin(9600, SERIAL_8N1,MP3_RX_PIN, MP3_TX_PIN);

  Serial.println();
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (CONTROL_MUSIC == ENABLED && !mp3Player.begin(mySerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println(F("Ready DFPlayer Mini"));
  Serial.println("Setting Up!");
  musicController.begin(&mp3Player);
  stepperController.begin();
  buttonsController.begin();
  ledController.begin();
  displayController.begin();
  consoleLightsController.begin();
  consoleController.begin();
  

}

void initialize() {
  
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("Initializing!");
  stepperController.init();
  buttonsController.init();
  musicController.init();
  ledController.init();
  consoleLightsController.init();
  displayController.init();
  consoleController.init();
  if (CONTROL_MUSIC == ENABLED)
    mp3Player.enableLoopAll();
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("Going to Working mode!");
  mode = WORKING_MODE;
}


void workingmode() {
  if (mode == START_MODE) {
    loop();
  }

  // First let's parse the buttons commands
  buttonsController.operate();

  // Delegate to the console the operations 
  consoleController.operate();
  musicController.operate();
  displayController.operate();
  ledController.operate();

  // Last priority is the stepper
  stepperController.operate();
  consoleLightsController.operate();

  if (consoleController.isReadyForSleep()) {
    mp3Player.stop();
    mp3Player.sleep();
    dbg("Everything is off! Sleeping");
    delay(1000);
    esp_deep_sleep_start();
    mode = SLEEPING_MODE;
    loop();
  }

}

void loop() {
  switch (mode) {
    case START_MODE:
      initialize();
      break;
    /*    case TEST_MODE:
      testmode();
      break;
    case DIAGNOSE_MODE:
      diagnoseMode();
      break;
*/
    case WORKING_MODE:
      workingmode();
      break;
    case SLEEPING_MODE:
      break;
  }
}



