/***************************************************
 * Gabriel Correa - Piscar Led
 * 
 * Codigo que faz o LED interno da ESP32
 * piscar com intervalos de 1 segundo
 */

//Declarando a variavel dos pinos.
const int pinoLED = 2;
const int pinoBotao = 4;

//criar a variavel que irá verificar o estado atual do botao.
int estadoBotao = 0;

//criar variavel que ira mostrar o estado atual do botao (chamamos de flag por padronização).
int flag = 0;

void setup() {
  //Declaracao do pino como saida
  pinMode(pinoLED, OUTPUT); //pinMode() é a função para definir o pin da GPIO, e qual a forma de interação com o usuário (I/O).
  pinMode(pinoBotao, INPUT);

}

void loop() {
  //Gravar variavel estadoBotao o estado do botao
  estadoBotao = digitalRead(pinoBotao); //digitalRead() é a função para ler o valor de um pino digital (sendo HIGH ou LOW).

  //Se o botao estiver pressionado aguarda 30 milisegundos, e então faz a nova leitura e altera o estado
  //atual do Led salvando na variavel flag o seu novo estado.
  if (estadoBotao == LOW) {
    delay(30);
    estadoBotao = digitalRead(pinoBotao);

    if (flag == 0) {
      digitalWrite(pinoLED, HIGH); //digitalWrite() é a função que adiciona um valor HIGH ou LOW para  um pino digital. Ver documentação para + info.
      flag = 1;
    } else {
      digitalWrite(pinoLED, LOW);
      flag = 0;
    }

    while (digitalRead(pinoBotao) == LOW){
      //fica aqui aguardando soltar o botão
    }
  }
}
