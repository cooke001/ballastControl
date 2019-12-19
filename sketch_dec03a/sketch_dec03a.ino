int up = 5;
int down = 6;
int a = 0;
int b = 0;
int c = 0;
int del = 2;
int error = 0;
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
   Serial.begin(9600);
}

void loop() {
  //Serial.println(dist);
  a = digitalRead(down);
  b = digitalRead(up);
  c = digitalRead(endStop);
  if(dist > 150){
    error = 1;
    a = 0;
    b = 0;
    Serial.print("Error ");
    Serial.print(error);
    Serial.println(": Too far out");
  }
  if(dist < 0){
    error = 2;
    a = 0;
    b = 0;
    Serial.print("Error ");
    Serial.print(error);
    Serial.println(": Negative reading");
  }
  if(!c){
    error = 3;
    a = 0;
    b = 0;
    Serial.print("Error ");
    Serial.print(error);
    Serial.println(": Endstop met");
  }

  if(error == 2 || error == 1){ //negative distance
    Serial.println("Moving back in....");
    while(c){
      in();
      c = digitalRead(endStop);
    }
    error = 3;
  }
  if(error == 3){ //met end stop
    Serial.println("Moving back out....");
    while(!c){    
      out();      
      c = digitalRead(endStop);
    }
    Serial.println("Endstop met");
    dist = 0;
    error = 0;
    Serial.println("Distance reset.");
  }
  
  if (a) {
    out();
    dist = dist + 1;
    Serial.println(dist);
  }
  if (b) {
    in();
    dist = dist - 1;
    Serial.println(dist);
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
