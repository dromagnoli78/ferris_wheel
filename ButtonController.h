#ifndef ButtonController_h
#define ButtonController_h


class ButtonController {
  private:
    // const int variousPins;
  public:
    ButtonController(){
    };
  void init();
  void begin();
  void operate();
  bool isNextSongRequested();
  void setNextSongRequested(int);
};



void ButtonController::begin() {
  Serial.println("ButtonController begin");
}

void ButtonController::init(){
  
}

bool ButtonController::isNextSongRequested(){
  return true;
}

void ButtonController::setNextSongRequested(int i){
  ;
}


void ButtonController::operate(){
  
}


#endif
