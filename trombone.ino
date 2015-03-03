#include <toneAC.h>

int D0 = 0;
int D1 = 1;
int D5 = 5;
int D9 = 9;

int onAir = LOW;
int takenIntoAccount = false;
double freqRecorded=0;

void setup() {
  pinMode(D0,INPUT);
  pinMode(A0,INPUT);
  pinMode(D1,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D9,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // onAir tells you when you can play and triggers a LED
  if(digitalRead(D0)==HIGH & takenIntoAccount==false){
    onAir = !onAir;
    takenIntoAccount = true;
  }
  else if(digitalRead(D0)==LOW){
    takenIntoAccount = false;
  } 
  digitalWrite(D1, onAir);
  
  // what to do when you're on air
  int xSlider = analogRead(A0);
  int xMidi = 48 + xSlider*(60-48)/765; // C3=48; C4=60; C5=72; C6=84; the slider goes from 1 to 765
  freqRecorded = 440*pow(pow(2,0.0833),xMidi-69); // fromula of MIDI encoding
 
  if(digitalRead(A1)==HIGH) {
    if(onAir == HIGH){
      //analogWrite(D5,xSlider);
      toneAC(freqRecorded);
      Serial.println(freqRecorded);
    }
    delay(50); // mms; changes the writing speed and the legato
  }
  
  if(digitalRead(A1)==LOW){
    noToneAC();
  }
}
