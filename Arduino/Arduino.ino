#include <MS5837.h>
#include <Wire.h>
MS5837 sensor;
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
  pinMode(endStop, INPUT);
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
  Wire.begin();        // join i2c bus (address optional for master)
//  while (!sensor.init()) {
//    Serial.println("Init failed!");
//    Serial.println("Are SDA/SCL connected correctly?");
//    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
//    Serial.println("\n\n\n");
//    delay(5000);
//  }
  
  //sensor.setModel(MS5837::MS5837_02BA);
  //sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
}

ISR(TIMER1_COMPA_vect){
  //sensor.read();
  //d = sensor.depth();
  //Wire.beginTransmission(4); // transmit to device #4
  //Wire.write(d);
  //Wire.write(",");
  //Wire.write(dist);
  //Wire.endTransmission();
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
