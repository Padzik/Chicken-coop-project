//----------------------------------------------
// Read Buttons from encoder function
//----------------------------------------------
byte phi_keypads::scanKeypad()
{
  if(butto::button_pressed!=0){
    int reply;
    reply= butto::button_pressed;
    butto::button_pressed=0; 
    return reply;
  }
  else return NO_KEYs; //NO_KEYs je spravne
}

void buttonClick(){butto::button_pressed=BUTTON_SELECT;}
void encoderClick(){
  // encoder must have turned one click because interrupt 0 was triggered
  // read value from both encoder pins
  int valA = digitalRead(Encoder1ChnA);
  int valB = digitalRead(Encoder1ChnB);
  
  // compare pins to determine in which direction encoder was turned
  if (valA != valB){
      // pinA just changed but pinB had not yet changed
      // Direction must be clockwise if A changes before B
      butto::button_pressed=BUTTON_UP;
  }
  else{
      // pinA just changed and pinB had already done so.
      // Direction must be counter-clockwise if B changes before A
      butto::button_pressed=BUTTON_DOWN;
  }
}
