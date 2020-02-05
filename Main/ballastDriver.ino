int up = 5;
int down = 6;
int a = 0;
int b = 0;
int c = 0;
int del = 1;
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
  //Serial.begin(9600);
}

void loop() {
  
  a = digitalRead(down);
  b = digitalRead(up);
  c = digitalRead(endStop);
  //Serial.println(dist);
  if(dist > 1600){
    //error = 1;
    a = 0;
    //b = 0;
  }
  if(dist < 0){
    error = 2;
    a = 0;
    b = 0;
   
  }
  if(!c){
    error = 3;
    a = 0;
    b = 0;
  
 
  }

  if(error == 2 || error == 1){ //negative distance
   
    while(c){
      in();
      c = digitalRead(endStop);
    }
    error = 3;
  }
  if(error == 3){ //met end stop
    
    while(!c){    
      out();      
      c = digitalRead(endStop);
    }
    
    dist = 0;
    error = 0;
   
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
