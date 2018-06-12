//pro attiny85

//5,7V na zdroji je dolni limit detekce
//graf spojnice trendu: y=0,3148x-0,0274


/* Spotreba: (mereno na verzi 1.8, 21/01/2017) (bez vetraku, na lab. zdroji)
 *  (6.77V)
 * vse v pohotovosti 114 mA
 * lcd vypnuto 91 mA
 *  (6.27V)
 * nouzovy rezim 14 mA, pri probrani attiny 23 mA
 * bazalni spotreba 5V stabilizatoru 10 mA
 * 
 * Baterka: 6V 4.5Ah
 * 91mA pri 6.5v -> 0.5915 W
 * 4.5Ah/0.091A=49.5h -> 2 dny a kousek
 * pri 60Ah 12V baterce tedy cca: 55 dni
 */



//#define INTERNAL2V56_NO_CAP (6)


#define RelayOn HIGH
#define RelayOff LOW

#define pin_Relay 0 //0
#define pin_LED 1 //1
#define pin_ArdRst 2
#define pin_MotIn 4
#define pin_BatteryIn A3


#include <avr/sleep.h>
#include <avr/wdt.h>
 
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


const float Vref = 4.99;  //4.99;            // Actual voltage and resistor values can be set here. 
const float R1 =  21900.0;            // R1 is nominally 22k
const float R2 =  10000.0;            // R2 is nominally 10k

const float resDiv = (R2/(R1 + R2));  // Resistor divider factor applied to measured voltage.
// resDiv = 0,31347

const float voltageLowSet = 6.35;             // Set low voltage threshold (engine off).
const float voltageHighSet = 6.7;            // Set high voltage threshold (engine running).

bool underVoltage = false, normal = true, wait_lowvoltage = false;
unsigned long motor_time = 0, wait_lowvoltage_time;
int motor_failsafe = 15;  //in seconds
int tim = 500;                         // Delay between low voltage checks
 

void setup() {
  pinMode(pin_Relay, OUTPUT);
  digitalWrite(pin_Relay, RelayOff);   
  pinMode(pin_LED, OUTPUT); 
  digitalWrite(pin_LED, LOW); 
  pinMode(pin_ArdRst, OUTPUT);
  digitalWrite(pin_ArdRst, HIGH);
  pinMode(pin_MotIn, INPUT); 
  pinMode(pin_BatteryIn, INPUT);

 setup_watchdog(7);                    // approximately 2 second sleep
 
  //blink on startup 
  for (int i=0; i<5; i++) {
    digitalWrite(pin_LED, HIGH);          
    delay(80);
    digitalWrite(pin_LED, LOW); 
    delay(80); 
  }  

 // This is the analogReference value for the ATTiny 2.56v internal
// reference, with no external cap bypass.
//  analogReference( INTERNAL2V56_NO_CAP );
 
 analogReference(DEFAULT);
 delay(500); //nachystani ADC 
}

void loop() {
    float volts=getVolts();

    if (volts < (voltageLowSet-0.1)){ //very low battery
      wait_lowvoltage=true;
      
      normal=false;
      underVoltage=true;
      system_sleep();
    }


    if(volts<(voltageLowSet-0.05)){
      wait_lowvoltage=true;
      
       underVoltage=true;
    }
    
    if(volts>=voltageHighSet){ //nabijecka pripojena
       underVoltage=false;
       normal=true;
       wait_lowvoltage=false;
    }  


    if((voltageLowSet<volts)&&(volts<voltageHighSet)){
       //v normalnim rozsahu na baterku
       wait_lowvoltage=false;
       if(underVoltage) { //jednou jsem tam, pozor. cekam na nabijecku
            //another measurement to ensure low voltage
            delay(100);
            if(volts<(getVolts()-0.05)){
              // delay(tim);
               normal=false;
              // system_sleep();
            }
       }
       else normal=true;   
    }


  
    if(underVoltage){ //safe mode
      if(wait_lowvoltage){
        wait_lowvoltage_time=millis()/1000; 
        if((millis()/1000-wait_lowvoltage_time)>=5){ //po 5s udelat nove mereni
                //reset arduino

              }    


        
      }
         digitalWrite(pin_Relay, RelayOff);
         for (int i=0; i<2; i++) {
            digitalWrite(pin_LED, HIGH);          
            delay(30);
            digitalWrite(pin_LED, LOW); 
            delay(30); 
          }    
      //usporny rezim a pauza do dalsiho mereni 60s

      system_sleep();
     // delay(tim);
    }
    else{ //normal mode    
      if(normal) digitalWrite(pin_Relay, RelayOn);
      
       if(digitalRead(pin_MotIn)){ //motor is running
          motor_time=millis()/1000; //in seconds
          do{
              if(digitalRead(pin_MotIn)==0) break;
              if((millis()/1000-motor_time)>=motor_failsafe){ //problem
                //reset arduino
                digitalWrite(pin_ArdRst, LOW);          
                delay(50);
                digitalWrite(pin_ArdRst, HIGH); 
                break;
              }
          }while(digitalRead(pin_MotIn)); 
       }
       wait_lowvoltage=false; 
     }
   
  delay(300);
}


float getVolts() {                    // Function returns the actual battery volts.
  int raw=0;
  raw = analogRead(pin_BatteryIn);
  float volts = (raw / 1023.0) * Vref;
  volts = (volts / resDiv)+0.08; //+ konstanta ziskana merenim
  return volts;
}  




// Put system into the sleep state
void system_sleep() {
  cbi(ADCSRA,ADEN);                    // switch ADC OFF to save power
 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
 
  sleep_disable();                     // System continues execution here when watchdog times out
  sbi(ADCSRA,ADEN);                    // switch ADC back ON
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {          // set the required register bits to
                                       // set up the watcdog timer.
  byte bb;
  if (ii > 9 ) ii=9;
  bb = ii & 7;
  if (ii > 7) bb |= (1<<5);
  bb |= (1<<WDCE);
 
  MCUSR &= ~(1<<WDRF);
  WDTCR |= (1<<WDCE) | (1<<WDE);
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}
 
// Watchdog Interrupt Service / is executed when watchdog times out
ISR(WDT_vect) {
 // Include this routine even if it doesn't do anything.
}



