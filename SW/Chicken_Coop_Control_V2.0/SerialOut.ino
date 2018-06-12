//using SendOnlySoftwareSerial.h


void sendData(){



   // digitalWrite(pinSpeaker, HIGH);
   // delay(200);
   // digitalWrite(pinSpeaker, LOW);

/*
   digitalWrite(pinSpeaker, HIGH);
   delay(200);
   digitalWrite(pinSpeaker, LOW);
*/


  
  mySerial.println (GlobTime);
//  mySerial.print (" ");
  mySerial.println (ActualTemp); //10x vic
  
  mySerial.println (WakeTime);
//  mySerial.print (" ");
  mySerial.println (BedTime);
 // mySerial.print (" ");

  mySerial.println (!button_check(pinLiT)); 
 // mySerial.print (" ");
  mySerial.println (!button_check(pinLiB));
 // mySerial.println ("");
}


/*
void sendData(){
  //RTC.get(rtc,true);




    digitalWrite(pinSpeaker, HIGH);
    delay(200);
    digitalWrite(pinSpeaker, LOW);


  mySerial.print ("Cas: ");
  mySerial.print (rtc[2]);
  mySerial.print (":");
  mySerial.print (rtc[1]);
  mySerial.print (":");
  mySerial.println (rtc[0]);



  mySerial.print ("GlobTime: ");
  mySerial.print (GlobTime);
  mySerial.print (" ");
  mySerial.println (OldGlobTime);

  mySerial.print ("GlobTimeSeconds: ");
  mySerial.print (GlobTimeSeconds);
  mySerial.print (" ");
  mySerial.println (OldGlobTimeSeconds);

  mySerial.print ("NewDayTimeCorection: ");
  mySerial.print (NewDayTimeCorection);
  mySerial.print (" TimeCorection ");
  mySerial.print (TimeCorection);
  mySerial.print (" LastDayCorection ");
  mySerial.println (LastDayCorection);
  
  mySerial.println (" ");
}
*/
