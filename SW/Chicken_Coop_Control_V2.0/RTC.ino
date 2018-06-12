//RTC se zpozdujou o 50s/1den


const int BST[36] ={
// BST  Non-BST start in Mar 2013 Oct 2030
     31,  27,
     30,  26,
     29,  25,
     27,  30,
     26,  29,
     25,  28,
     31,  27,
      29, 25,
      28, 31,
      27, 30,
      26, 29,
      31, 27,
      30, 26,
      29, 25,
      28, 31,
      26, 29,
      25, 28,
      31, 27,
  };


void RTC_initialise()
{

 //  mySerial.println ("RTC init fce");
  
  DDRC|=_BV(2) |_BV(3);  // POWER:Vcc Gnd
  PORTC |=_BV(3);  // VCC PINC3

    RTC.stop();
  RTC.set(DS1307_SEC,0);
  RTC.set(DS1307_MIN,0);
  RTC.set(DS1307_HR,0);
  RTC.set(DS1307_DOW,1);
  RTC.set(DS1307_DATE,1);
  RTC.set(DS1307_MTH,1);
  RTC.set(DS1307_YR,18);
  RTC.start();
}

void init_minutes(){
  //mySerial.println ("init minutes");
  int local_hour;
  RTC.get(rtc,true);
  if(BST_flag==false) local_hour=rtc[2];
  else {
    if (rtc[2]+1==24)local_hour=0;
    else local_hour=rtc[2]+1;
  }
  OldGlobTime=(local_hour*60)+rtc[1]; //cas v minutach od pulnoci
  OldGlobTimeSeconds = (long)GlobTime*60ul+rtc[0]; //cas v sekundach od pulnoci  //cislo a 00ul pro 
  
}

void get_time(){
 // mySerial.println ("RTC get time fce");
  RTC.get(rtc,true);
  int local_hour;
  check_BST();
  
  if(BST_flag==false) local_hour=rtc[2];
  else {
    if (rtc[2]+1==24) local_hour=0;
    else local_hour=rtc[2]+1;
  }

  lcd.setCursor(2,1);
  if (Flash == 0) 
  {      
    lcd.print(":");
    Flash = 1;
  } 
  else {
    lcd.print(" ");
    Flash = 0;
  }

  BedTime= mySunrise.Set(rtc[5],rtc[4])+AddSetTime; //time in minutes
  WakeTime= mySunrise.Rise(rtc[5],rtc[4])+AddRiseTime; //Rise(month, day)
  if(BST_flag){
     BedTime+=60; //add 1 hour for summer time
     WakeTime+=60; 
  }
  GlobTime=(local_hour*60)+rtc[1]; //cas v minutach od pulnoci
  GlobTimeSeconds = GlobTime*60+rtc[0]; //cas v sekundach od pulnoci


  if ((rtc[2]==0)  && (rtc[1]==0) && (rtc[0]==0)){ //new day counter reset
    OldGlobTime=0;
    OldGlobTimeSeconds=0;
  }


  if((rtc[2]==45) && (rtc[1]==85) && (rtc[0]==85)){ //RTC not running / connected
      lcd.clear();
      lcd.setCursor(0,0);  
      lcd.print("  Nebezi RTC modul!");
      lcd.setCursor(0,2);  
      lcd.print("Zkontroluj zapojeni.");
      delay(1000);
  }

  
}

void print_time()
{
 // mySerial.println ("RTC print time");
  RTC.get(rtc,true);
  int local_hour;
  check_BST();
  
  if(BST_flag==false) local_hour=rtc[2];
  else {
    if (rtc[2]+1==24) local_hour=0;
    else local_hour=rtc[2]+1;
  }


  lcd.setCursor(0,0);  
  if(BST_flag==true) lcd.print("Letni");
  else lcd.print("Zimni");
  
    lcd.setCursor(0,1);
    if(local_hour < 10)
      lcd.print('0');
    lcd.print(local_hour); 
    lcd.setCursor(3,1);
    if(rtc[1] < 10)
     lcd.print('0');  
    lcd.print(rtc[1]);
 
}



void set_time_date()
{
 // mySerial.println ("RTC set time date fce");
  lcd.clear();
  lcd.print("SEC   Datum Rok");
  lcd.setCursor(0,1);
  RTC.get(rtc,true);
  RTC.stop();

  lcd.print(rtc[2]);
  if(rtc[2] < 10)
    lcd.print('0');  
  lcd.print(":");
  if(rtc[1] < 10)
    lcd.print('0');  
  lcd.print(rtc[1]);

  int user_input=rtc[2]; // Set input for RTC HR
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer=&user_input;
  myIntegerInput.low.i=0;
  myIntegerInput.high.i=23;
  myIntegerInput.step.i=1;
  myIntegerInput.col=0;
  myIntegerInput.row=1;
  myIntegerInput.width=2;
  myIntegerInput.option=1;
  input_integer(&myIntegerInput);
  RTC.set(DS1307_HR,user_input);

  user_input=rtc[1]; // Set input to be RTC MIN

  myIntegerInput.low.i=0;
  myIntegerInput.high.i=59;
  myIntegerInput.col=3;
  input_integer(&myIntegerInput); 
  RTC.set(DS1307_MIN,user_input);

  lcd.setCursor(6,1);
  if(rtc[4] < 10)
    lcd.print('0');  
  lcd.print(rtc[4]);
  lcd.print("/");
  if(rtc[5] < 10)
    lcd.print('0');  
  lcd.print(rtc[5]);

  user_input=rtc[4]; // Set input for RTC DATE
  myIntegerInput.low.i=1;
  myIntegerInput.high.i=31;
  myIntegerInput.col=6;
  input_integer(&myIntegerInput);
  RTC.set(DS1307_DATE,user_input);

  user_input=rtc[5]; // Set input to be RTC MTH
  myIntegerInput.high.i=12;
  myIntegerInput.col=9;
  input_integer(&myIntegerInput); 
  RTC.set(DS1307_MTH,user_input);

  user_input=rtc[6]; // Set input to be RTC YR
  lcd.setCursor(12,1);
  lcd.print(user_input);
  user_input=user_input-2000;
  myIntegerInput.low.i=17;
  myIntegerInput.high.i=50;
  myIntegerInput.col=14;
  input_integer(&myIntegerInput); 
  RTC.set(DS1307_YR,user_input);

  RTC.start();
  wait_on_escape(2000);

}




void check_BST()
{
   
  int BST_start_day = BST[(rtc[6]-2013)*2];
  int BST_finish_day = BST[((rtc[6]-2013)*2)+1];
  //lcd.setCursor(0,0);
  int BST_start_date=(300+BST_start_day);
  int BST_finish_date=(1000+BST_finish_day);
  int Today=((rtc[5]*100)+rtc[4]);         //crude convert date to number for comparison

  if ((Today>= BST_start_date && Today <= BST_finish_date))
  {
    //lcd.print("Letni");
    BST_flag=true;
  }
  else
  {
    //lcd.print("Zimni");
    BST_flag=false;
  }
}


void RTC_correction()
{
 // mySerial.println ("RTC correction fce");
 //every new day do correction
  RTC.get(rtc,true);
  int seconds_now = rtc[0]; //get seconds
  int day_now = rtc[3]; //get day of week
  
  if( seconds_now == 58){ //58 second on second counter
    //do time corection
    if(TimeCorection!=0){ //correction needed
      if ((LastDayCorection - day_now)!=0){ //was not corrected today
        lcd.clear(); //vlozeno - jednou za den vymazat displej
        RTC.stop();
        int correct= seconds_now - TimeCorection; 
        RTC.set(DS1307_SEC,correct);
        RTC.start();
        LastDayCorection = day_now;
      }
    }
    NewDayTimeCorection = true; 
  }
  
}














