#ifndef StepperController_h
#define StepperController_h

#include <CheapStepper.h>
class StepperController {
  private:
    const int stepsPerRevolution;
    const int stSpeed;
    ButtonController* buttonController;
    CheapStepper* stepper;
    boolean moveClockwise = true;
    long deltaTime = 100;
    long lastTime;
  public:
    void init();
  void begin();
  void operate();
    StepperController(const int iStepsPerRevolution, const int iStSpeed, ButtonController *pButtonController):
    stepsPerRevolution(iStepsPerRevolution), stSpeed(iStSpeed){
      buttonController = pButtonController;
      lastTime = millis();
    };

};



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
   stepper->moveDegrees(true, 1);
    this->lastTime = millis();
  }
  
}
  
#endif
