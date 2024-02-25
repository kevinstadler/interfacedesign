/* Inkludieren der Servo.h library fuer die Servo-Steuerung */
#include<Servo.h>

/* Definiere die minimal und maximal postion der Servos */
#define SVO_POS_MIN     500
#define SVO_POS_MAX     2500
#define SVO_POS_MID     1500

/* Deklarieren der Servos als globale Objekte/Variablen mit Namen myServoX und myServoY */
Servo myServoX;
Servo myServoY;
Servo myServoY_End;

int myServoPosX     = SVO_POS_MID;
int myServoPosY     = SVO_POS_MID;
int myServoPosY_End = SVO_POS_MID;

void setup() {
  Serial.begin(115200); /* Initialize Serial with 115200 BoudRate */

  /* Setzen der Servo-Pins */
  myServoX.attach(5); /* Attach servo PWM signal to pin 5 */
  myServoY.attach(6); /* Attach servo PWM signal to pin 6 */
  myServoY_End.attach(7); /* Attach servo PWM signal to pin 6 */
}

void loop() {

  myServoX.write(myServoPosX); /* Update position of servo X */
  myServoY.write(myServoPosY); /* Update position of servo Y */
  myServoY_End.write(myServoPosY_End); /* Update position of servo Y */
  
  while (Serial.available() > 0) /* while there is data available repeat reading */
  {
    char data = Serial.read(); /* read one byte from the received data */

    switch(data)
    {
      case 'a':
        myServoPosX = myServoPosX + 1;
        break;
        
      case 'd':
        myServoPosX = myServoPosX - 1;
        break;

      case 'w':
        myServoPosY = myServoPosY + 1;
        break;
        
      case 's':
        myServoPosY = myServoPosY - 1;
        break;

      case 'r':
        myServoPosY_End = myServoPosY_End + 1;
        break;
        
      case 'f':
        myServoPosY_End = myServoPosY_End - 1;
        break;

      default:
        break;
    }

    /* Beschraencken auf min und max Grenzen */
    if(myServoPosX < SVO_POS_MIN) myServoPosX = SVO_POS_MIN;
    if(myServoPosX > SVO_POS_MAX) myServoPosX = SVO_POS_MAX;
    if(myServoPosY < SVO_POS_MIN) myServoPosY = SVO_POS_MIN;
    if(myServoPosY > SVO_POS_MAX) myServoPosY = SVO_POS_MAX;
    if(myServoPosY_End < SVO_POS_MIN) myServoPosY_End = SVO_POS_MIN;
    if(myServoPosY_End > SVO_POS_MAX) myServoPosY_End = SVO_POS_MAX;

    Serial.print("ServoX: ");Serial.print(myServoPosX);
    Serial.print("\tServoY: ");Serial.print(myServoPosY);
    Serial.print("\tServoY End: ");Serial.println(myServoPosY_End);
  }
}


