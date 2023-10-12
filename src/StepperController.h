#ifndef StepperController_h
#define StepperController_h

#include "Constants.h"
#define STEPPER_INCREMENT 1

class MyStepper {
private:
  int line1;
  int line2;
  int line3;
  int line4;
  int phase = 0;

public:
  MyStepper(int iLine1, int iLine2, int iLine3, int iLine4) {
    line1 = iLine1;
    line2 = iLine2;
    line3 = iLine3;
    line4 = iLine4;
    pinMode(line1, OUTPUT);
    pinMode(line2, OUTPUT);
    pinMode(line3, OUTPUT);
    pinMode(line4, OUTPUT);
  }
  void doStep(bool moveClockwise);
  void stop();
};

void MyStepper::doStep(bool moveClockwise) {
  if (!moveClockwise) {
    switch (phase) {
      case 0:
        digitalWrite(line1, HIGH);
        digitalWrite(line2, HIGH);
        digitalWrite(line3, LOW);
        digitalWrite(line4, LOW);
        break;
      case 3:
        digitalWrite(line1, LOW);
        digitalWrite(line2, HIGH);
        digitalWrite(line3, HIGH);
        digitalWrite(line4, LOW);
        break;
      case 2:
        digitalWrite(line1, LOW);
        digitalWrite(line2, LOW);
        digitalWrite(line3, HIGH);
        digitalWrite(line4, HIGH);
        break;
      case 1:
        digitalWrite(line1, HIGH);
        digitalWrite(line2, LOW);
        digitalWrite(line3, LOW);
        digitalWrite(line4, HIGH);
        break;
    }
  }else {
    switch (phase) {
      case 0:
        digitalWrite(line1, HIGH);
        digitalWrite(line2, HIGH);
        digitalWrite(line3, LOW);
        digitalWrite(line4, LOW);
        break;
      case 1:
        digitalWrite(line1, LOW);
        digitalWrite(line2, HIGH);
        digitalWrite(line3, HIGH);
        digitalWrite(line4, LOW);
        break;
      case 2:
        digitalWrite(line1, LOW);
        digitalWrite(line2, LOW);
        digitalWrite(line3, HIGH);
        digitalWrite(line4, HIGH);
        break;
      case 3:
        digitalWrite(line1, HIGH);
        digitalWrite(line2, LOW);
        digitalWrite(line3, LOW);
        digitalWrite(line4, HIGH);
        break;
    }
  }
  phase++;
  phase %= 4;
}

void MyStepper::stop() {
  digitalWrite(line1, LOW);
  digitalWrite(line2, LOW);
  digitalWrite(line3, LOW);
  digitalWrite(line4, LOW);  
}

class StepperController {
private:
  bool moveTriggered = false;
  bool isStopped = true;
  MyStepper stepper = MyStepper(STEPPER_IN_1, STEPPER_IN_2, STEPPER_IN_3, STEPPER_IN_4);
  bool moveClockwise = true;
  int deltaTime;
  u_int8_t currentSpeed = 2;
  unsigned long lastTime;
public:
  void init();
  void begin();
  void operate();
  StepperController(int iDeltaTime)
    : deltaTime(iDeltaTime) {
    lastTime = millis();
  };
  bool triggerMovement();
  bool isItStopped(){return isStopped;};
  void speedChange(u_char c);
  const char* getSpeedSymbol();
};

bool StepperController::triggerMovement() {
  moveTriggered = true;
  dbg("StepperController triggered");
  return !isStopped;
}

const char* StepperController::getSpeedSymbol() {
  if (moveClockwise) {
    if (currentSpeed == 1) {
      return "Stepper>";
    } else if (currentSpeed == 2) {
      return "Stepper>>";
    } else if (currentSpeed == 3) {
      return "Stepper>>>";
    } else {
      return "-";
    }
  } else {
    if (currentSpeed == 1) {
      return "  <Stepper";
    } else if (currentSpeed == 2) {
      return " <<Stepper";
    } else if (currentSpeed == 3) {
      return "<<<Stepper";
    } else {
      return "-";
    }
  }
}  

void StepperController::speedChange(u_char c) {
  switch (c) {
    case 'L':
      dbg("Stepper Anti-Clockwise");
      moveClockwise = false;
      break;
    case 'R':
      dbg("Stepper Clockwise");
      moveClockwise = true;
      break;
    case 'U':
      currentSpeed++;
      if (currentSpeed > 3) {
        currentSpeed--;
      } else {
        dbg("Stepper speed-up");
      }
      break;
    case 'D':
      currentSpeed--;
      if (currentSpeed < 1) {
        currentSpeed++;
      } else {
        dbg("Stepper speed-down");
      }
      break;
  }
  switch (currentSpeed){
    case 1:
      deltaTime = STEPPER_SPEED_SLOW;
      break;
    case 2:
      deltaTime = STEPPER_SPEED_MED;
      break;
    case 3:
      deltaTime = STEPPER_SPEED_FAST;
      break;
  }
}

void StepperController::begin() {
  if (CONTROL_STEPPER == CTL_DISABLED) return;
  dbg("StepperController begin");
}

void StepperController::init() {
  dbg("StepperController init");
 
}

void StepperController::operate() {
  if (CONTROL_STEPPER == CTL_DISABLED) return;
  unsigned long time = millis();
  if ((time - this->lastTime) > deltaTime) {
    this->lastTime = millis();
    if (moveTriggered) {
      moveTriggered = false;
      if (isStopped) {
        isStopped = false;
      } else {
        isStopped = true;

        // Power off all the 4 phases
        stepper.stop();

        return;
      }
    }
    if (!isStopped) {
      stepper.doStep(moveClockwise);
    }
  }
}

#endif