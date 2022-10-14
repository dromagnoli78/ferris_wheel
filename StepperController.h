#ifndef StepperController_h
#define StepperController_h

#include "Constants.h"
#include <CheapStepper.h>

#define INCREMENT 3

class StepperController {
  private:
    const int stepsPerRevolution;
    const int stSpeed;
    bool moveTriggered = false;
    bool isStopped = false;
    CheapStepper* stepper;
    boolean moveClockwise = true;
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
  Serial.println("StepperController begin");
  stepper = new CheapStepper();
  stepper->setRpm(20); 
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
       stepper->moveCW(INCREMENT);
       this->lastTime = millis();
    }
  }
  
}
  
#endif
