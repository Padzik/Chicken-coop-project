// Times are UK London revised on 1 Sep 2013
// Based on mid month times plus 15 mins to the nearest quarter hour - make sure you leave enough time for the chickens to go to bed - I left an addition 30mins after sunset

/*
int sun_set[24];
int sun_rise[24];
*/

//----------------------------------------------
// Start of Main Control Loop
//---------------------------------------------


void coop_initialise()
{   

   
  // Set pin modes
  pinMode(pinFan, OUTPUT);
  digitalWrite(pinFan, FANOFF);
  pinMode(pinIP1, OUTPUT);
  digitalWrite(pinIP1,LOW );
  pinMode(pinIP2, OUTPUT);
  digitalWrite(pinIP2,LOW);
  pinMode(FLED, OUTPUT);
  pinMode(pinSpeaker, OUTPUT);


  //dodelano
  pinMode(Encoder1ChnA, INPUT);  
  pinMode(Encoder1ChnB, INPUT);
  pinMode(Encoder1_SW, INPUT);

  attachInterrupt(0, encoderClick, RISING);
  attachInterrupt(1, buttonClick, RISING);

  

  //RTC_initialise(); //pak vypnout
  //Eeprom_init(); //run once

  
  
  LoadParameters(); //load from eeprom

  //loaded longitude + latitude from eeprom
  mySunrise.UpdateLocation(Latitude,Longitude,TimeZone);
  mySunrise.Actual();

  //Set initial state of the motors
  stateMotor= STOP;
  GlobTime = 0;
  CheckTempChange();

  

  mySerial.begin(9600);
  //mySerial.println("start");
  
  init_minutes();
  
  for(int k=0;k<3;k++){
    digitalWrite(FLED,HIGH);
    digitalWrite(pinSpeaker,HIGH);
    delay(150);
    digitalWrite(FLED,LOW);
    digitalWrite(pinSpeaker,LOW);
    delay(100);
  }
}



void run_controller()
{
  SetFlashOn=false;
  lcd.clear();
  get_time(); //nacita cas a blika
  OldGlobTimeSeconds=GlobTimeSeconds-1; //aby se po navratu z menu vypsal screen
  
  int LCDLightTimer=backLightTime;

  while (wait_on_escape(750)==0)
  {
    ControllerRunning = true;
    if (SetFlashOn==true) FlashLED();

    get_time(); //nacita cas a blika

    if(OldGlobTimeSeconds<=(GlobTimeSeconds-1)){ //one second tick
      OldGlobTimeSeconds=GlobTimeSeconds;

      //refresh LCD screen once in second
      lcd.setCursor(6,0);
      if(FanRun) lcd.write((uint8_t)6); //when spinning
      else if (AutoFan && Flash) lcd.write((uint8_t)7); //fan auto waiting symbol
      else lcd.print(" ");
      
      lcd.setCursor(7,0);
      lcd.print("Zavri  Otevri");
      //lcd.setCursor(0,1);
      print_time(); //tiskne cas
      print_door_time();
      lcd.setCursor(0,2);
      lcd.print("Teplota Min    Max");
      print_max_min_temp();
      print_current_temp();
    }
    
    

  
    if (WakeTime==GlobTime)door_change(DoorUp);
    if (BedTime==GlobTime) door_change(DoorDown);
    
    if (((WakeTime-1)==GlobTime) || ((BedTime-1)==GlobTime)){
      if(!mute && !Flash) digitalWrite(pinSpeaker, HIGH);
      else digitalWrite(pinSpeaker, LOW);
    } 

      
    if ((GlobTime==0) && (millis() > 70000 ) && (NewDayTimeCorection==true)) //0 minutes+running longer
      NewDayTimeCorection=false; //new day - needed new corection
    if (NewDayTimeCorection==false) RTC_correction(); //check seconds and correct  



    if(OldGlobTime<=(GlobTime-1)){ //one minute tick
      OldGlobTime=GlobTime;
      sendData(); //data serial logging
    }
    

    MotorOnTime = rtc[0];

    if (AutoFan) checkFan();

    while (stateMotor != STOP)
    {
      RTC.get(rtc,true);

      if (rtc[0]>=(MotorOnTime+MotorFailSafeClose))
        door_change(Overide);

      else
        door_change(0);
    }

    if(LCDLightTimer==0)
      digitalWrite(backLight, LOW);
      //lcd.noBacklight();
    LCDLightTimer--;
  }
}

//--------------------------------------------------
// End of Main Control Loop
//--------------------------------------------------


//--------------------------------------------------
// printing and setting the Sunrise and Sunset times
//--------------------------------------------------



void print_door_time()
{
  lcd.setCursor(7,1);
 // RTC.get(rtc,true);

  if(BedTime/60 < 10)
    lcd.print('0');  
  lcd.print(BedTime/60);
  lcd.print(":");
  if(BedTime%60 < 10)
    lcd.print('0');  
  lcd.print(BedTime%60);

  lcd.setCursor(14,1);
  //RTC.get(rtc,true);

  if(WakeTime/60 < 10)
    lcd.print('0');  
  lcd.print(WakeTime/60);
  lcd.print(":");
  if(WakeTime%60 < 10)
    lcd.print('0');  
  lcd.print(WakeTime%60);

}

////////////////////////////////////
// Set time in minutes to add to computed time of sunrise and sunset
///////////////////////////////////

void set_door_time()
{
  lcd.clear();
  lcd.print("Ranni cas dvirek + (min)");
  
  int user_input=AddRiseTime;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer=&user_input;
  myIntegerInput.low.i=0;
  myIntegerInput.high.i=200;
  myIntegerInput.step.i=5;
  myIntegerInput.col=10;
  myIntegerInput.row=2;
  myIntegerInput.width=3;
  myIntegerInput.option=1;
  input_integer(&myIntegerInput);
  AddRiseTime=user_input;   
  if(EEPROM_readInt(24)!=user_input) EEPROM_writeInt(24,user_input); //update eeprom

  lcd.clear();

  lcd.print("Vecerni cas dvirek + (min)");
  user_input=AddSetTime;
  //phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer=&user_input;
  myIntegerInput.low.i=0;
  myIntegerInput.high.i=120;
  myIntegerInput.step.i=5;
  myIntegerInput.col=10;
  myIntegerInput.row=2;
  myIntegerInput.width=3;
  myIntegerInput.option=1;
  input_integer(&myIntegerInput);
  AddSetTime=user_input;   
  if(EEPROM_readInt(28)!=user_input) EEPROM_writeInt(28,user_input); //update eeprom

  
  wait_on_escape(2000);

}



//----------------------------------------------
// Door change and set 
// the State Motor variable to Stop
//---------------------------------------------

void door_change(int action){

  if (action == Overide ){                //  MD Motor Overide Command
    digitalWrite(pinSpeaker, LOW);
    digitalWrite(pinIP1,LOW);
    digitalWrite(pinIP2,LOW);
    stateMotor = STOP;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ZKONTROLUJ DVIRKA");
    lcd.setCursor(0, 1);

    lcd.print("Cekam 42s...");     // Confirm to user limit switch had to be over writen
    lcd.setCursor(0, 2);
    lcd.print("jako ochrana dvirek!");
    lcd.setCursor(0, 3);
    lcd.print(" Lze potvrdit tlac.");

    SetFlashOn = true;
   // delay(50000); //cekej minutu
    int ticks = 0;
    do{ 
      if (butto::button_pressed==BUTTON_DOWN || butto::button_pressed==BUTTON_UP || butto::button_pressed==BUTTON_SELECT){
       // butto::button_pressed=0; //clear choice
        break; //confirm error 
      }
      delay(10);
      ticks++;
    }while(ticks < 4200);
    lcd.clear();
    return;
  }


  if (action == DoorDown ){                //  MD Motor Down Command
    if (button_check(pinLiB)==0){
      lcd.setCursor(0, 3);
      if(ControllerRunning==false)  lcd.print("Dvirka ZAVRENO    ");     // Confirm to user if manual button pressed that the door is CLOSED - shouty mode
      //digitalWrite(pinEN,LOW);
      digitalWrite(pinIP1,LOW);
      digitalWrite(pinIP2,LOW);
      digitalWrite(pinSpeaker, LOW);
      stateMotor = STOP;
      SetFlashOn=false;
      digitalWrite(FLED,HIGH);
      if(ControllerRunning==false) delay(1000);
    }
    else{
      lcd.setCursor(0, 3);
      lcd.print("Zaviram Dvirka       ");            // Symbol to show door state going down
      //digitalWrite(pinEN,HIGH);
      //analogWrite(pinIP1,DownRate);         // Used analogue write to control the rate of the door closing - don't want to loose any heads!!
      digitalWrite(pinIP1,HIGH);
      digitalWrite(pinIP2,LOW);
      if(!mute)digitalWrite(pinSpeaker, HIGH);
      stateMotor = DOWN;
    }
    return;
  }
  if (action == DoorUp ){                   // MU Motor Up Command
    if (button_check(pinLiT)==0){
      lcd.setCursor(0, 3);
      if(ControllerRunning==false) lcd.print("Dvirka OTEVRENO");       // Confirm to user if manual button pressed that the door is OPEN - shouty mode
      //digitalWrite(pinEN,LOW);
      
      digitalWrite(pinIP1,LOW);
      digitalWrite(pinIP2,LOW);
      digitalWrite(pinSpeaker, LOW);
      stateMotor = STOP;
      SetFlashOn=false;
      digitalWrite(FLED,LOW);
      if(ControllerRunning==false) delay(1000);
    }
    else{
      lcd.setCursor(0, 3);
      lcd.print("Oteviram Dvirka     ");            // Symbol to show door state going up
      //digitalWrite(pinEN,HIGH);
      digitalWrite(pinIP1,LOW);
      digitalWrite(pinIP2,HIGH);            // analogWrite not used as you need as much power you can get to raise the door quickly
      if(!mute)digitalWrite(pinSpeaker, HIGH);
      stateMotor = UP;
    }
    return;
  }
  else {
    if (stateMotor== DOWN && button_check(pinLiB) == 0){
      lcd.setCursor(0, 3);
      lcd.print("Dvirka Zavreno      ");
      //digitalWrite(pinEN,LOW);
     // delay(500); //pro zavreni zamku 0,1s navic
      digitalWrite(pinIP1,LOW);
      digitalWrite(pinIP2,LOW);
      digitalWrite(pinSpeaker, LOW);
      stateMotor = STOP;
      delay(500);
    }
    if (stateMotor== UP && button_check(pinLiT) == 0){
      lcd.setCursor(0, 3);
      lcd.print("Dvirka Otevreno     ");
      //digitalWrite(pinEN,LOW);
      
      digitalWrite(pinIP1,LOW);
      digitalWrite(pinIP2,LOW);
      digitalWrite(pinSpeaker, LOW);
      stateMotor = STOP;
      SetFlashOn=false;
      digitalWrite(FLED,LOW);
      delay(500);
    }
  }
}



//----------------------------------------------
// Keyboard and limiter switches detect rountine.  
// Analogue 1 - Limter switch Top
// Analogue 0 - Limter switch Bottom
//---------------------------------------------


int button_check(int LimiterSw) 
{
  //resi pouze dvirka

  //spatne: nula nic, jednicka magnet pobliz, snimano zezadu cidla kvuli polarite.
  //spravne: jednicka nic tam neni, nula magnet pobliz. snimano zezadu cidla kvuli polarite.

  #ifdef INVERTED_DOORS
  if(LimiterSw==88) return(analogRead(A6)  > 100 ? 0 : 1);
  else return(!digitalRead(pinLiB));
  #else
  //normalne
  if(LimiterSw==88) return(analogRead(A6)  > 100 ? 1 : 0);
  else return(digitalRead(pinLiB));
  #endif
}


void show_credit()
{
  // Display credits
  phi_prompt_struct myLongMsg;

  lcd.clear();
  lcd.noBlink();
  center_text("O tvurci"); // Prompt user for input. Use center_text to display center-aligned.
  myLongMsg.ptr.msg_P=msg_00; // Assign the address of the text string to the pointer.
  myLongMsg.low.i=0; // Default text starting position. 0 is highly recommended.
  myLongMsg.high.i=strlen_P(msg_00); // Position of the last character in the text string, which is size of the string - 1.
  myLongMsg.step.c_arr[0]=lcd_rows-1; // rows to auto fit entire screen
  myLongMsg.step.c_arr[1]=lcd_columns-1; // one col list
  myLongMsg.col=0; // Display the text area starting at column 0
  myLongMsg.row=1; // Display the text area starting at row 0
  myLongMsg.option=1; // Option 0, display classic message, option 1, display message with scroll bar on right.

  text_area_P(&myLongMsg);
}

void FlashLED()
{
  if (toggle==1)
  {  
    digitalWrite(FLED,LOW);
    toggle=0;
  }
  else
  {
    digitalWrite(FLED,HIGH);
    toggle=1;
  }

}
























































