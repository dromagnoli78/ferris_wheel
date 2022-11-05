#ifndef StepperController_h
#define StepperController_h

#include "Constants.h"
#include <Stepper.h>

#define STEPPER_INCREMENT 1

class StepperController {
private:
  int stepsPerRevolution;
  int stSpeed;
  bool moveTriggered = false;
  bool isStopped = true;
  Stepper stepper = Stepper(STEPS_PER_REVOLUTION, STEPPER_IN_1, STEPPER_IN_2, STEPPER_IN_3, STEPPER_IN_4);
  bool moveClockwise = true;
  long deltaTime;
  long lastTime;
public:
  void init();
  void begin();
  void operate();
  StepperController(int iStepsPerRevolution, int iStSpeed, int iDeltaTime)
    : stepsPerRevolution(iStepsPerRevolution), stSpeed(iStSpeed), deltaTime(iDeltaTime) {
    lastTime = millis();
    //stepper = Stepper(stepsPerRevolution, STEPPER_IN_1, STEPPER_IN_2, STEPPER_IN_3, STEPPER_IN_4);
  };
  bool triggerMovement();
};

bool StepperController::triggerMovement() {
  moveTriggered = true;
   if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("StepperController triggered");
  return !isStopped;
}

void StepperController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("StepperController begin");
  //stepper = Stepper(STEPS_PER_REVOLUTION, STEPPER_IN_1, STEPPER_IN_2, STEPPER_IN_3, STEPPER_IN_4);
  //stepper->setRpm(STEPPER_RPM);
  stepper.setSpeed(stSpeed);
}

void StepperController::init() {
    if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("StepperController init");
  //stepper->setSpeed(stSpeed);
  //stepper->step(100);
}

void StepperController::operate() {
  if (CONTROL_STEPPER == DISABLED) return;
  long time = millis();
  if ((time - this->lastTime) > deltaTime) {
    this->lastTime = millis();
    if (moveTriggered) {
      moveTriggered = false;
      if (isStopped) {
        isStopped = false;
        stepper.setSpeed(stSpeed);
      } else {
        isStopped = true;
        digitalWrite(STEPPER_IN_1, LOW);
        digitalWrite(STEPPER_IN_2, LOW);
        digitalWrite(STEPPER_IN_3, LOW);
        digitalWrite(STEPPER_IN_4, LOW);

        return;
      }
    }
    if (!isStopped) {
      stepper.step(STEPPER_INCREMENT);
      //stepper.move(1);
    }
  }
}

#endif