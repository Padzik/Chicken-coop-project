//----------------------------------------------
// Real Time Temperature
//----------------------------------------------
void print_current_temp()
{
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  // DisplayTemp (500, 0); //used to test screen when probe not attached

  if ( !TempProbe.search(addr)) {
    TempProbe.reset_search();
    return;
  }


  TempProbe.reset();
  TempProbe.select(addr);
  TempProbe.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(750);           

  present = TempProbe.reset();
  TempProbe.select(addr);    
  TempProbe.write(0xBE);         // Read Scratchpad


  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = TempProbe.read();

  }

  int rawtemp = (data[1] << 8) + data[0];

  TReading = rawtemp;
  SignBit = TReading & 0x8000;  // test most sig bit
  lcd.setCursor(0,3);
  DisplayTemp (TReading, SignBit,1);
  CheckTempChange();
}



//----------------------------------------------
// Printing the Minimum and Maxium Teperatures
//----------------------------------------------


void print_max_min_temp()
{
  lcd.setCursor(7, 3);
  DisplayTemp(MinTReading,MinTSignBit,0);
  lcd.setCursor(14, 3);
  DisplayTemp(MaxTReading,MaxTSignBit,0);
}




void DisplayTemp(int TReading, int SignBit, int Save)  //Simple print conversion routine
{
  int Tc_100, PreD, PostD;
  char buf[6];
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25


  PreD = Tc_100 / 100;  // separate off the whole and fractional portions
  PostD = Tc_100 % 100;


  if (SignBit) // If its negative
  {
    lcd.print("-");
    if(Save) ActualTemp = (-10*PreD) + PostD; //save temp for fan function
  }
  else
  {
    lcd.print("+");
    if(Save) ActualTemp = (PreD*10) + PostD/10; //save temp for fan function
  }

  

  if (PostD < 10)
  {
    snprintf(buf, sizeof(buf), "%02d.%01d", PreD, PostD);
  }
  else 
  {
    snprintf(buf, sizeof(buf), "%02d.%01d", PreD, PostD);
  }
  lcd.print(buf);
  //lcd.print(" "); //rewrites first char of LCD: error
}




//----------------------------------------------
// Checking the Minimum and Maxium Teperatures
//----------------------------------------------


void CheckTempChange()
{
  if ( (GlobTime==0) || (millis() < 3000 ) )  // resets teperature to current temperature at 0.00am in the morning and start delay for min/max value
  {
    MaxTReading = TReading;
    MaxTSignBit = SignBit;
    MinTReading = TReading;
    MinTSignBit = SignBit;
  }
  if (TReading > MaxTReading)
  {
    MaxTReading=TReading;
    MaxTSignBit=SignBit;
  }
  if (TReading < MinTReading)
  {
    MinTReading=TReading;
    MinTSignBit=SignBit;
  }
}



