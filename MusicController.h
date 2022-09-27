#ifndef MusicController_h
#define MusicController_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define RX_PIN 6
#define TX_PIN 7
SoftwareSerial mSoftwareSerial(RX_PIN, TX_PIN);

class MusicController {
  private:
    bool needUpdate;
    bool isPlaying = false;
    const bool debug;
    int lastTrack;
    int currentTrack;
    DFRobotDFPlayerMini* mp3Player;
    ButtonController* buttonController;

  public:
    MusicController(ButtonController *pButtonController){
      Serial.println("MusicController constructor");
      buttonController = pButtonController;
    };
  void init();
  void begin();
  void operate();
};

void MusicController::begin() {
  Serial.println("MusicController begin");
  mSoftwareSerial.begin(9600);
}

void MusicController::init(){
  currentTrack = 0;
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!mp3Player->begin(mSoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("Ready DFPlayer Mini"));
  mp3Player->enableLoopAll();
  mp3Player->volume(24);  //Set volume value. From 0 to 30
  lastTrack = mp3Player->readFileCounts();
}

void MusicController::operate(){
  if (!isPlaying || buttonController->isNextSongRequested()) {
    buttonController->setNextSongRequested(false);
    mp3Player->play(currentTrack);
    isPlaying = true;
    currentTrack++;
    currentTrack = currentTrack % lastTrack;
  }
}



#endif
