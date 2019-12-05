int up = 5;
int down = 6;
int a = 0;
int b = 0;
int c = 1;
int PWMA = 3;
int PWMB = 11;
int DIRA = 12;
int DIRB = 13;
void setup() {
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);
   Serial.begin(9600);
}

void loop() {
  
  a = digitalRead(down);
  b = digitalRead(up);
  if (a) {
      digitalWrite(PWMA, HIGH);
      digitalWrite(PWMB, HIGH);
     
      digitalWrite(DIRA, HIGH);      
      digitalWrite(DIRB, HIGH);
      delay(c);
      
      digitalWrite(DIRA, LOW);      
      digitalWrite(DIRB, HIGH);
      delay(c);
      
      digitalWrite(DIRA, LOW);      
      digitalWrite(DIRB, LOW);
      delay(c);
      
      digitalWrite(DIRA, HIGH);      
      digitalWrite(DIRB, LOW);
      delay(c);
      
      digitalWrite(PWMA, LOW);
      digitalWrite(PWMB, LOW);
      }
if (b) {
      digitalWrite(PWMA, HIGH);
      digitalWrite(PWMB, HIGH);
     
      digitalWrite(DIRA, HIGH);      
      digitalWrite(DIRB, HIGH);
      delay(c);
      
      digitalWrite(DIRA, HIGH);      
      digitalWrite(DIRB, LOW);
      delay(c);
      
      digitalWrite(DIRA, LOW);      
      digitalWrite(DIRB, LOW);
      delay(c);
      
      digitalWrite(DIRA, LOW);      
      digitalWrite(DIRB, HIGH);
      delay(c);
     
      digitalWrite(PWMA, LOW);
      digitalWrite(PWMB, LOW);
      }
}

