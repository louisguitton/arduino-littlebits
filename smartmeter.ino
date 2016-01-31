// this sketch uses the Time library http://www.pjrc.com/teensy/td_libs_Time.html
#include <Time.h>

// Light sensor input on A1
int D9 = 9; // CloudBit analog output

time_t t = now(); 
int numFlash = 0; 
boolean packetSent = false;
boolean flashTakenIntoAccount = false;
int per = 45;//periode of the output in seconds < 60
int intensityThreshold = 800;//the analog value of the input above which the input signal is considered to be HIGH

  
void setup() {
  pinMode(A1,INPUT);
  pinMode(D9,OUTPUT);
  Serial.begin(9600);
}

/* 2 ways of coding the sketch: 
    1) every n flashes of the LED (n is an integer), the CloudBit sends a digital output
    2) every t seconds (t is a time), the CloudBit sends an analog signal that is equal to the number of LED flashs within t
  The bottleneck of the circuit is the CloudBit frequence through the cloud. So we should set the output frequency and let free the 'size' of the message (analog rather than digital)
  2) is hence chosen
*/
void loop() {
  printState(); 
  if (second(now()-t) >= per) {
    t = now();
    if ( numFlash > 0 && packetSent == false ){
      sendPacket();
      Serial.println();
    } 
    //digitalWrite(D9,LOW);
  }
  else if ( second(now()-t) < per ){
    incrementFlash();
  }
}

void sendPacket(){
    analogWrite(D9,ceil(numFlash*10/4));
    //digitalWrite(D9,HIGH);
    delay(500);
    numFlash = 0;//reset flash counter
    packetSent = true;
}

void incrementFlash(){
//  numFlash += 1; 
  if ( analogRead(A1) > intensityThreshold && flashTakenIntoAccount == false ){
    numFlash += 1;
    packetSent = false;
    flashTakenIntoAccount = true;
  }
  else if( analogRead(A1) <= intensityThreshold){
    flashTakenIntoAccount = false;
  }  
}
void printState(){
  Serial.print("t: ");
  Serial.print(minute(t));
  Serial.print(":");
  Serial.print(second(t));
  Serial.print("\t");
  Serial.print("now: ");
  Serial.print(minute(now()));
  Serial.print(":");
  Serial.print(second(now()));
  Serial.print("\t intoAccount: ");
  Serial.print(flashTakenIntoAccount); 
  Serial.print("\t flashs: ");
  Serial.print(numFlash);
  Serial.print(" aRead: ");
  Serial.print(analogRead(A1));
  Serial.print("\t \t sent?: ");
  Serial.println(packetSent);  
}
