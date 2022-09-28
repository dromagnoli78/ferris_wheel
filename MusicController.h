#ifndef MusicController_h
#define MusicController_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define RX_PIN 6
#define TX_PIN 7
#define VOLUME_PIN A0
#define DELTA_TIME 50

SoftwareSerial mSoftwareSerial(RX_PIN, TX_PIN);

class MusicController {
  private:
    bool needUpdate;
    bool isPlaying = false;
    const bool debug;
    int lastTrack;
    int currentTrack;
    int volume;
    long lastVolumeCheckTime;
    long lastPlayCheckTime;
    DFRobotDFPlayerMini* mp3Player;
    ButtonController* buttonController;

  public:
    MusicController(ButtonController *pButtonController){
      Serial.println("MusicController constructor");
      buttonController = pButtonController;
    };
  void init();
  void begin(DFRobotDFPlayerMini* pMp3Player);
  void operate();
  void adjustVolume();
};

void MusicController::begin(DFRobotDFPlayerMini* pMp3Player) {
  Serial.println("MusicController begin");
  mp3Player = pMp3Player;
  Serial.begin(9600);
}

void MusicController::init(){
  currentTrack = 0;
  adjustVolume();
  long time = millis();
  lastTrack = mp3Player->readFileCounts();
  lastVolumeCheckTime = time;
  lastPlayCheckTime = time;
}

void MusicController::adjustVolume(){
  int v = analogRead(VOLUME_PIN);
  volume = map(v, 0,1024,0,30);
  mp3Player->volume(volume);  //Set volume value. From 0 to 30
}

void MusicController::operate(){
  long time = millis();
  if ((time - lastVolumeCheckTime) > DELTA_TIME) {
    adjustVolume();
    lastVolumeCheckTime = time;
  }
  if ((time - lastPlayCheckTime) > (DELTA_TIME*10)) {
    if (!isPlaying /*|| buttonController->isNextSongRequested()*/) {
    buttonController->setNextSongRequested(false);
    mp3Player->play(currentTrack);
    isPlaying = true;
    currentTrack++;
    currentTrack = currentTrack % lastTrack;
    lastPlayCheckTime = time;
    }
  }
}



#endif
