/***************************************************
 * Gabriel Correa - Piscar Led
 * 
 * Codigo que faz o LED interno da ESP32
 * piscar com intervalos de 1 segundo
 */

//Declarando a variavel dos pinos.
const int pinoLED = 2;

void setup() {
  //Declaracao do pino como saida
  pinMode(pinoLED, OUTPUT); //pinMode() é a função para definir o pin da GPIO, e qual a forma de interação com o usuário (I/O).
 
}

void loop() {

  //Acende o Led durante 1 segundo
  digitalWrite(pinoLED, HIGH);
  delay(100);

  //Apaga o LED Durante 1 segundo
  digitalWrite(pinoLED, LOW);
  delay(100); 

}
