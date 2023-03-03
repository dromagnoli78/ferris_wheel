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
  void doStep();
  void stop();
};

void MyStepper::doStep() {
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
  unsigned long lastTime;
public:
  void init();
  void begin();
  void operate();
  StepperController(int iDeltaTime)
    : deltaTime(iDeltaTime) {
    lastTime = millis();
    //stepper = Stepper(stepsPerRevolution, STEPPER_IN_1, STEPPER_IN_2, STEPPER_IN_3, STEPPER_IN_4);
  };
  bool triggerMovement();
};

bool StepperController::triggerMovement() {
  moveTriggered = true;
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("StepperController triggered");
  return !isStopped;
}

void StepperController::begin() {
  if (CONTROL_STEPPER == DISABLED) return;
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("StepperController begin");
}

void StepperController::init() {
  if (CURRENT_MODE > DEBUG_MODE)
    Serial.println("StepperController init");
 
}

void StepperController::operate() {
  if (CONTROL_STEPPER == DISABLED) return;
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
      stepper.doStep();
    }
  }
}

#endif