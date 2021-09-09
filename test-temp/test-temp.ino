/**********************************************
 * Gabriel Correa - Ler Temperatura
 * 
 * Codigo para verificar a temperatura e a umidade, junto com o 
 * botao e o sensor de luminosidade 
 **********************************************/
// Incluindo a biblioteca do sentor DHT11 (Sensor de temperatura e umidade)
#include "DHT.h"

// Declarando variaveis dos pinos dos sensorer e do botao
const int pinoDHT = 12;
const int pinoBotao = 4;
const int pinoLDR = 15;

// Declarando as variaveis que receberão a leitura do botao, e do sensor de luz
int estadoBotao = 0;
int valorLDR = 0;

// Criação da instancia DHT, em funcao do pino do sensor e do tipo do DHT
DHT dht(pinoDHT, DHT11);


void setup() {
  // Iniciando o monitor serial
  Serial.begin(115200);

  // Iniciando o sensor DHT11
  dht.begin();

  // Definindo a atuação do Pino
  pinMode(pinoBotao, INPUT);

}

void loop() {

  // definindo a leitura do botao a variavel
  estadoBotao = digitalRead(pinoBotao);

  // definindo a condição, de que se o botao for pressionado realizar as seguintes informações
  if (estadoBotao == LOW) {
    delay(30);
    estadoBotao = digitalRead(pinoBotao);

    // Criando a variavel para armazenar a temperatura e a umidade
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    // Mostrando as informações de temperatura e umidade no monitor Serial
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" *C - Umidade: ");
    Serial.print(umidade);
    Serial.print(" % - Luminosidade: ");

    // Lendo os valores do pino LDR (luminosidade)
    valorLDR = analogRead(pinoLDR);
    valorLDR = map(valorLDR, 4095, 2500, 0, 100);

    // Mostrando as informações da luminosidade 
    Serial.print(valorLDR);
    Serial.println(" %");

    while (digitalRead(pinoBotao) == LOW) {
    
  }
 }


 


}
