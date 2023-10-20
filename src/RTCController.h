#ifndef rtc_controller_h
#define rtc_controller_h

#include "RTClib.h"
#include "Constants.h"
#include "Images.h"
#include "ModeController.h"

class RTCController {
private:
  char str[8];
  bool colon = true;
  RTC_DS1307 rtc;
public:
  void begin();
  char* getTimeString();
  char* getSecondsString();
  const unsigned char* getHourImage();
  const unsigned char* getMinuteImage();
};

void RTCController::begin() {
  if (CONTROL_RTC == CTL_DISABLED) return;
  if (CURRENT_MODE > DEBUG_MODE)
    dbg("RTCController begin");
  if (!rtc.begin()) {
    dbg("Couldn't find RTC");
    Serial.flush();
  }

  if (!rtc.isrunning()) {
    if (CURRENT_MODE > DEBUG_MODE)
      dbg("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

char* RTCController::getTimeString() {
  DateTime now = rtc.now();
  if (colon) {
    sprintf(str, "%02d:%02d", now.hour(), now.minute());
  } else {
    sprintf(str, "%02d.%02d", now.hour(), now.minute());
  }
  colon = !colon;
  return str;
}

char* RTCController::getSecondsString() {
  DateTime now = rtc.now();
  sprintf(str, "%02d", now.second());
  colon = !colon;
  return str;
}

const unsigned char* RTCController::getHourImage() {
  DateTime now = rtc.now();
  int minute = now.minute();
  int hour = now.hour();
  // Let's add one hour if the minutes are > 45
  if (minute > 45)
    hour++;
  int hourImage = hour % 12;
  switch (hourImage) {
    case 0:
      return c12S;
    case 1:
      return c1S;
    case 2:
      return c2S;
    case 3:
      return c3S;
    case 4:
      return c4S;
    case 5:
      return c5S;
    case 6:
      return c6S;
    case 7:
      return c7S;
    case 8:
      return c8S;
    case 9:
      return c9S;
    case 10:
      return c10S;
    case 11:
      return c11S;
    default:
      return clock_frame;
  }
}

const unsigned char* RTCController::getMinuteImage() {
  DateTime now = rtc.now();
  int minute = (now.minute()+2) / 5;
  switch (minute) {
    case 0:
    case 12:
      return c12L;
    case 1:
      return c1L;
    case 2:
      return c2L;
    case 3:
      return c3L;
    case 4:
      return c4L;
    case 5:
      return c5L;
    case 6:
      return c6L;
    case 7:
      return c7L;
    case 8:
      return c8L;
    case 9:
      return c9L;
    case 10:
      return c10L;
    case 11:
      return c11L;
    default:
      return clock_frame;
  }
}

#endif