/*****************************************
 *  Codigo para verificação de luminosidade
 *  Gabriel
 * 
 */

// Declaração da variavel do pino do sensor LDR
const int pinoLDR = 15;

// Variavel que recbe a leitura dos valores do sensor
int valorLDR = 0;


void setup() {
  // Iniciando a comunicação com o serial para fazer a leitura no Monitor Serial
  Serial.begin(115200); // inicia a porta serial, configura a taxa de dados para 115200 bps
  
}

void loop() {
  valorLDR = analogRead(pinoLDR);

  //Atraves dessa leitura, chegamos as seguintes conclusoes:
  //Quanto maior a luminosidade, mais proximo de 0 temos a variavel valorLDR 
  //Quanto menor a luminosidade, mais proximo de 4096 temos a variavel valorLDR
  
  //Mapeamos o valor do LDR para, ao inves de mostrar valor de 4095 a 0, mostrar valores de 0 a 100%    
    
  valorLDR = map(valorLDR, 4095, 2500, 0, 100);
  
  Serial.print(valorLDR); // Serial.print() Imprime dados na porta serial com texto ASCII
  Serial.println(" %"); // Serial.println() Imprime dados na porta serial como texto ASCII seguido pelo caratere de retorno de carruagem (ASCII 13, ou '\r') e um cactere de nova linha (ASCII 10, ou '\n').
  delay(2500);
  
}
