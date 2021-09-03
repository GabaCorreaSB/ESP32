/****************************************************
 * Gabriel Correa - Kit IOT
 * 
 * Codigo para realizar a leitura da luminosidade no local,
 * e então definir o estado do LED diante a % de luminosidade.
 * 
 */
 
// Definindo os pinos dos sensores
const int pinoLDR = 15;
const int pinoLED = 2;

// Definindo o valor inicia da medição de luminosidade
int valorLDR = 0;


void setup() {

  // Iniciando a conexão com o SERIAL na velocidade especificada entre ().
  Serial.begin(115200);

  // deinifir modo de I/O do pino de LED.
  pinMode(pinoLED, OUTPUT);
  
}

// Função para piscar o LED.
void blink_led() {
  
  digitalWrite(pinoLED, HIGH);
  delay(100);
  digitalWrite(pinoLED, LOW);
  delay(100);
  
}

// Função para ligar o LED.
void led_on() {
  digitalWrite(pinoLED, HIGH);
}

// Função para desligr o LED.
void led_off(){
  digitalWrite(pinoLED, LOW);
}


void loop() {
  // definindo a entrada dos dados na variavel de valor (de maneira analogica).
  valorLDR = analogRead(pinoLDR);

  // realizando o mapeamento do menor valor para 0% e o maior para 100% ao invés de menor 4095 e maior 100.
  valorLDR = map(valorLDR, 4095, 2500, 0, 100);

  // criando e definindo a condição de que se a luminosidade estiver mais que 50% desligar o LED,
  // abaixo de 50% ligar o LED, e ao atingir 0% de luminosidade piscar o LED.
  if (valorLDR >= 50) {
    led_off();
  } else if (valorLDR == 0) {
    blink_led();
  } else {
    led_on();
  }
  
  // Printar as informações da luminosidade no monitor serial em %
  Serial.print(valorLDR);
  Serial.println(" %");
  delay(250);
 
}
