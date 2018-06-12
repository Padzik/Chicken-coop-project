/*
  Chicken Coop menu Structure

*/

// Menu texts
const char top_menu_item00[] PROGMEM = "Spust AutomatRezim";
const char top_menu_item01[] PROGMEM = "Dvirka nahoru";
const char top_menu_item02[] PROGMEM = "Dvirka dolu";
const char top_menu_item03[] PROGMEM = "Ovladac vetraku";
const char top_menu_item04[] PROGMEM = "Korekc. Cas Dvirek";
const char top_menu_item05[] PROGMEM = "Nastav. Cas/Datum";
const char top_menu_item06[] PROGMEM = "Nastaveni Systemu";
const char top_menu_item07[] PROGMEM = "O tvurci+navod";
const char * const top_menu_items[] PROGMEM  = {
  top_menu_item00, top_menu_item01, top_menu_item02, top_menu_item03, top_menu_item04, top_menu_item05, top_menu_item06, top_menu_item07
};

const char sub_menu_1_item00[] PROGMEM = "Zpatky do hlavniho menu";
const char sub_menu_1_item01[] PROGMEM = "Nastaveni Dvirek";
const char sub_menu_1_item02[] PROGMEM = "LCD Podsviceni";
const char sub_menu_1_item03[] PROGMEM = "LED trvale zapnout";
const char sub_menu_1_item04[] PROGMEM = "LED vypnout";
const char sub_menu_1_item05[] PROGMEM = "RTC korekcni clen";
const char sub_menu_1_item06[] PROGMEM = "Pipani nastaveni";
const char sub_menu_1_item07[] PROGMEM = "Nacist vychozi hodnoty";
const char * const sub_menu_1_items[] PROGMEM  = {
  sub_menu_1_item00, sub_menu_1_item01, sub_menu_1_item02, sub_menu_1_item03, sub_menu_1_item04, sub_menu_1_item05, sub_menu_1_item06, sub_menu_1_item07
};

const char sub_menu_2_item00[] PROGMEM = "Zpatky do hlavniho menu";
const char sub_menu_2_item01[] PROGMEM = "Zapnout Vetrak";
const char sub_menu_2_item02[] PROGMEM = "Vypnout Vetrak";
const char sub_menu_2_item03[] PROGMEM = "Zapnout automatiku";
const char sub_menu_2_item04[] PROGMEM = "Vypnout automatiku";
const char sub_menu_2_item05[] PROGMEM = "Nast spinaci tepl";
const char sub_menu_2_item06[] PROGMEM = "Nast cas toceni";
const char * const sub_menu_2_items[] PROGMEM  = {
  sub_menu_2_item00, sub_menu_2_item01, sub_menu_2_item02, sub_menu_2_item03, sub_menu_2_item04, sub_menu_2_item05, sub_menu_2_item06
};

const char sub_menu_3_item00[] PROGMEM = "Zpatky do hlavniho menu";
const char sub_menu_3_item01[] PROGMEM = "Otviraci Cas";
const char sub_menu_3_item02[] PROGMEM = "Zaviraci Cas";
const char sub_menu_3_item03[] PROGMEM = "Zemepisna Sirka";
const char sub_menu_3_item04[] PROGMEM = "Zemepisna Delka";
const char sub_menu_3_item05[] PROGMEM = "Casova Zona";
const char * const sub_menu_3_items[] PROGMEM  = {
  sub_menu_3_item00, sub_menu_3_item01, sub_menu_3_item02, sub_menu_3_item03, sub_menu_3_item04, sub_menu_3_item05
};

const char sub_menu_4_item00[] PROGMEM = "Zpatky do nastaveni";
const char sub_menu_4_item01[] PROGMEM = "Pipani zapnuto";
const char sub_menu_4_item02[] PROGMEM = "Pipani vypnuto";
const char sub_menu_4_item03[] PROGMEM = "Test pipani";
const char * const sub_menu_4_items[] PROGMEM  = {
  sub_menu_4_item00, sub_menu_4_item01, sub_menu_4_item02, sub_menu_4_item03
  };


//This program is the main menu. It handles inputs from the keys, updates the menu or executes a certain menu function accordingly.
int global_style = 105; // This is the style of the menu


void top_menu()
{
  int menu_pointer_1 = 0; // This stores the menu choice the user made.
  phi_prompt_struct myMenu; // This structure stores the main menu.

  // Initialize the top menu
  myMenu.ptr.list = (char**)&top_menu_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 7; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4 - ((global_style & phi_prompt_index_list) != 0); // x for additional feature such as an index
  myMenu.col = 0; // Display menu at column 0
  myMenu.row = 1; // Display menu at row 1
  myMenu.option = global_style; // Option 0, display classic list, option 1, display 2X2 list, option 2, display list with index, option 3, display list with index2.

  digitalWrite(backLight, HIGH);
  //lcd.backlight();
  run_controller();

  while (1) // This loops every time a menu item is selected.
  {
    
    
    ControllerRunning = false;
    lcd.clear();  // Refresh menu if a button has been pushed
    digitalWrite(backLight, HIGH);
    //lcd.backlight();
    center_text(" Ovladac Kurniku");



    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // This makes sure the change of menu style is updated if there is a menu style option in this level of menu. Auto fit the size of the list to the screen. Length in characters of the longest list item.
    myMenu.step.c_arr[3] = lcd_columns - 4 - ((global_style & phi_prompt_index_list) != 0); // This makes sure the change of menu style is updated if there is a menu style option in this level of menu. x for additional feature such as an index
    select_list(&myMenu); // Use the select_list to ask the user to select an item of the list, that is a menu item from your menu.
    menu_pointer_1 = myMenu.low.i; // Get the selected item number and store it in the menu pointer.
    
    //if(wait_on_escape(2000)==NO_KEYs) break; //nevali
   // if(!wait_on_escape(2000)) break; //nevali

    
    switch (menu_pointer_1) // See which menu item is selected and execute that correspond function
    {
      case 0:
        run_controller();
        break;

      case 1:
        set_door_up();
        break;

      case 2:
        set_door_down();
        break;

      case 3:
        set_fan();
        break;

      case 4:
        set_door_time();
        break;

      case 5:
        set_time_date();
        break;

      case 6:
        settings_menu();
        break;

      case 7:
        show_credit();
        break;

      default:
        break;
    }
  }
  //nove
   OldGlobTimeSeconds=GlobTimeSeconds-1; //aby se po navratu z menu vypsal screen
}


void set_door_up()
{
  RTC.get(rtc, true);
  MotorOnTime = rtc[0];
  door_change(DoorUp);

  while (stateMotor != STOP)
  {
    RTC.get(rtc, true);

    if (rtc[0] >= (MotorOnTime + MotorFailSafeOpen))
      door_change(Overide);

    else
      door_change(0);
  }

}



void set_door_down()
{
  RTC.get(rtc, true);
  MotorOnTime = rtc[0];
  door_change(DoorDown);

  while (stateMotor != STOP)
  {
    RTC.get(rtc, true);

    if (rtc[0] >= (MotorOnTime + MotorFailSafeClose))
      door_change(Overide);

    else
      door_change(0);
  }

}


void settings_menu() { //this is settings menu
  int menu_pointer_1 = 0;
  phi_prompt_struct myMenu; // This structure stores the sub menu.

  // Initialize the top menu
  myMenu.ptr.list = (char**)&sub_menu_1_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 7; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4; // x for additional feature such as an index
  myMenu.col = 0; // Display prompt at column 0
  myMenu.row = 1; // Display prompt at row 1
  myMenu.option = global_style;

  while (1)
  {
    lcd.clear();  // Refresh menu if a button has been pushed
    center_text("Nastaveni");

    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.


    select_list(&myMenu);
    menu_pointer_1 = myMenu.low.i;
    switch (menu_pointer_1)
    {
      case 0:
        return;
        break;

      case 1:
        door_parameters();
        break;

      case 2:
        sub_menu_BacklightTimer();
        break;

      case 3:
        sub_menu_LED_ON();
        break;

      case 4:
        sub_menu_LED_OFF();
        break;

      case 5:
        sub_menu_RTC_correction_set();
        break;

      case 6:
        beep_menu();
        break;

      case 7:
        menu_eepromInit();
        break;


      default:
        break;
    }
  }


}


void beep_menu(){
  int menu_pointer_4 = 0;
  phi_prompt_struct myMenu; // This structure stores the sub menu.

  // Initialize the top menu
  myMenu.ptr.list = (char**)&sub_menu_4_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 3; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4; // x for additional feature such as an index
  myMenu.col = 0; // Display prompt at column 0
  myMenu.row = 1; // Display prompt at row 1
  myMenu.option = global_style;

  while (1)
  {
    lcd.clear();  // Refresh menu if a button has been pushed
    center_text("Nastaveni pipani");

    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.


    select_list(&myMenu);
    menu_pointer_4 = myMenu.low.i;
    switch (menu_pointer_4)
    {
      case 0:
        return;
        break;

      case 1:
        set_beep(true);
        break;

      case 2:
        set_beep(false);
        break;

      case 3:
        beep_test();
        break;

      default:
        break;
    }
  }
}


void door_parameters() // This is the sub_menu_1
{
  int menu_pointer_1 = 0;
  phi_prompt_struct myMenu; // This structure stores the sub menu.

  // Initialize the top menu
  myMenu.ptr.list = (char**)&sub_menu_3_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 5; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4; // x for additional feature such as an index
  myMenu.col = 0; // Display prompt at column 0
  myMenu.row = 1; // Display prompt at row 1
  myMenu.option = global_style;

  while (1)
  {
    lcd.clear();  // Refresh menu if a button has been pushed
    center_text("Parametry Dvirek");

    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.


    select_list(&myMenu);
    menu_pointer_1 = myMenu.low.i;
    switch (menu_pointer_1)
    {
      case 0:
        return;
        break;

      case 1:
        sub_menu_setOpenTime();
        break;

      case 2:
        sub_menu_setCloseTime();
        break;

      case 3:
        sub_menu_setLatitude();
        break;

      case 4:
        sub_menu_setLongitude();
        break;

      case 5:
        sub_menu_setTimeZone();
        break;

      default:
        break;
    }
  }
}

/////
void sub_menu_RTC_correction_set()
{
  lcd.clear();
  lcd.print("Kazdy den zpomalit hodiny o (s)");

  int user_input = TimeCorection;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer = &user_input;
  myIntegerInput.low.i = 0;
  myIntegerInput.high.i = 55;
  myIntegerInput.step.i = 1;
  myIntegerInput.col = 10;
  myIntegerInput.row = 2;
  myIntegerInput.width = 3;
  myIntegerInput.option = 1;
  input_integer(&myIntegerInput);
  TimeCorection = user_input;
  if (EEPROM_readInt(20) != user_input) EEPROM_writeInt(20, user_input); //update eeprom
}

//////////////////

void set_fan() // This is the sub_menu_2
{
  int menu_pointer_2 = 0;
  phi_prompt_struct myMenu; // This structure stores the sub menu.

  // Initialize the top menu
  myMenu.ptr.list = (char**)&sub_menu_2_items; // Assign the list to the pointer
  myMenu.low.i = 0; // Default item highlighted on the list
  myMenu.high.i = 6; // Last item of the list is size of the list - 1.
  myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.
  myMenu.step.c_arr[0] = lcd_rows - 1; // rows to auto fit entire screen
  myMenu.step.c_arr[1] = 1; // one col list
  myMenu.step.c_arr[2] = 0; // y for additional feature such as an index
  myMenu.step.c_arr[3] = lcd_columns - 4; // x for additional feature such as an index
  myMenu.col = 0; // Display prompt at column 0
  myMenu.row = 1; // Display prompt at row 1
  myMenu.option = global_style;

  while (1)
  {
    lcd.clear();  // Refresh menu if a button has been pushed
    center_text("Nastaveni vetraku");

    myMenu.option = global_style; // This makes sure the change of menu style is updated if there is a menu style option in this level of menu.
    myMenu.width = lcd_columns - ((global_style & phi_prompt_arrow_dot) != 0) - ((global_style & phi_prompt_scroll_bar) != 0); // Auto fit the size of the list to the screen. Length in characters of the longest list item.


    select_list(&myMenu);
    menu_pointer_2 = myMenu.low.i;
    switch (menu_pointer_2)
    {
      case 0:
        return;
        break;

      case 1:
        set_FanOn();
        return;
        break;

      case 2:
        set_FanOff();
        return;
        break;

      case 3:
        set_AutoFanOn();
        return;
        break;
      case 4:
        set_AutoFanOff();
        return;
        break;

      case 5:
        set_FanTemp();
        break;

      case 6:
        set_fanTime();
        break;

      default:
        break;
    }
  }
}


void sub_menu_LED_ON() {
  digitalWrite(FLED, HIGH);

  lcd.setCursor(0, 3);
  lcd.print("  LED trvale zapnuta");
  delay(1000);
}

void sub_menu_LED_OFF() {
  digitalWrite(FLED, LOW);
  SetFlashOn = false;

  lcd.setCursor(0, 3);
  lcd.print("  LED vypnuta         ");
  delay(1000);
}

void set_beep(bool choice){
  lcd.setCursor(0, 3);
  if(choice) {
    lcd.print("Pipani trvale zapnuto");
    mute=false;
    if (EEPROM_readInt(60) != 0) EEPROM_writeInt(60, 0); //update eeprom
  }
  else {
    lcd.print("Pipani trvale vypnuto");
    mute=true;
    if (EEPROM_readInt(60) != 1) EEPROM_writeInt(60, 1); //update eeprom
  }
  delay(1000);
}

void beep_test(){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Testuji pipani...");
  digitalWrite(pinSpeaker, HIGH);
  delay(2000);
  digitalWrite(pinSpeaker, LOW);
  delay(2000);
  digitalWrite(pinSpeaker, HIGH);
  delay(2000);
  digitalWrite(pinSpeaker, LOW);
}




void sub_menu_BacklightTimer()
{
  lcd.clear();
  lcd.print("Cas Podsviceni (s)");

  int user_input = backLightTime;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer = &user_input;
  myIntegerInput.low.i = 0;
  myIntegerInput.high.i = 300;
  myIntegerInput.step.i = 1;
  myIntegerInput.col = 10;
  myIntegerInput.row = 2;
  myIntegerInput.width = 3;
  myIntegerInput.option = 1;
  input_integer(&myIntegerInput);
  backLightTime = user_input;
  if (EEPROM_readInt(0) != user_input) EEPROM_writeInt(0, user_input); //update eeprom
}


void sub_menu_setOpenTime()
{
  lcd.clear();
  lcd.print("Cas otevirani (s)");

  int user_input = MotorFailSafeOpen;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer = &user_input;
  myIntegerInput.low.i = 1;
  myIntegerInput.high.i = 30;
  myIntegerInput.step.i = 1;
  myIntegerInput.col = 10;
  myIntegerInput.row = 2;
  myIntegerInput.width = 3;
  myIntegerInput.option = 1;
  input_integer(&myIntegerInput);
  MotorFailSafeOpen = user_input;
  if (EEPROM_readInt(4) != user_input) EEPROM_writeInt(4, user_input); //update eeprom
}

void sub_menu_setCloseTime()
{
  lcd.clear();
  lcd.print("Cas zavirani (s)");

  int user_input = MotorFailSafeClose;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer = &user_input;
  myIntegerInput.low.i = 1;
  myIntegerInput.high.i = 30;
  myIntegerInput.step.i = 1;
  myIntegerInput.col = 10;
  myIntegerInput.row = 2;
  myIntegerInput.width = 3;
  myIntegerInput.option = 1;
  input_integer(&myIntegerInput);
  MotorFailSafeClose = user_input;
  if (EEPROM_readInt(8) != user_input) EEPROM_writeInt(8, user_input); //update eeprom
}


void sub_menu_setTimeZone()
{
  lcd.clear();
  lcd.print("Casove pasmo od GTM (h)");
  lcd.setCursor(8, 2);
  if(TimeZone<0) lcd.print("-");
  else lcd.print("0");

  if(TimeZone/10>0) lcd.print(TimeZone);
  else {
    lcd.print("0");
    lcd.print(TimeZone);
  }

  int user_input = TimeZone;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer = &user_input;
  myIntegerInput.low.i = -12;
  myIntegerInput.high.i = 14;
  myIntegerInput.step.i = 1;
  myIntegerInput.col = 8;
  myIntegerInput.row = 2;
  myIntegerInput.width = 3;
  myIntegerInput.option = 1;
  input_integer(&myIntegerInput);
  TimeZone = user_input;
  if (EEPROM_readInt(40) != user_input) EEPROM_writeInt(40, user_input); //update eeprom
  mySunrise.UpdateLocation(Latitude,Longitude,TimeZone);
}


void sub_menu_setLatitude()
{
  lcd.clear();
  lcd.print("Sirka: +-XX.YY");
  lcd.setCursor(8, 1);

  if(Lat_pre<0) lcd.print("-");
  else lcd.print("0");
  if(abs(Lat_pre)/10>=1) lcd.print(abs(Lat_pre));
  else {
    lcd.print("0");
    lcd.print(abs(Lat_pre));
  }
  lcd.print(".");
  if(Lat_suf/10>=1) lcd.print(Lat_suf);
  else {
    lcd.print("0");
    lcd.print(Lat_suf);
  }

  int user_input = Lat_pre;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer = &user_input;
  myIntegerInput.low.i = -90;
  myIntegerInput.high.i = 90;
  myIntegerInput.step.i = 1;
  myIntegerInput.col = 8;
  myIntegerInput.row = 1;
  myIntegerInput.width = 3;
  myIntegerInput.option = 1;
  input_integer(&myIntegerInput);
  Lat_pre = user_input;
  if (EEPROM_readInt(44) != user_input) EEPROM_writeInt(44, user_input); //update eeprom

  //next part
  user_input = Lat_suf;

  myIntegerInput.low.i = 0;
  myIntegerInput.high.i = 99;
  myIntegerInput.width = 2;
  myIntegerInput.col = 12;
  input_integer(&myIntegerInput);
  Lat_suf = user_input;
  if (EEPROM_readInt(48) != user_input) EEPROM_writeInt(48, user_input); //update eeprom

  Latitude=Lat_pre+Lat_suf/100;

  mySunrise.UpdateLocation(Latitude,Longitude,TimeZone);

  wait_on_escape(2000);

}


void sub_menu_setLongitude()
{
  lcd.clear();
  lcd.print("Delka: +-XXX.YY");
  lcd.setCursor(8, 1);

  if(Lon_pre<0) lcd.print("-");
  else lcd.print("0");

  if(Lon_pre/100>=10) lcd.print(abs(Lon_pre));
  else {
    if(Lon_pre/10>=1) {
      lcd.print("0");
      lcd.print(abs(Lon_pre));
    }
    else {
      lcd.print("00");
      lcd.print(abs(Lon_pre));
    }
  }
  lcd.setCursor(12, 1);
  lcd.print(".");
  if(Lon_suf/10>=1) lcd.print(Lon_suf);
  else {
    lcd.print("0");
    lcd.print(Lon_suf);
  }

  int user_input = Lon_pre;
  phi_prompt_struct myIntegerInput;
  myIntegerInput.ptr.i_buffer = &user_input;
  myIntegerInput.low.i = -180;
  myIntegerInput.high.i = 180;
  myIntegerInput.step.i = 1;
  myIntegerInput.col = 8;
  myIntegerInput.row = 1;
  myIntegerInput.width = 4;
  myIntegerInput.option = 1;
  input_integer(&myIntegerInput);
  Lon_pre = user_input;
  if (EEPROM_readInt(52) != user_input) EEPROM_writeInt(52, user_input); //update eeprom

  //next part
  user_input = Lon_suf;

  myIntegerInput.low.i = 0;
  myIntegerInput.high.i = 99;
  myIntegerInput.width = 2;
  myIntegerInput.col = 13;
  input_integer(&myIntegerInput);
  Lon_suf = user_input;
  if (EEPROM_readInt(56) != user_input) EEPROM_writeInt(56, user_input); //update eeprom

  Longitude=Lon_pre+Lon_suf/100;

  mySunrise.UpdateLocation(Latitude,Longitude,TimeZone);

  wait_on_escape(2000);

}




