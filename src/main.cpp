#include "ButtonController.h"
#include "LedController.h"
#include "MusicController.h"
#include "StepperController.h"
#include "ConsoleController.h"
#include "ConsoleLightsController.h"
#include "DisplayController.h"
#include "Constants.h"
#include <Arduino.h>
//#include <SoftwareSerial.h>


/**
 * Operational modes
 */
#define START_MODE 100
#define INITMODE 101
#define TEST_MODE 103
#define DIAGNOSE_MODE 104
#define WORKING_MODE 105
#define SETTING_MODE 106
#define SLEEPING_MODE 200

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

ButtonController buttonsController = ButtonController();
DisplayController displayController = DisplayController();

LedController ledController = LedController();
StepperController stepperController = StepperController(STEPPER_INIT_SPEED);
MusicController musicController = MusicController(&displayController);
ConsoleLightsController consoleLightsController = ConsoleLightsController(&musicController);

ConsoleController consoleController = ConsoleController(&ledController, &musicController, &stepperController, &displayController, &buttonsController, &consoleLightsController);


/** void operate2(void* pvParameters) {
  int line1=25;
  int line2=33;
  int line3=32;
  int line4=13;
  while (true) {
  delay(100);
      digitalWrite(line1, HIGH);
      digitalWrite(line2, HIGH);
      digitalWrite(line3, LOW);
      digitalWrite(line4, LOW);
  delay(100);
      digitalWrite(line1, LOW);
      digitalWrite(line2, HIGH);
      digitalWrite(line3, HIGH);
      digitalWrite(line4, LOW);
delay(100);
      digitalWrite(line1, LOW);
      digitalWrite(line2, LOW);
      digitalWrite(line3, HIGH);
      digitalWrite(line4, HIGH);
delay(100);
      digitalWrite(line1, HIGH);
      digitalWrite(line2, LOW);
      digitalWrite(line3, LOW);
      digitalWrite(line4, HIGH);
      }
}
*/

void setup() {
  if (CURRENT_MODE > DEBUG_MODE) {
    Serial.begin(115200);
    Serial.println("Setting up");
  }
  dbg("Charging capacitors");

  //mySerial.begin(9600);
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
  //xTaskCreatePinnedToCore(operate2, "Operate", 10000,NULL, 1, &Task2, 0);

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



