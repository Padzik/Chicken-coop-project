//eeprom functions

void Eeprom_init(){ //run first time
     EEPROM_writeInt(0,90); //init eeprom value backLightTime
     EEPROM_writeInt(4,12); //MotorFailSafeOpen
     EEPROM_writeInt(8,7); //MotorFailSafeClose
     EEPROM_writeInt(20,41);//TimeCorection
     EEPROM_writeInt(24,0);//AddRiseTime
     EEPROM_writeInt(28,30);//AddSetTime
     EEPROM_writeInt(32,30);//FanTemp
     EEPROM_writeInt(36,20);//FanTime 
     EEPROM_writeInt(40,1);//TimeZone  
     EEPROM_writeInt(44,49);//Lat_pre  
     EEPROM_writeInt(48,18);//Lat_suf  
     EEPROM_writeInt(52,16);//Lon_pre  
     EEPROM_writeInt(56,53);//Lon_suf 
     EEPROM_writeInt(60,1);//Mute  
}

void menu_eepromInit(){
  char buffer[32];
  int yn;
  lcd.clear();
  snprintf(buffer, sizeof(buffer), "Nacist vychozi hodnoty?"); 
  yn=yn_dialog(buffer);
    lcd.clear();
    lcd.setCursor(0,2);
  delay(500);
  if (yn==1){ //1=yes,0==no
    RTC_initialise(); //reset RTC
    Eeprom_init();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Vychozi hodnoty");
    lcd.setCursor(0,1);
    lcd.print("   nacteny.");
   	lcd.setCursor(0,3);
    lcd.print("Restart za 2s."); 
    delay(500);
    lcd.print(".");  	 
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print("."); 
    delay(500);
    //reboot
    softReset();
  }
  else{
    lcd.clear();
    lcd.setCursor(0,2);
    lcd.print("     Nic nenacteno!");
  }
  wait_on_escape(1500); 
}


void softReset(){
  asm volatile ("  jmp 0");
}


void LoadParameters(){
  // Load from EEPROM
   int e_backLightTime = EEPROM_readInt(0);
   int e_MotorFailSafeOpen = EEPROM_readInt(4);
   int e_MotorFailSafeClose = EEPROM_readInt(8);
   int e_TimeCorection = EEPROM_readInt(20);
   int e_AddRiseTime = EEPROM_readInt(24);
   int e_AddSetTime = EEPROM_readInt(28);   
   int e_FanTemp = EEPROM_readInt(32);
   int e_FanTime = EEPROM_readInt(36);
   int e_TimeZone = EEPROM_readInt(40);
   int e_Lat_pre = EEPROM_readInt(44);
   int e_Lat_suf = EEPROM_readInt(48);
   int e_Lon_pre = EEPROM_readInt(52);
   int e_Lon_suf = EEPROM_readInt(56);
   int e_mute = EEPROM_readInt(60);
   
   if (isnan(e_backLightTime)) {
     backLightTime=90;
   }
   else backLightTime=e_backLightTime; //load

   if (isnan(e_MotorFailSafeOpen)) {
     MotorFailSafeOpen=7;
   }
   else MotorFailSafeOpen=e_MotorFailSafeOpen; //load

   if (isnan(e_MotorFailSafeClose)) {
     MotorFailSafeClose=4;
   }
   else MotorFailSafeClose=e_MotorFailSafeClose; //load


   if (isnan(e_TimeCorection)) {
     TimeCorection=50;
   }
   else TimeCorection=e_TimeCorection; //load   

   if (isnan(e_FanTemp)) {
     FanTemp=35;
   }
   else FanTemp=e_FanTemp; //load 
   
   if (isnan(e_FanTime)) {
     FanTime=30;
   }
   else FanTime=e_FanTime; //load   

   if (isnan(e_AddRiseTime)) {
     AddRiseTime=60;
   }
   else AddRiseTime=e_AddRiseTime; //load 
   
   if (isnan(e_AddSetTime)) {
     AddSetTime=20;
   }
   else AddSetTime=e_AddSetTime; //load 

   if (isnan(e_TimeZone)) {
     TimeZone=1;
   }
   else TimeZone=e_TimeZone; //load 

   if (isnan(e_Lat_pre)) {
     Lat_pre=49;
   }
   else Lat_pre=e_Lat_pre; //load 

   if (isnan(e_Lat_suf)) {
     Lat_suf=18;
   }
   else Lat_suf=e_Lat_suf; //load 
 
  if (isnan(e_Lon_pre)) {
     Lon_pre=16;
   }
   else Lon_pre=e_Lon_pre; //load 

  if (isnan(e_Lon_suf)) {
     Lon_suf=53;
   }
   else Lon_suf=e_Lon_suf; //load 

  if (isnan(e_mute)) {
     mute=true;
   }
   else mute=e_mute; //load 

  Latitude=Lat_pre+Lat_suf/100; //float convertion
  Longitude=Lon_pre+Lon_suf/100;
}




// ************************************************
void EEPROM_writeInt(int address, int value)
{
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
      EEPROM.write(address++, *p++);
   }
}
// ************************************************
int EEPROM_readInt(int address)
{
   int value = 0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
      *p++ = EEPROM.read(address++);
   }
   return value;
}
