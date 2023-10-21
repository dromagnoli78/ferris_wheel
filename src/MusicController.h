#ifndef MusicController_h
#define MusicController_h

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "Constants.h"
#include "TracksController.h"
#include "DisplayController.h"

#define DELTA_TIME 100

class MusicController {
private:
  bool needUpdate;
  bool playing = false;
  boolean paused = false;
  bool muted = false;
  bool canPlay = false;
  bool muteTriggered = false;
  int trackRequestIncrement = 0;
  bool songTriggered = false;
  bool sleepMode = false;
  int lastTrack;
  int numFolders = 0;
  int currentTrack = 0;
  int sleepTrack = 0;
  int analogVolume;
  int8_t volume;
  int8_t previousVolume;
  int8_t maxVolume = 30;
  unsigned long lastOperateCheckTime;
  unsigned long lastVolumeCheckTime;
  unsigned long lastPlayCheckTime;
  unsigned long lastDebugTime = 0;
  unsigned long sleepStartTime = 0;
  unsigned long sleepLastCheck = 0;
  unsigned long sleepingStepInterval = 0;
  int sleepShutdownTime;

  int sleepingSteps = 0;
  DFRobotDFPlayerMini* mp3Player;
public:
  DisplayController* displayController;
  TracksController* tracksController;
  MusicController(DisplayController* pDisplayController){
    displayController = pDisplayController;
    tracksController = displayController->tracksController;
  };
  void init();
  void begin(DFRobotDFPlayerMini* pMp3Player);
  void operate();
  void adjustVolume();
  int getAnalogVolume() {return analogVolume;}
  int computeSkip(u_char c);
  int computeNextFolder();
  int computePreviousFolder();
  void requestNewTrack(int step);
  void sleeping(bool isSleeping);
  bool requestMute();
  void stop();
  void pause();
  void unpause();
  bool isPlaying(){return playing;};
  bool isMuted(){return muted;};
  void printDetail(uint8_t type, int value);
  void updateSleepShutdownTime(int iShutdownTime){sleepShutdownTime = iShutdownTime;}
};



void MusicController::begin(DFRobotDFPlayerMini* pMp3Player) {
  dbg("MusicController begin");
  mp3Player = pMp3Player;
}

void MusicController::requestNewTrack(int step){
    if (!sleepMode) {
      trackRequestIncrement = step;
      canPlay = true;
    }
    dbg("MusicController Requesting Track Change:", step);
  }

void MusicController::stop() {
  dbg("MusicController Stopping");
  mp3Player->stop();
  playing = false;
  trackRequestIncrement = 0;
}

void MusicController::pause() {
  dbg("MusicController Pausing");
  mp3Player->pause();
  playing = false;
  paused = true;
  trackRequestIncrement = 0;
}

void MusicController::unpause() {
  dbg("MusicController Unpausing");
  if (paused) {
    mp3Player->start();
    paused = false;
    playing = true;
    } else {
      requestNewTrack(1);
    }
}

void MusicController::init() {
  if (CONTROL_MUSIC == CTL_DISABLED) return;
  currentTrack = 0;
  adjustVolume();
  
  previousVolume = volume;
  unsigned long time = millis();
  lastTrack = mp3Player->readFileCounts();
  numFolders = tracksController->getNumFolders();
  sleepTrack = lastTrack;

  mp3Player->EQ(DFPLAYER_EQ_CLASSIC);
  lastVolumeCheckTime = time;
  lastPlayCheckTime = time;
  if (CURRENT_MODE > DEBUG_MODE){
     Serial.println("MusicController init");
     Serial.print("Found Tracks: ");
     Serial.println(lastTrack);
  }
}

void MusicController::adjustVolume() {
  unsigned long time = millis();
  if (sleepMode && (time - sleepLastCheck > sleepingStepInterval)){
    // Slowly reducing the volume, based on the sleep shutdown
    analogVolume = 0;
    sleepLastCheck = time;
    volume--;
    if (volume < 0) {
      volume =0;
    }
    dbg("MusicController Reducing maxVolume for sleep. Vol:", (unsigned long) volume);
    if (volume == 0) {
      mp3Player->stop();
      playing == false;
    }
    
  } else {
    volume = map(analogVolume, 0, 4095, 0, maxVolume);
  }
 
  //Set volume value. From 0 to 30

  /*if (CURRENT_MODE > DEBUG_MODE) {
    unsigned long time = millis();
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
  if (CONTROL_MUSIC == CTL_DISABLED) return;
  unsigned long time = millis();
  if ((time - lastOperateCheckTime) < DELTA_MUSIC_TIME)
    return;
  analogVolume = analogRead(VOLUME_PIN);
  if (muteTriggered) {
    if (!muted) {
      dbg("MusicController Muting");
      mp3Player->volume(0);
      muted = true;
    } else {
      dbg("MusicController Un-Muting ");
      adjustVolume();
      muted = false;
    }
    muteTriggered = false;
  }

  // leave if muted
  if (muted) {
    return;
  }

  // volume control
  int deltaTime = sleepMode ? sleepingStepInterval :  DELTA_VOLUME_TIME;
  if ((time - lastVolumeCheckTime) > deltaTime && trackRequestIncrement==0) {
    adjustVolume();
    lastVolumeCheckTime = time;
  }

  // music control every 500 seconds
  if ((time - lastPlayCheckTime) > (DELTA_TIME * 5)) {
    if (mp3Player->available()) {
      // Check the status of the player
      uint8_t readType = mp3Player->readType();
      if (CURRENT_MODE > DEBUG_MODE) {
        printDetail(readType, mp3Player->read()); 
      }

      // Did it stop playing?
      if (readType == DFPlayerPlayFinished) {
        playing = false;
        trackRequestIncrement = 1;
      }
    }

    bool play = true;
    if (canPlay && (!playing || trackRequestIncrement != 0)) {
      if (!sleepMode) {
        if (trackRequestIncrement != 0)
          ("MusicController playing next");
        currentTrack+=trackRequestIncrement;
        if (currentTrack <= 0) {
          currentTrack = lastTrack;
        } else if (currentTrack > lastTrack) {
          currentTrack = 1;
        }
        if (trackRequestIncrement != 0) {
          int trackIndex = currentTrack-1;
          const char* trackTitle = tracksController->getTrack(trackIndex);
          displayController->initDisplayTrack(trackTitle);
          displayController->requestNewTrack(currentTrack);

        }
      } else {
        currentTrack = sleepTrack;
        if (time - sleepStartTime < DELAY_SOFT_START_SLEEP) {
          play = false;
        }
      }
      
      if (trackRequestIncrement != 0||sleepMode){
        dbg("MusicController is going to play track number:", currentTrack);
        if (play) mp3Player->play(currentTrack);
        playing = play;
      }
      trackRequestIncrement = 0;
    }
    lastPlayCheckTime = time;
    lastOperateCheckTime = time;
  }
}

bool MusicController::requestMute() {
  muteTriggered = true;
  return muted;
}

void MusicController::sleeping(bool sleeping){
  sleepMode = sleeping;
  if (sleepMode) {
    
    currentTrack = SLEEP_TRACK;
    sleepStartTime = millis();
    sleepLastCheck = sleepStartTime;
    playing = false;
    canPlay = true;
    int v = analogRead(VOLUME_PIN);
    sleepingSteps = map(v, 0, 4095, 0, maxVolume);
    if (sleepingSteps == 0) {
      sleepingSteps = 1;
    }
    sleepingStepInterval = sleepShutdownTime / sleepingSteps;
    dbg("DisplayController: SleepMode triggered with sleepingStepInterval:", sleepingStepInterval);
    if (CONTROL_MUSIC == ENABLED)
      mp3Player->stop();
  }

}

int MusicController::computeSkip(u_char c){
  int skip;
  switch (c) {
    case 'N':
      dbg("MusicController ComputeSkip: Returning +1");
      skip = 1;
      break;
    case 'P':
      dbg("MusicController ComputeSkip: Returning -1");
      skip  = -1;
      break;
    case 'U':
      dbg("MusicController ComputeSkip: Returning +1D");
      skip = tracksController->computeNextFolder(currentTrack);
      break;
    case 'D':
      dbg("MusicController ComputeSkip: Returning -1D");
      skip = tracksController->computePreviousFolder(currentTrack);
  }
  return skip;
}

int MusicController::computePreviousFolder() {
  return 0;
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