// Fan controller

void set_AutoFanOn(){  
  FanRun=true;
  AutoFan=true;
  FanLastSpin=GlobTime;
  
  lcd.setCursor(0, 3);
  lcd.print(" Automatika zapnuta "); 
  delay(1000);
}


void set_AutoFanOff(){
  FanRun=false;
  AutoFan=false;
  lcd.setCursor(0, 3);
  lcd.print(" Automatika vypnuta ");
  delay(1000);
}


void set_FanOn(){
  FanRun=true;
  digitalWrite(pinFan,FANON);
  
  lcd.setCursor(0, 3);
  lcd.print(" Vetrak zapnut "); 
  delay(1000);
}


void set_FanOff(){
  FanRun=false;
  digitalWrite(pinFan,FANOFF);
  
  lcd.setCursor(0, 3);
  lcd.print(" Vetrak vypnut    "); 
  delay(1000);  
}

/*
void checkFan(){
  int month = rtc[5];
  if ((FanTemp<=ActualTemp) && (5<=month && month<=9)){
    //month May,Jun,Jul,Aug,Sep and temp greater than set T

    if(millis()<(FanLastSpin+FanTime*60000)) {
      //start spinning
      digitalWrite(pinFan,FANON);
      FanRun = true;   
    }
    else {if(millis()<(FanLastSpin+FanTime*120000)) {
      //pause spinning
      digitalWrite(pinFan,FANOFF);
      FanRun = false;     
      
    }
    else{
      //reset time counter
      FanLastSpin=millis();
    }
    }
   }
 else {
   digitalWrite(pinFan,FANOFF);
   FanRun = false;
   }
}
*/


void checkFan(){
  int month = rtc[5];
  if ((FanTemp<=ActualTemp/10) && (5<=month && month<=9)){
    //month May,Jun,Jul,Aug,Sep and temp greater than set T

    if(GlobTime<(FanLastSpin+FanTime)) {
      //start spinning
      digitalWrite(pinFan,FANON);
      FanRun = true;   
    }
    else {
      if(GlobTime<(GlobTime+FanTime*2)) {
        //pause spinning
        digitalWrite(pinFan,FANOFF);
        FanRun = false;     
      }
      else{
        //reset time counter
        FanLastSpin=GlobTime;
      }
    }
   }
 else {
   digitalWrite(pinFan,FANOFF);
   FanRun = false;
   }
}


void set_FanTemp(){
  lcd.clear();
  lcd.print("Teplota sepnuti");

  int user_input=FanTemp;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer=&user_input;
  myIntegerInput.low.i=15;
  myIntegerInput.high.i=50;
  myIntegerInput.step.i=1;
  myIntegerInput.col=10;
  myIntegerInput.row=2;
  myIntegerInput.width=3;
  myIntegerInput.option=1;
  input_integer(&myIntegerInput);
  FanTemp=user_input;   
  if(EEPROM_readInt(32)!=user_input) EEPROM_writeInt(32,user_input); //update eeprom
}

void set_fanTime(){
  lcd.clear();
  lcd.print("Delka toceni (min)");

  int user_input=FanTime;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer=&user_input;
  myIntegerInput.low.i=1;
  myIntegerInput.high.i=180;
  myIntegerInput.step.i=1;
  myIntegerInput.col=10;
  myIntegerInput.row=2;
  myIntegerInput.width=3;
  myIntegerInput.option=1;
  input_integer(&myIntegerInput);
  FanTime=user_input; 
  if(EEPROM_readInt(36)!=user_input) EEPROM_writeInt(36,user_input); //update eeprom  
}

