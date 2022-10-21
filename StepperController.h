#ifndef StepperController_h
#define StepperController_h

#include "Constants.h"
#include <CheapStepper.h>

class StepperController {
  private:
    const int8_t stepsPerRevolution;
    const int stSpeed;
    bool moveTriggered = false;
    bool isStopped = false;
    CheapStepper* stepper;
    bool moveClockwise = true;
    long deltaTime = 50;
    long lastTime;
  public:
    void init();
    void begin();
    void operate();
    StepperController(const int iStepsPerRevolution, const int iStSpeed):
    stepsPerRevolution(iStepsPerRevolution), stSpeed(iStSpeed){
      lastTime = millis();
    };
    bool triggerMovement();

};

bool StepperController::triggerMovement() {
  moveTriggered = true;
  return !isStopped;
}

void StepperController::begin() {
  if (CURRENT_MODE == DEBUG_MODE)
    Serial.println("StepperController begin");
  stepper = new CheapStepper();
  stepper->setRpm(STEPPER_RPM); 
}

void StepperController::init(){
  //stepper->setSpeed(stSpeed);
  //stepper->step(100);  
}

void StepperController::operate(){
  long time = millis();
  if ((time - this->lastTime) > deltaTime) {
    if(moveTriggered) {
      if (isStopped) {
        isStopped = false;
      } else {
        isStopped = true;
        return;
      }
      moveTriggered = false;
    }
    if (!isStopped) {
       stepper->moveCW(STEPPER_INCREMENT);
       this->lastTime = millis();
    }
  }
  
}
  
#endif
