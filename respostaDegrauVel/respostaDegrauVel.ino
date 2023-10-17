#include <util/atomic.h>
// Definição das conexões dos pinos
#define ENA 10
#define IN1 7
#define IN2 8
#define ENC1 2
#define ENC2 3
#define N 30

int pwm = 30; // PWM 55 - RPM 35
int pos = 0;
int idealPos = 300;
unsigned long t0 = millis();

long prevT= 0;
int posPrev = 0;
volatile int pos_i = 0;

//Filtro media movel
int vetorMedia[N];
// inicializa o vetor com NULL

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
  for(int i = 0; i < N; i++){
    vetorMedia[i]  = NULL;
  }
  attachInterrupt(digitalPinToInterrupt(ENC2), readEncoder, RISING);
  delay(1000);
  t0 = millis();
}

void loop() {
    
//  if ((millis() - t0 >= 5000) && (pwm < 255)){
//    pwm+=10;
//    t0 = millis();
//  }

  control();
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos = pos_i;
  }
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/1.0e6;
  float vel1 = (pos - posPrev)/deltaT;
  posPrev = pos;
  prevT = currT;
//
  Serial.println("Ideal: ");
  Serial.println(300);
  //Serial.println(",");
  Serial.print("Real: ");
  Serial.println(vel1);
//  Serial.print("Media: ");
//  Serial.println(media_movel(vel1));


}

int returnRPM(int PWM){
  return int(-0.298*pow(PWM,2) + 142*PWM  - 7880);
}

void readEncoder(){
  int b = digitalRead(ENC1);
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

}

int media_movel(int newValue){
  int soma = 0;
  int media;
  int contNULL = 0;
  for(int i = 0; i < N - 1; i++){
    vetorMedia[i] = vetorMedia[i+1];
    if(vetorMedia[i+1] != NULL){
      soma += vetorMedia[i+1];
    }
    else{
      contNULL++;
    }
  }
  vetorMedia[N-1] = newValue;
  soma += newValue;
  media = int(soma/(N - contNULL));
  return media;
}
