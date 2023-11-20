// Definição das conexões dos pinos
#define ENA 10
#define IN1 7
#define IN2 8
#define ENC1 2
#define ENC2 3

int pwm = 0;
int pos = 0;
int idealPos = 0;
unsigned long t0 = millis();

void setup() {
  //Definição dos pinos como saídas
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENC1, INPUT);
  pinMode(ENC2, INPUT);
  
  // Motores começam desligados
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(ENC1), readEncoder, RISING);
  delay(5000);
  t0 = millis();
}

void loop() {
  if(millis() - t0 > 10000){
    idealPos = idealPos + 8;
    t0 = millis();
    if(idealPos >= 100){
      voltarAoInicio(); 
    }
  }
  control();
//  
  Serial.println(pos);
}

void readEncoder(){
  int b = digitalRead(ENC2);
  if (b >0){
    pos ++;
  }
  else{
    pos--;
  }
}
void control(){
  if(pos < idealPos){
    pwm = 55;
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if(pos > idealPos){
    pwm = 55;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else{
  pwm = 0;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  }
  analogWrite(ENA, pwm);
}

void voltarAoInicio(){
  idealPos = 0;
  while(pos!=0){
    control();
  }
  //RESET
  delay(60000);
}
