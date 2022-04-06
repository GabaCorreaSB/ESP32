/*******************************************************
 * Codigo para conexão com a rede lorawan e o Dashboard
 * para monitorar todos os modulos do IoT DevKit
 *
 */

#if !defined(ARDUINO_ESP32_DEV) // ESP 32
#error Use this example with the ESP32
#endif

//------------------------------------------------------
// Libraries

// Incluindo todas as libraries que usaremos no codigo

#include "RoboCore_SMW_SX1276M0.h"
#include "DHT.h"
#include "ArduinoJson.h"
#include "RoboCore_MMA8452Q.h"
#include "HardwareSerial.h"

//------------------------------------------------------
// Variaveis

// Informando ao ESP32 quais pinos iremos conectar ao módulo LoRaWAN Bee
HardwareSerial LoRaSerial(2);
#define RXD2 16
#define TXD2 17

// Informando ao ESP32 quais pinos os sensores, botões e os LEDs estão ligados
const int pinoDHT = 12;
const int pinoBotao = 4;
const int pinoLDR = 15;
const int statusLED = 13;
const int sendLED = 2;

// Iniciando as variaves que irão realizar as leituras do botão e do LDR
int estadoBotao = 0;
int valorLDR = 0;

// Criando as instancias do DHT11, Acelerometro e Lorawan, que serão usadas no decorrer do codigo
DHT dht(pinoDHT, DHT11);
MMA8452Q acel;
SMW_SX1276M0 lorawan(LoRaSerial);

CommandResponse response;

// Inserindo as chaves APPEUI e APPKEY, conforme configurado na plataforma ProIoT
const char APPEUI[] = "50f8a500000104c8"; // EUI Application
const char APPKEY[] = "6f3246e0501cd68fd817dd733f781db9"; //Application Key

// Criando variavel para determinar de quanto em quanto tempo a informação via LoRaWan sera enviada
const unsigned long PAUSE_TIME = 300000; // [ms] (5 min)
unsigned long timeout;

//--------------------------------------------------------
// Prototypes

void event_handler(Event);

//--------------------------------------------------------
//--------------------------------------------------------

void setup() {

  // Iniciando a comunicação com o serial (ESP32 > PC)
  Serial.begin(115200);
  Serial.println(F("--- SMW_SX1276M0 Join (OTAA) ---"));

  // Iniciando a comunicação serial (ESP32 > LoRaWAN Bee)
  LoRaSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);

  // Criando uma variavel que ira "escutar" se algo bater no modulo LoRaWAN Bee
  lorawan.event_listener = &event_handler;
  Serial.println(F("Handler set"));

  // Resetando o modulo LoRaWAN Bee para configurar novamente
  delay(1000);
  lorawan.setPinReset(5);
  lorawan.reset();

  // Lendo o DEVEUI do modulo LoRaWAN Bee e mostrando no monitor serial
  char deveui[16];
  response = lorawan.get_DevEUI(deveui);
  if(response == CommandResponse::OK){
    Serial.print(F("DevEUI: "));
    Serial.write((uint8_t *)deveui, 16);
    Serial.println();
  } else {
    Serial.println(F("Error getting the Device EUI"));
  }

  // Configurando a chave APPEUI no modulo e mostrando no Monitor Serial
  response = lorawan.set_AppEUI(APPEUI);
  if(response == CommandResponse::OK){
    Serial.print(F("Application EUI set ("));
    Serial.write((uint8_t *)APPEUI, 16);
    Serial.println(')');
  } else {
    Serial.println(F("Error setting the Application EUI"));
  }

  // Configurando a chave APPKEY no modulo e msotrando no monitor Serial
  response = lorawan.set_AppKey(APPKEY);
  if(response == CommandResponse::OK){
    Serial.print(F("Application Key set ("));
    Serial.write((uint8_t *)APPKEY, 32);
    Serial.println(')');
  } else {
    Serial.println(F("Error setting the Application Key"));
  }

  // Configurando o modo de operação do LoRaWAN Bee para OTAA
  response = lorawan.set_JoinMode(SMW_SX1276M0_JOIN_MODE_OTAA);
  if(response == CommandResponse::OK){
    Serial.println(F("Conncetion mode set to OTAA"));
  } else {
    Serial.println(F("Error setting the join mode"));
  }

  // Começando as tentativas de conexão na rede LoRaWAN da ATC
  Serial.println(F("Joining the network"));
  lorawan.join();

  // Configurando os pinos como entrada e saida, conforme a sua função
  pinMode(pinoBotao, INPUT);
  pinMode(pinoLDR, OUTPUT);
  pinMode(sendLED, OUTPUT);

  // Iniciando os sensores DHT11 e Acelerometro
  dht.begin();
  acel.init();
  

}

//--------------------------------------------------------
// Criando uma função para fazer a leitura dos dados, seja o codigo
// enviando automaticamente para o payload para a plataforma, ou quando
// Pressionamos o botao

void enviaDados(){

  // Acendendo o LED azul do ESP32 quando inicamos a leitura e o envio dos dados
  digitalWrite(sendLED, HIGH);

  // Fazendo a leitura do sensor de luminosidade LDR e mapeando o valor
  valorLDR = analogRead(pinoLDR);
  valorLDR = map(valorLDR, 4095, 2500, 0, 100);

  // Fazendo a leitura da temperatura e da umidade
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // Fazendo a leitura do acelerometro e multiplicando os valores por 100,
  // para tornalos valores inteiros e facilitar o envio para a plataforma
  acel.read();
  int x = acel.x * 100;
  int y = acel.y * 100;
  int z = acel.z * 100;

  // Criando uma variavel JSON para armazenar os valores das variaveis acima
  DynamicJsonDocument json(JSON_OBJECT_SIZE(6));

  // Configurando a variavel JSON com os Alias criados na Plataforma ProIoT
  // Savlando em cada componente o respectivo valor da variavel lida
  json["T"] = temperatura;
  json["U"] = umidade;
  json["L"] = valorLDR;
  json["X"] = x;
  json["Y"] = y;
  json["Z"] = z;

  // Criando uma String chamada payload que ira conter todas as informacoes do JSON
  String payload = "";
  serializeJson(json, payload);

  Serial.print("Valores enviados: ");
  Serial.println(payload);

  // Enviando todas as informaç~es via LoRaWAN
  lorawan.sendT(1, payload);

  // Apagando o Led azul do esp32 quando terminar o envio
  digitalWrite(sendLED, LOW);
  
}

void loop() {
  // "Escutando" se algo vem do modulo LoRaWAN Bee
  lorawan.listen();

  // Condição para ver se esta conectado a rede na rotina atual
  if(lorawan.isConnected()){

    // Faz a leitura do botao e acessa a função de envio de dados
    estadoBotao = digitalRead(pinoBotao);
    if(estadoBotao == LOW){
      delay(30);
      estadoBotao = digitalRead(pinoBotao);
      enviaDados();
      while(digitalRead(pinoBotao) == LOW){
        // Espero soltar o botao
      }
    }

    // A cada PAUSE_TIME milisegundos, acessamos a função de envio de dados
    if(timeout < millis()){

      enviaDados();

      timeout = millis() + PAUSE_TIME;
    }
  } else {

    // Se nao conseguir se conectar a rede LoRaWAN, imprime no monitor serial
    // "." a cada 5 segundos
    if(timeout < millis()){
      Serial.println('.');
      timeout = millis() + 5000; // 5 s
    }
  }

}

//--------------------------------------------------------
//--------------------------------------------------------

// Verificando o que foi recebido do modulo LoRaWAN Bee e, se o que foi recebido
// for um evento do tipo JOINED, informa que se conectou no MOnitor Serial e
// acende o LED do IoT DevKit

void event_handler(Event type){
  if(type == Event::JOINED){
    Serial.println(F("Joined"));
    digitalWrite(statusLED, HIGH);
  }
}
