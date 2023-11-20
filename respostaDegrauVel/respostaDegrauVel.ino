#include <util/atomic.h>
// Definição das conexões dos pinos
#define ENA 10
#define IN1 7
#define IN2 8
#define ENC1 2
#define ENC2 3

int pwm = 70; // 150 tá baum
int pos = 0;
int idealPos = 300;
unsigned long t0 = millis();

long prevT= 0;
int posPrev = 0;
volatile int pos_i = 0;

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
  
//  if (Serial.available()>0){
//  
//    idealPos = Serial.readString().toInt();
//  }
//  Serial.println("idealPos: ");
//  Serial.println(idealPos);
//  //Serial.println(",");
//  Serial.print("pos: ");
//  Serial.println(pos);
  
  if ((millis() - t0 >= 5000) && (pwm < 255)){
    pwm+=10;
    t0 = millis();
  }
  control();
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos = pos_i;
  }
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/1.0e6;
  float vel1 = (pos - posPrev)/deltaT;
  posPrev = pos;
  prevT = currT;

  Serial.println("Ideal: ");
  Serial.println(returnRPM(pwm));
  //Serial.println(",");
  Serial.print("Real: ");
  Serial.println(vel1);

}

int returnRPM(int PWM){
  return int(-0.298*pow(PWM,2) + 142*PWM  - 7880);
}

void readEncoder(){
  int b = digitalRead(ENC2);
  int increment = 0;
  if (b >0){
    increment = 1;
  }
  else{
    increment = -1;
  }
  pos_i = pos_i + increment;
}
void control(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
//  Serial.print("PWM: ");
//  Serial.println(pwm);
//  if(pos < idealPos){
//    //pwm = 255 * abs(pos - idealPos)/idealPos;
//    digitalWrite(IN1, HIGH);
//    digitalWrite(IN2, LOW);
//  }
//  else if(pos > idealPos){
//    //pwm = 55;
//    //pwm = 255 * abs(pos - idealPos)/idealPos;
//    digitalWrite(IN1, LOW);
//    digitalWrite(IN2, HIGH);
//  }
//  else{
//  //pwm = 0;
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, LOW);
//  }
//  analogWrite(ENA, pwm);
}

void voltarAoInicio(){
  idealPos = 0;
  while(pos!=0){
    control();
  }
  //RESET
  delay(60000);
}
