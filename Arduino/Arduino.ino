//#include <MS5837.h>
#include <Wire.h>
//MS5837 sensor;
int up = 5;
int down = 6;
int a = 0;
int b = 0;
int c = 0;
int d = 0;
int del = 1;
int error = 1;
int PWMA = 3;
int PWMB = 11;
int DIRA = 12;
int DIRB = 13;
int endStop = 7;
int dist = 0;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);
  
  pinMode(endStop, INPUT);
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);
  Serial.println("Pins Initialised");
  
}

void requestEvent() {
  uint8_t buffer[2];
  buffer[0] = dist >> 8;
  buffer[1] = dist & 0xff;  
  Wire.write(buffer, 2);
}

void loop() {
  a = digitalRead(down);
  b = digitalRead(up);
  c = digitalRead(endStop);
  
  if(dist == 1600){ //limit past 1600 movement or distance unknown
    a = 0;
  }
  
  if(dist > 1600){// distance unknown
    error = 1;
    a = 0;
  }
  
  if(dist < 0){ //negative distance without meeting endstop error
    error = 2;
    a = 0;
    b = 0;
  }
  
  if(!c){ //met end stop
    error = 3;
    a = 0;
    b = 0;
  }

  if(error == 1){ //endstop not pressed as case 3 error has not overwritten 
    while(c){
      in();
      c = digitalRead(endStop);
    }
    error = 3;
  }

  if(error == 2){ //negative distance
    while(c){  //move in until endstop pressed
      in();
      c = digitalRead(endStop);
    }
    error = 3; //endstop now pressed, hence error case 3 error
  }
  
  if(error == 3){ //met end stop 
    while(!c){    //move out until enstop depressed
      out();      
      c = digitalRead(endStop);
    }
    dist = 0; //reset distance
    error = 0; //clear error
  }
  
  if (a) {
    out();
    dist = dist + 1;  
  }
  
  if (b) {
    in();
    dist = dist - 1;
  }
  
}

void out(void){
  digitalWrite(PWMA, HIGH);
  digitalWrite(PWMB, HIGH);
   
  digitalWrite(DIRA, HIGH);      
  digitalWrite(DIRB, HIGH);
  delay(del);
  
  digitalWrite(DIRA, LOW);      
  digitalWrite(DIRB, HIGH);
  delay(del);
  
  digitalWrite(DIRA, LOW);      
  digitalWrite(DIRB, LOW);
  delay(del);
  
  digitalWrite(DIRA, HIGH);      
  digitalWrite(DIRB, LOW);
  delay(del);
  
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
}  

void in(void){
  digitalWrite(PWMA, HIGH);
  digitalWrite(PWMB, HIGH);
 
  digitalWrite(DIRA, HIGH);      
  digitalWrite(DIRB, HIGH);
  delay(del);
  
  digitalWrite(DIRA, HIGH);      
  digitalWrite(DIRB, LOW);
  delay(del);
  
  digitalWrite(DIRA, LOW);      
  digitalWrite(DIRB, LOW);
  delay(del);
  
  digitalWrite(DIRA, LOW);      
  digitalWrite(DIRB, HIGH);
  delay(del);
 
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
}
