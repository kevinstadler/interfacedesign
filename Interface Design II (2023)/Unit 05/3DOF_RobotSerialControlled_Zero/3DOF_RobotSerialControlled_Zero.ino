/* Inkludieren der Servo.h library fuer die Servo-Steuerung */
#include<Servo.h>

/* Definiere die minimal und maximal postion der Servos */
#define SVO_POS_MIN     500
#define SVO_POS_MAX     2500
#define SVO_POS_MID     1500

int myServoPosX_Zero     = 1500;
int myServoPosY_Zero     = 1510;
int myServoPosY_End_Zero = 2005;

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

    switch (data)
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

      case 'Z':
        myServoPosX_Zero = myServoPosX;
        myServoPosY_Zero = myServoPosY;
        myServoPosY_End_Zero = myServoPosY_End;
        break;

      case 'z':
        myServoPosX = myServoPosX_Zero;
        myServoPosY = myServoPosY_Zero;
        myServoPosY_End = myServoPosY_End_Zero;
        break;

      case '1':
        DriveLinear(1100, 1700, 2000, 2000);
        break;
      case '2':
        DriveLinear(1600, 1300, 900, 2000);
        break;
      case '3':
        DriveLinear(1100, 1952, 604, 1500);
        DriveLinear(1400, 1800, 1652, 1500);
        DriveLinear(1800, 2200, 1552, 1500);

        DriveLinear(myServoPosX_Zero, myServoPosY_Zero, myServoPosY_End_Zero, 1500);

        break;

      default:
        break;
    }

    /* Beschraencken auf min und max Grenzen */
    if (myServoPosX < SVO_POS_MIN) myServoPosX = SVO_POS_MIN;
    if (myServoPosX > SVO_POS_MAX) myServoPosX = SVO_POS_MAX;
    if (myServoPosY < SVO_POS_MIN) myServoPosY = SVO_POS_MIN;
    if (myServoPosY > SVO_POS_MAX) myServoPosY = SVO_POS_MAX;
    if (myServoPosY_End < 600) myServoPosY_End = 600;
    if (myServoPosY_End > 2400) myServoPosY_End = 2400;

    Serial.print("ServoX: "); Serial.print(myServoPosX);
    Serial.print("\tServoY: "); Serial.print(myServoPosY);
    Serial.print("\tServoY End: "); Serial.println(myServoPosY_End);
  }
}



void DriveLinear(int TargetPosX, int TargetPosY, int TargetPosY_End, int TimeMS)
{
  int timePast = 0;
  int tmpStartedX = myServoPosX;
  int tmpStartedY = myServoPosY;
  int tmpStartedY_End = myServoPosY_End;

  while (timePast < TimeMS)
  {
    myServoPosX = tmpStartedX + ((float)(TargetPosX - tmpStartedX) *
                                 (float)timePast / (float)TimeMS);
    myServoPosY = tmpStartedY + ((float)(TargetPosY - tmpStartedY) *
                                 (float)timePast / (float)TimeMS);
    myServoPosY_End = tmpStartedY_End + ((float)(TargetPosY_End - tmpStartedY_End) *
                                         (float)timePast / (float)TimeMS);

    myServoX.writeMicroseconds(myServoPosX); /* Update position of servo X */
    myServoY.writeMicroseconds(myServoPosY); /* Update position of servo Y */
    myServoY_End.writeMicroseconds(myServoPosY_End); /* Update position of servo Y */

    delay(1);
    timePast += 1;
  }
}


