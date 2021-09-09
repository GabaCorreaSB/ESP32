/*****************************************
 * Codigo para enviar comandos para o modulo e imprimir as respostas do modulo
 * 
 */

#if !defined(ARDUINO_ESP32_DEV) // Verificar se o modelo da placa está correto
#error Use this example with the ESP32
#endif
//Incluindo o json para modificar widgets
#include <ArduinoJson.h>

//Incluindo Biblioteca 
#include "RoboCore_SMW_SX1276M0.h"

//declarando os pinos de comunicação serial do KIT
#include <HardwareSerial.h>

HardwareSerial LoRaSerial(2);
#define RXD2 16
#define TXD2 17

//Criacao do objeto lorawan para a biblioteca SMW_SX1276M0
SMW_SX1276M0 lorawan(LoRaSerial);

//Declaração da variavel que armazena as respostas do modulo
CommandResponse resposta;

//Declaracao das variaveis que armazenam as informações da rede
const char APPEUI[] = "b754502cd24c9b7a"; // EUI Application
const char APPKEY[] = "d98c268704cb1141dd6a97c7cb79e428"; //Application Key

// Declarando a variavel que armazena o "Alias" da variavel do PROIot
const char ALIAS_LDR[] = "L";

//Declaracao da variavel de intervalo de tempo
const unsigned long TEMPO_ESPERA = 300000; // 5 Minutos
unsigned long intervalo;

//Declaracao das variaves para o LDR
const int Pino_LDR = 15;
int leitura_LDR = 0;

//declaracao da função que verifica a conexão do modulo
void event_handler(Event);

//-----------------------------------------------------------
void setup() {

  Serial.begin(115200);
  Serial.println(F("Envio de Luminosidade para a Plataforma PROIoT"));

  //definindo o pino do reset do modulo
  lorawan.setPinReset(5);
  lorawan.reset(); // Realiza o reset do modulo

  LoRaSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);

  pinMode(Pino_LDR, INPUT);

  lorawan.event_listener = &event_handler;
  Serial.println(F("Evento Configurado."));

  //Requisita e imprime o devie EUI do modulo
  char deveui[16];
  resposta = lorawan.get_DevEUI(deveui);
  
  if(resposta == CommandResponse::OK){
    Serial.print(F("DevEUI: "));
    Serial.write((uint8_t *)deveui, 16);
    Serial.println();
  } else {
    Serial.println(F("Erro ao obter o Device EUI"));
  }

  // Configura o Application EUI no modulo
  resposta = lorawan.set_AppEUI(APPEUI);
  if(resposta == CommandResponse::OK){
    Serial.print(F("Application EUI configurado ("));
    Serial.write((uint8_t *)APPEUI, 16);
    Serial.println(')');
  } else {
    Serial.println(F("Erro ao configurar o Application EUI"));
  }

  resposta = lorawan.set_AppKey(APPKEY);
  if(resposta == CommandResponse::OK){
    Serial.print(F("Application Key configurado ("));
    Serial.write((uint8_t *)APPKEY, 32);
    Serial.println(')');
  } else {
    Serial.println(F("Erro ao configurar o Application Key"));
  }

  resposta = lorawan.set_JoinMode(SMW_SX1276M0_JOIN_MODE_OTAA);
  if(resposta == CommandResponse::OK){
    Serial.println(F("Metodo de Conexao configruado como OTAA"));
  } else {
    Serial.println(F("Erro ao configurar o metodo OTAA"));
  }


  //Requisita conexão com a rede
  Serial.println(F("Conectando a Rede"));
  lorawan.join();

}

void loop() {

  //Recebe as informacoes do modulo
  lorawan.listen();

  //Verificando se o modulo está conectado
  if(lorawan.isConnected()){

    //Executa o envio da mensagem uma vez a cada 5 minutos

    if(intervalo < millis()){

      leitura_LDR = analogRead(Pino_LDR);
      leitura_LDR = map(leitura_LDR, 0, 4095, 100, 0);

      //Criando o objeto dinamico json com tamanho 1para a biblioteca
      DynamicJsonDocument json(JSON_OBJECT_SIZE(1));

      //Atrelando ao objeto json a leitura do LDR com o nome do alias definido
      json[ALIAS_LDR] = leitura_LDR;

      //Mede o tamanho da mensagem "json" e atrela o valor somado em uma unidade ao objeto "tamanho_mensagem"
      size_t tamanho_mensagem = measureJson(json) + 1;

      //Criando a string "mensagem" de acordo com o tamanho do objeto acima
      char mensagem[tamanho_mensagem];

      //copia o objeto json para a variavel mensagem e com o tamanho do objeto
      serializeJson(json, mensagem, tamanho_mensagem);

      //Imprime a mensagem e envia a mensagem como texto pelo modulo
      Serial.print(F("Mensagem: "));
      Serial.println(mensagem);
      resposta = lorawan.sendT(1, mensagem);

      intervalo= millis() + TEMPO_ESPERA;
    }
  } else { // Se o modulo não estiver conectado

    //Imprime um "." a cada 5 segundos
    if(intervalo < millis()){
      Serial.println('.');
      intervalo = millis() + 5000; // Atualiza a contagem de tempo
    }
  }
}

void event_handler(Event type){

  if(type == Event::JOINED){
    Serial.println(F("Conectado!"));
  }
}
