
//project based on code by Perry Childs
#define lcd_rows 4
#define lcd_columns 20

//relay fan changes
#define FANON LOW
#define FANOFF HIGH

// Encoder 1 returns U for up and D for down. phi_rotary_encoders my_encoder1(mapping1, Encoder1ChnA, Encoder1ChnB, Detent);
#define EncoderDetent 20
#define btn_u 8
#define btn_d 8
#define btn_l 8
#define btn_r 8
#define btn_b 8
#define btn_a 8
#define btn_null 0
#define total_buttons 6

// Arduino pin setting

#define Probe 12
#define pinFan 11
#define FLED 0 //z 4ky

#define pinIP1 4 //motor
#define pinIP2 5 //motor



#define Encoder1_SW 3
#define Encoder1ChnB 8
#define Encoder1ChnA 2 

#define LCD_RS 6
#define LCD_EN 7 
#define LCD_D4 A3
#define LCD_D5 A2
#define LCD_D6 A1
#define LCD_D7 A0

#define backLight 13 //5

//digital door input
//#define INVERTED_DOORS //odkomentovat pro nula=sepnuto
#define pinLiT 88 //door input //A6
#define pinLiB 9  //door input //D9

#define PIN_TX 1 
#define pinSpeaker 10

#include <Arduino.h>
#include <Wire.h>
#include <DS1307.h>
#include <OneWire.h>
#include <EEPROM.h>
#include <LCD.h>
#include <phi_interfaces.h>
#include <phi_prompt.h>
#include <Sunrise.h>
#include <SendOnlySoftwareSerial.h>




#define STOP 0
#define UP 1
#define DOWN -1
#define DoorDown 68
#define DoorUp 85
#define Overide 90
#define debounce_delay 150
#define NumKeys 5

#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_SELECT 5

namespace butto{ 
  volatile int button_pressed;  
}


// Create hardware objects


LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7); // Create the lcd object 

int rtc[7];
OneWire TempProbe(Probe);
float Latitude, Longitude;
int TimeZone, Lat_pre, Lat_suf, Lon_pre, Lon_suf;
//Sunrise mySunrise(49.18,16.53,1);//Brno, CzechRep, Europe - Latitude/Longitude and Timezone 49.18N,16.53E: 	49.18,16.53, 1=time zone
Sunrise mySunrise(0,0,0); //create global variable, update values later
SendOnlySoftwareSerial mySerial (PIN_TX);  // Tx pin

// Global variables
int GlobTime, OldGlobTime, WakeTime, BedTime, Flash, stateMotor, door_action, toggle=HIGH, BST_flag; //odebrano adc_key_in, key=-1, oldkey=-1,
long GlobTimeSeconds, OldGlobTimeSeconds;
int AddRiseTime, AddSetTime; //correction for computed suset/rise time
//char buf[6];
int MaxTReading, MaxTSignBit, MinTReading, MinTSignBit, TReading, SignBit;
int isLcdLightOn = false;
//int adc_key_val[5] ={ 30, 150, 360, 535, 760 }; //zmena
//int UpRate;//=450; //puv 300
//int DownRate;//=220; //puv 200
int backLightTime; //z puvodnich 300
//int MotorFailSafe = 6; //z puvodnich 3 // kolik sekund se bude vytahovat
int MotorOnTime;
int SetFlashOn = false;
int ControllerRunning = false;
int FanTemp, ActualTemp;
int FanTime; //minutes to spin fan and than same pause
bool FanRun = false, AutoFan=false, mute, speaker_running=false;
int FanLastSpin=0; //zmena z unsigned long
bool NewDayTimeCorection = true; //was time corrected today?
int TimeCorection;// = 40; //seconds to substract
int LastDayCorection = -1;

int MotorFailSafeOpen;// = 7;
int MotorFailSafeClose;// = 4;


//fan symbol PROGMEM const 
const uint8_t fan_symbol[8]  = {0x0,0x6,0x14,0x1b,0x5,0xc,0x0};
const uint8_t fan_auto_symbol[8]  = {0x1f,0x0,0x6,0x14,0x1b,0x5,0xc};



//char mapping1[]={
//  'U','D'}; // This is a rotary encoder so it returns U for up and D for down on the dial.
//phi_rotary_encoders my_encoder1(mapping1, Encoder1ChnA, Encoder1ChnB, EncoderDetent);
char mapping[]={
  1,2,3,4,5,6}; // This is a list of names for each button.
byte pins[]={
  0,0,0,0,0,0}; // predposledni na nulu
phi_button_groups my_btns(mapping, pins, 6);
multiple_button_input * keypads[]={  &my_btns,0};
//&my_btns,&my_encoder1,0};

char up_keys[]={
  'U',0}; ///< All keys that act as the up key are listed here.
char down_keys[]={
  'D',0}; ///< All keys that act as the down key are listed here.
char left_keys[]={
  'U',0}; ///< All keys that act as the left key are listed here.
char right_keys[]={
  'D',0}; ///< All keys that act as the right key are listed here.
char enter_keys[]={
  5,0}; ///< All keys that act as the enter key are listed here.
char escape_keys[]={
  5,0}; ///< All keys that act as the escape key are listed here.
char * function_keys[]={
  up_keys,down_keys,left_keys,right_keys,enter_keys,escape_keys}; ///< All function key names are gathered here fhr phi_prompt.



PROGMEM const char msg_00[]="Navod k pouziti: v menu navigace koleckem a potvrzeni kliknutim. Nastavovani ciselnych polozek: toceni po smeru hodinovych rucicek cyklicky meni hodnotu, protismeru ulozi a klik preskoci zadavani.     Zprovoznil Pavel na zaklade navodu na webu od Perry Childs. Rijen 2014";

void setup()
{
  //dat najevo, ze funguje
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);
//  delay(1000);

  
  lcd.begin(lcd_columns, lcd_rows);
  lcd.createChar(6, fan_symbol);
  lcd.createChar(7, fan_auto_symbol);
  init_phi_prompt(&lcd,keypads,function_keys, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
//  init_phi_prompt(&lcd,0,0, lcd_columns, lcd_rows, '~'); // Supply the liquid crystal object, input keypads, and function key names. Also supply the column and row of the lcd, and indicator as '>'. You can also use '\x7e', which is a right arrow.
  
  coop_initialise();
}

void loop()
{
  top_menu();
}






