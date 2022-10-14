#ifndef MusicController_h
#define MusicController_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Constants.h"

#define RX_PIN 6
#define TX_PIN 7
#define VOLUME_PIN A0
#define DELTA_TIME 50

SoftwareSerial mSoftwareSerial(RX_PIN, TX_PIN);

class MusicController {
  private:
    bool needUpdate;
    bool isPlaying = false;
    bool isMuted = false;
    bool volumeTriggered = false;
    const bool debug;
    int lastTrack;
    int currentTrack;
    int volume;
    int previousVolume;
    long lastVolumeCheckTime;
    long lastPlayCheckTime;
    DFRobotDFPlayerMini* mp3Player;

  public:
    MusicController(){
      Serial.println("MusicController constructor");
    };
    void init();
    void begin(DFRobotDFPlayerMini* pMp3Player);
    void operate();
    void adjustVolume();
    bool triggerVolume();
};

void MusicController::begin(DFRobotDFPlayerMini* pMp3Player) {
  Serial.println("MusicController begin");
  mp3Player = pMp3Player;
  Serial.begin(9600);
}

void MusicController::init(){
  currentTrack = 0;
  adjustVolume();
  previousVolume = volume;
  long time = millis();
  lastTrack = mp3Player->readFileCounts();
  lastVolumeCheckTime = time;
  lastPlayCheckTime = time;
}

void MusicController::adjustVolume(){
  int v = analogRead(VOLUME_PIN);
  volume = map(v, 0,1024,0,30);
  //Set volume value. From 0 to 30
  mp3Player->volume(volume);  
}

void MusicController::operate(){
  long time = millis();
  
  // leave if muted
  if (volumeTriggered) {
    if(!isMuted) {
      mp3Player->volume(0);
      isMuted=true;
    } else {
      adjustVolume();
      isMuted = false;
    }
    volumeTriggered=false;
  }
  if (isMuted) {
    return;
  }

  // volume control
  if ((time - lastVolumeCheckTime) > DELTA_TIME) {
    adjustVolume();
    lastVolumeCheckTime = time;
  }

  // music control
  if ((time - lastPlayCheckTime) > (DELTA_TIME*10)) {
    if (!isPlaying /*|| buttonController->isNextSongRequested()*/) {
    //buttonController->setNextSongRequested(false);
    mp3Player->play(currentTrack);
    isPlaying = true;
    currentTrack++;
    currentTrack = currentTrack % lastTrack;
    lastPlayCheckTime = time;
    }
  }
}

bool MusicController::triggerVolume() {
  volumeTriggered = true;
  return isMuted;
}



#endif
