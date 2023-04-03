//----- MOTOR ESQUERDA -----
#define INPUT1 2
#define INPUT2 4
#define ENABLE1 3

//----- MOTOR DIREITA -----
#define INPUT3 7
#define INPUT4 8
#define ENABLE2 9

//----- SENSORES -----
#define SENSOR_DIREITA 12
#define SENSOR_CENTRO 11
#define SENSOR_ESQUERDA 10

//----- CONSTANTES -----
#define K 255
#define VELOCIDADE_BASE 255
#define DT 300

//----- AUXILIARES -----
int erro = 0; //Erro de posicao do robo
unsigned long t0 = 0; //Medicao de tempo com millis()
String sensores = ""; //Leitura de todos os sensores

void setup() {
  //Inicializando comunicacao serial
  Serial.begin(9600);

  //Motor esquerda
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(ENABLE1, OUTPUT);

  //Motor direita
  pinMode(INPUT3, OUTPUT);
  pinMode(INPUT4, OUTPUT);
  pinMode(ENABLE2, OUTPUT);

  //Sensores
  pinMode(SENSOR_DIREITA, INPUT);
  pinMode(SENSOR_CENTRO, INPUT);
  pinMode(SENSOR_ESQUERDA, INPUT);
}

void loop() {
  lerSensores();  
  
  if (sensores == "000") {
    mover(255, 255); //Movimento para frente

    t0 = millis();
    while (millis() - t0 < DT) {
      lerSensores();
      if (sensores != "000") goto jump;
    }

    mover(-255, 255); //Movimento para esquerda

    t0 = millis();
    while (millis() - t0 < DT) {
      lerSensores();
      if (sensores != "000") goto jump;
    }

    mover(255, -255); //Movimento para direita
      
    t0 = millis();
    while (millis() - t0 < 2 * DT) {
      lerSensores();
      if (sensores != "000") goto jump;
    }

    mover(-255, 255); //Movimento para esquerda

    t0 = millis();
    while (millis() - t0 < DT + 30) {
      lerSensores();
      if (sensores != "000") goto jump;
    }
    	
	  jump:;
  } else if (sensores == "111") {
    mover(VELOCIDADE_BASE, VELOCIDADE_BASE); //Movimento para frente
  } else {
    calculaErro();
    mover(VELOCIDADE_BASE + K * erro, VELOCIDADE_BASE - K * erro);
  }
}

/*
  Calcula o erro de posicionamento do 
  robo em relacao a linha preta
*/
void calculaErro() {
  if (sensores == "010") erro = 0;
  else if (sensores == "110") erro = -1;
  else if (sensores == "100") erro = -2;
  else if (sensores == "011") erro = 1;
  else if (sensores == "001") erro = 2;
}

/*
  Fucao para mover o robo
  mover(int esquerda, int direita)
    esquerda -> velocidade do motor da esquerda [0, 255] 
	           ((+) para frente, e (-) para tras);
    direita -> velocidade do motor da direta [0, 255] 
	           ((+) para frente, e (-) para tras);
*/
void mover(int esquerda, int direita) {

  //Saturando as velocidades
  if (esquerda > 255) esquerda = 255;
  if (esquerda < -255) esquerda = -255;

  if (direita > 255) direita = 255;
  if (direita < -255) direita = -255;

  //Controlando sentido dos motores
  if (esquerda >= 0) {
    digitalWrite(INPUT1, HIGH);
    digitalWrite(INPUT2, LOW);
    analogWrite(ENABLE1, esquerda);  
  } else {
    digitalWrite(INPUT1, LOW);
    digitalWrite(INPUT2, HIGH);
    analogWrite(ENABLE1, -esquerda);
  }

  if (direita >= 0) {
    digitalWrite(INPUT3, HIGH);
    digitalWrite(INPUT4, LOW);
    analogWrite(ENABLE2, direita);  
  } else {
    digitalWrite(INPUT3, LOW);
    digitalWrite(INPUT4, HIGH);
    analogWrite(ENABLE2, -direita);
  }
}

/*
  Fucao para efetuar a leitura dos sensores
  Retorna uma string contentdo a leitura dos sensores
  1 -> preto
  0 -> branco
  
  Ideias: Utilizar bits para melhorar a representacao
*/
void lerSensores() {
  int sensor_direita = digitalRead(SENSOR_DIREITA);
  int sensor_centro = digitalRead(SENSOR_CENTRO);
  int sensor_esquerda = digitalRead(SENSOR_ESQUERDA);

  sensores = String(sensor_esquerda) + String(sensor_centro) + String(sensor_direita);
}