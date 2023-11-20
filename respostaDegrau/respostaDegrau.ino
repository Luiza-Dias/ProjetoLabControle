// Definição das conexões dos pinos
#define ENA 10
#define IN1 7
#define IN2 8
#define ENC1 2
#define ENC2 3

int pwm = 0;
int pos = 0;
int idealPos = 300;
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
  delay(1000);
  t0 = millis();
}

void loop() {
  
  if (Serial.available()>0){
  
    idealPos = Serial.readString().toInt();
  }
  Serial.println("idealPos: ");
  Serial.println(idealPos);
  //Serial.println(",");
  Serial.print("pos: ");
  Serial.println(pos);
  control();
//  
//  Serial.println(pos);
}

void readEncoder(){
  int b = digitalRead(ENC2);
  if (b >0){
    pos ++;
  }
  else{
    pos--;
  }
  pos = pos%400;
}
void control(){
  pwm = 140 * (pos - idealPos)/400;
  pwm = abs(pwm) + 50;
//  Serial.print("PWM: ");
//  Serial.println(pwm);
  if(pos < idealPos){
    //pwm = 255 * abs(pos - idealPos)/idealPos;
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if(pos > idealPos){
    //pwm = 55;
    //pwm = 255 * abs(pos - idealPos)/idealPos;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else{
  //pwm = 0;
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
