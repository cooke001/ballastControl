#include "MS5837.h"
#include <Wire.h>
MS5837 sensor;
int i=0;
int up = 5;
int down = 6;
int a = 0;
int b = 0;
int c = 0;
int d = 10;
int del = 1;
float depth = 0;
float sdist = 0;
int error = 1;
float derror = 0;
int PWMA = 3;
int PWMB = 11;
int DIRA = 12;
int DIRB = 13;
int endStop = 7;
int dist = 0;
int target = 34;

void setup() {
  Serial.begin(9600);
  
  Serial.println("Starting");
  
  Wire.begin();

  // Initialize pressure sensor
  // Returns true if initialization was successful
  // We can't continue with the rest of the program unless we can initialize the sensor
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  
  pinMode(endStop, INPUT);
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);
}

void loop() {
  sensor.read();
  depth = sensor.depth();
  derror = 1-(depth + 0.4); // if derror negative, too deep. 
  sdist = 2+float(dist - 800)/800;
  Serial.print(-0.30);
  Serial.print(",");
  Serial.print(sdist);
  Serial.print(",");
  Serial.print(derror);
  Serial.print(",");
  Serial.println(0.30);
  i = 0;
  while(i<200){
  if(abs(derror)<0.3){
    a = 0;
    b = 0;
  }
  else if(derror<0){
    a = 0;
    b = 1;
  }
  else{
    b = 0;
    a = 1;
  }
  //a = digitalRead(down); // replace this line with the up command
  //b = digitalRead(up);   // replace this line with the down command
  c = !digitalRead(endStop);
  
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
      c = !digitalRead(endStop);
    }
    error = 3;
  }

  if(error == 2){ //negative distance
    while(c){  //move in until endstop pressed
      in();
      c = !digitalRead(endStop);
    }
    error = 3; //endstop now pressed, hence error case 3 error
  }
  
  if(error == 3){ //met end stop 
    while(!c){    //move out until enstop depressed
      out();      
      c = !digitalRead(endStop);
    }
    dist = 0; //reset distance
    error = 0; //clear error
  }
  
  if (a) {
    out();
    dist = dist + 1;  
  }
  else if (b) {
    in();
    dist = dist - 1;
  }
  else{
    delay(4*del);
    i++;
  }
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
  i++;
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
  i++;
}
