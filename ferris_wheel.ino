#include "ButtonController.h"
#include "LedController.h"
#include "MusicController.h"
#include "StepperController.h"
#include "ConsoleController.h"
#include "ConsoleLightsController.h"
#include "DisplayController.h"


/**
 * Operational modes
 */
#define START_MODE 100
#define INITMODE 101
#define TEST_MODE 103
#define DIAGNOSE_MODE 104
#define WORKING_MODE 105
#define SETTING_MODE 106

#define RX_PIN 6
#define TX_PIN 7
#define STOP 1
#define MODE WORKING_MODE

long t = 0;
bool debug = true;
int mode = START_MODE;
SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN);
DFRobotDFPlayerMini mp3Player;

ButtonController buttonsController = ButtonController();
DisplayController displayController = DisplayController();
ConsoleLightsController consoleLightsController = ConsoleLightsController();
//ConsoleLedController consoleLedController = ConsoleLedController();
LedController ledController = LedController();
StepperController stepperController = StepperController(200, 100);
MusicController musicController = MusicController();

ConsoleController consoleController = ConsoleController(&ledController, /*&consoleLedController,*/ &musicController, &stepperController, &displayController, &buttonsController);


void setup() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.begin(9600);
  mySoftwareSerial.begin(9600);


  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!mp3Player.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
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
  ledController.begin();
  displayController.begin();
  //consoleLightsController.begin();
  consoleController.begin();
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
  }
}

void initialize() {
  mode = WORKING_MODE;
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("Initializing!");
  buttonsController.init();
  stepperController.init();
  musicController.init();
  ledController.init();
  //consoleLightsController.init();
  displayController.init();
  consoleController.init();
  mp3Player.enableLoopAll();
  mp3Player.volume(3);
  mp3Player.play(2);
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
 // consoleLightsController.operate();
}