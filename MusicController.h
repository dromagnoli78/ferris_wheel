#ifndef MusicController_h
#define MusicController_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Constants.h"

#define DELTA_TIME 50

class MusicController {
private:
  bool needUpdate;
  bool playing = false;
  bool muted = false;
  bool volumeTriggered = false;
  int lastTrack;
  int currentTrack;
  int8_t volume;
  int8_t previousVolume;
  long lastVolumeCheckTime;
  long lastPlayCheckTime;
  DFRobotDFPlayerMini* mp3Player;

public:
  MusicController(){

  };
  void init();
  void begin(DFRobotDFPlayerMini* pMp3Player);
  void operate();
  void adjustVolume();
  void nextSong(){};
void sleepMode(){};
  bool triggerMute();
  bool isPlaying(){return playing;};
  bool isMuted(){return muted;};
};

void MusicController::begin(DFRobotDFPlayerMini* pMp3Player) {

  if (CURRENT_MODE == DEBUG_MODE) Serial.println("MusicController begin");
  mp3Player = pMp3Player;
}

void MusicController::init() {
  currentTrack = 0;
  adjustVolume();
  previousVolume = volume;
  long time = millis();
  lastTrack = mp3Player->readFileCounts();
  lastVolumeCheckTime = time;
  lastPlayCheckTime = time;
}

void MusicController::adjustVolume() {
  int v = analogRead(VOLUME_PIN);
  volume = map(v, 0, 1024, 0, 30);
  //Set volume value. From 0 to 30
  mp3Player->volume(volume);
}

void MusicController::operate() {
  if (CONTROL_MUSIC == DISABLED) return;
  long time = millis();

  // leave if muted
  if (volumeTriggered) {
    if (!muted) {
      if (CURRENT_MODE == DEBUG_MODE) Serial.println("Muting MusicController");
      mp3Player->volume(0);
      muted = true;
    } else {
      if (CURRENT_MODE == DEBUG_MODE) Serial.println("Un-Muting MusicController");
      adjustVolume();
      muted = false;
    }
    volumeTriggered = false;
  }
  if (muted) {
    return;
  }

  // volume control
  if ((time - lastVolumeCheckTime) > DELTA_TIME) {
    adjustVolume();
    if (CURRENT_MODE == DEBUG_MODE) Serial.println("Adjusting Volume in MusicController");
    lastVolumeCheckTime = time;
  }

  // music control
  if ((time - lastPlayCheckTime) > (DELTA_TIME * 10)) {
    if (!playing /*|| buttonController->isNextSongRequested()*/) {
      //buttonController->setNextSongRequested(false);
      mp3Player->play(currentTrack);
      playing = true;
      currentTrack++;
      currentTrack = currentTrack % lastTrack;
      lastPlayCheckTime = time;
    }
  }
}

bool MusicController::triggerMute() {
  volumeTriggered = true;
  return muted;
}



#endif