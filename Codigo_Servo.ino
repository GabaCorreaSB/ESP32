/*
Ratreador Solar
*/

#include <Servo.h> //incluindo bilioteca servo
Servo servo; // nome do servo
//descrever pinos
#define SER_PIN 3
#define POT_PIN A0
#define FOT_PIN A1
#define FOT_PIN1 A2
#define  LAMPE 5

int potState;
int ledBright;
int daleyTime;
int fotlevel;
int fotlevel_1;
int angle;
void setup ()
{
  pinMode (SER_PIN, OUTPUT);
  pinMode (LAMPE, OUTPUT);
  pinMode (FOT_PIN, OUTPUT);
  Serial.begin (9600);
  Serial.println ("Começando ...");
  delay (1000);
  angle = 90;
  servo.attach (3);
  servo.write (90);
}

void loop ()
{
  potState=analogRead (POT_PIN);
  fotlevel=analogRead(FOT_PIN) ;
    fotlevel_1=analogRead(FOT_PIN1);
  daleyTime =map (potState, 0, 1023,1,100);

  if (fotlevel_1>fotlevel) {
    angle+=+1;
    servo.write(angle);
    delay (daleyTime) ; 
  }
  if (fotlevel_1<fotlevel) {
      angle+=-1;
    servo.write(angle);
    delay(daleyTime);
  }
  if (fotlevel<50 && fotlevel_1<50) {
    digitalWrite (LAMPE,HIGH);
    servo.write (90);
}
  else {
    digitalWrite(LAMPE,LOW);
  }

  Serial.print ("angle: ") ;
  Serial.println (angle);
  Serial.print ("fot");
  Serial.println (fotlevel);
  delay (100);
}
