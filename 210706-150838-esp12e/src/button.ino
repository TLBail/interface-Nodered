
bool isLeftBoutonPushed = false; 

bool isRightBoutonPushed = false; 

bool isLeftButtonToggle = true;

bool isRightButtonToggle = true;

unsigned long leftlastTimePush = 0;
unsigned long rightlastTimePush = 0;
unsigned long timeForLongPress = 500; //en millis



void checkButton(){
  
  //check left button
  isLeftBoutonPushed = !(digitalRead(leftboutonPin));
  //Serial.println("left button" + isLeftBoutonPushed);
  if(isLeftBoutonPushed && isLeftButtonToggle){
    //Button Press Down
    isLeftButtonToggle= false;
    leftlastTimePush = millis();
    onButtonPressDown();

  }else if(!isLeftBoutonPushed && !isLeftButtonToggle){
    //Button Press UP
    isLeftButtonToggle = true;
    
    unsigned long elaspedTime = millis() - leftlastTimePush;

    if(elaspedTime < timeForLongPress){
      Serial.println("petite pression");
      onLeftButtonPressUp();
    
    }else{
      Serial.println("grosse pression");
      onLeftButtonPressUp();
    }
  }
  


  //check right button
  isRightBoutonPushed = !(digitalRead(rightboutonPin));
  //Serial.println("right button" +isRightBoutonPushed);
  if(isRightBoutonPushed  && isRightButtonToggle){
    //Button Press Down
    isRightButtonToggle = false;
    rightlastTimePush = millis();
    onButtonPressDown();

  }else if(!isRightBoutonPushed  && !isRightButtonToggle){
    //Button Press UP
    isRightButtonToggle = true;
    
    unsigned long elaspedTime = millis() - rightlastTimePush;

    if(elaspedTime < timeForLongPress){
      Serial.println("petite pression2");
      onRightButtonPressUp();
    
    }else{
      Serial.println("grosse pression2");
      onRightButtonPressUp();
    }
  }

}
