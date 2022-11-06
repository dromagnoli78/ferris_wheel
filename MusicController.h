#ifndef MusicController_h
#define MusicController_h

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "Constants.h"
#include "DisplayController.h"

#define DELTA_TIME 50

class MusicController {
private:
  bool needUpdate;
  bool playing = false;
  bool muted = false;
  bool canPlay = false;
  bool volumeTriggered = false;
  bool nextSongRequested = false;
  bool songTriggered = false;
  int lastTrack;
  int lastFolder = 0;
  int currentTrack = 0;
  int folder = 0;
  int8_t volume;
  int8_t previousVolume;
  int8_t maxVolume = 30;
  long lastVolumeCheckTime;
  long lastPlayCheckTime;
  long lastDebugTime = 0;
  bool sleepMode = false;
  long sleepStartTime = 0;
  long sleepLastCheck = 0;
  DFRobotDFPlayerMini* mp3Player;
  DisplayController* displayController;

public:
  MusicController(DisplayController* pDisplayController){
    displayController = pDisplayController;
  };
  void init();
  void begin(DFRobotDFPlayerMini* pMp3Player);
  void operate();
  void adjustVolume();
  int getTrackNumber(){
    return folder*1000 + currentTrack;
  };
  
  
  int nextSong(){
    if (!sleepMode) {
      nextSongRequested = true;
      canPlay = true;
      if (CURRENT_MODE == DEBUG_MODE) Serial.println("MusicController Requesting Next Song");
      return getTrackNumber();
    }

  };
  void sleeping(bool isSleeping);
  bool triggerMute();
  bool isPlaying(){return playing;};
  bool isMuted(){return muted;};
  void printDetail(uint8_t type, int value);
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

  mp3Player->EQ(DFPLAYER_EQ_CLASSIC);
  lastVolumeCheckTime = time;
  lastPlayCheckTime = time;
  if (CURRENT_MODE == DEBUG_MODE){
     Serial.println("MusicController init");
     Serial.print("Found songs: ");
     Serial.println(lastTrack);
  }
}

void MusicController::adjustVolume() {
  int v = analogRead(VOLUME_PIN);
  long time = millis();
    if (sleepMode && (time - sleepLastCheck > 30000)){
    if (CURRENT_MODE == DEBUG_MODE){
     Serial.println("MusicController Reducing maxVolume for sleep");
    }

    maxVolume-=2;
    if (maxVolume < 0) {
      maxVolume = 0;
    }
    sleepLastCheck = time;
  }
  volume = map(v, 0, 4095, 0, maxVolume);
 
  
  //Set volume value. From 0 to 30

  /*if (CURRENT_MODE == DEBUG_MODE) {
    long time = millis();
    if (time - lastDebugTime > 5000) {
      Serial.print("MusicController adjusting volume:");
      Serial.println(volume);
      lastDebugTime = time;
    }
  }
  */
  mp3Player->volume(volume);
}

void MusicController::operate() {
  if (CONTROL_MUSIC == DISABLED) return;
  long time = millis();

  // leave if muted
  if (volumeTriggered) {
    if (!muted) {
      if (CURRENT_MODE == DEBUG_MODE) Serial.println("MusicController Muting");
      mp3Player->volume(0);
      muted = true;
    } else {
      if (CURRENT_MODE == DEBUG_MODE) Serial.println("MusicController Un-Muting ");
      adjustVolume();
      muted = false;
    }
    volumeTriggered = false;
  }
  if (muted) {
    return;
  }

  // volume control
  if ((time - lastVolumeCheckTime) > DELTA_TIME && !nextSongRequested) {
    adjustVolume();
    lastVolumeCheckTime = time;
  }

  // music control every 500 seconds
  if ((time - lastPlayCheckTime) > (DELTA_TIME * 10)) {
    if (mp3Player->available()) {
      // Check the status of the player
      uint8_t readType = mp3Player->readType();
      if (CURRENT_MODE == DEBUG_MODE) {
        printDetail(readType, mp3Player->read()); 
      }

      // Did it stop playing?
      if (readType == DFPlayerPlayFinished) {
        playing = false;
      }
    }


    if (canPlay && (!playing || nextSongRequested)) {
      if (CURRENT_MODE == DEBUG_MODE) Serial.println("MusicController playing next");
      if (!sleepMode) {
        displayController->nextSong(currentTrack);
        currentTrack++;
      } else {
        currentTrack = SLEEP_SONG;
      }
      currentTrack = currentTrack % lastTrack;
      if (CURRENT_MODE == DEBUG_MODE) {
        Serial.print("MusicController: song number:");
        Serial.println(currentTrack);
      }
      mp3Player->play(currentTrack);
      if (CURRENT_MODE == DEBUG_MODE) {
        Serial.println("MusicController is playing");
      }
      playing = true;
      nextSongRequested = false;
    }
    lastPlayCheckTime = time;
  }
}

bool MusicController::triggerMute() {
  volumeTriggered = true;
  return muted;
}

void MusicController::sleeping(bool sleeping){
  sleepMode = sleeping;
  if (sleepMode) {
    currentTrack = SLEEP_SONG;
    sleepStartTime = millis();
    sleepLastCheck = sleepStartTime;
    playing = false;
    mp3Player->stop();
  }

}


void MusicController::printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}


#endif