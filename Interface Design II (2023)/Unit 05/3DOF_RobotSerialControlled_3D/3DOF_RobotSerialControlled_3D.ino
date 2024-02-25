/* Inkludieren der Servo.h library fuer die Servo-Steuerung Einfuehrung in Robotik Daniel Schatzmayr*/
#include<Servo.h>

/* Definiere die minimal und maximal postion der Servos */
#define SVO_POS_MIN     500
#define SVO_POS_MAX     2500
#define SVO_POS_MID     1500

int myServoPosX_Zero     = 1500;
int myServoPosY_Zero     = 1500;
int myServoPosY_End_Zero = 1500;

/* Deklarieren der Servos als globale Objekte/Variablen mit Namen myServoX und myServoY */
Servo myServoX;
Servo myServoY;
Servo myServoY_End;

int myServoPosX     = myServoPosX_Zero;
int myServoPosY     = myServoPosY_Zero;
int myServoPosY_End = myServoPosY_End_Zero;

int PWM_Servo1_180          =   (900 * 2);
int PWM_Servo2_180          =   (900 * 2);
int PWM_Servo3_180          =   (900 * 2);

float dist_Servo1To2_Back  = 28.0f;
float dist_BaseTo2_Up      = 40.0f;
float dist_Servo2To3       = 50.0f;
float dist_Servo3ToTip     = 56.0f;
float dist_Servo1To_ToLeft = 0.0f;

void setup() {
  Serial.begin(115200); /* Initialize Serial with 115200 BoudRate */

  /* Setzen der Servo-Pins */
  myServoX.attach(2); /* Attach servo PWM signal to pin 5 */
  myServoY.attach(3); /* Attach servo PWM signal to pin 6 */
  myServoY_End.attach(4); /* Attach servo PWM signal to pin 6 */
}

void squareWithX() {
  DriveLinear3D(-10, 35, 0, 1000);
  DriveLinear3D(10, 35, 0, 1000);
  DriveLinear3D(10, 55, 0, 1000);
  DriveLinear3D(-10, 55, 0, 1000);
  DriveLinear3D(-10, 35, 0, 1000);
  DriveLinear3D(-10, 35, 30, 1000);

  // 1 -> 3
  DriveLinear3D(-10, 35, 0, 1000);
  DriveLinear3D(10, 55, 0, 1000);
  DriveLinear3D(10, 55, 30, 1000);

  // 2 -> 4
  DriveLinear3D(10, 35, 0, 1000);
  DriveLinear3D(-10, 55, 0, 1000);
  DriveLinear3D(-10, 55, 30, 1000);
}

float XOffset = 0;
float YOffset = 0;
float ZOffset = 0;

bool Cal_3D_To_ServoPWM(float X, float Y, float Z, int &Servo1PWM, int &Servo2PWM, int &Servo3PWM)
{
  X += XOffset;
  Y += YOffset;
  Z += ZOffset;

  Z *= -1;

  float dist1  = sqrt( (X * X) + (Y * Y));
  float alpha1 = asin( X / dist1);

  dist1 += dist_Servo1To2_Back;

  Z += dist_BaseTo2_Up;

  float tmpAlphaTarget = atan2(Z, dist1);
  float lengthToTarget =  sqrt( (Z * Z) + (dist1 * dist1));
  float tmpAlphaBase1 = acos((dist_Servo3ToTip * dist_Servo3ToTip - dist_Servo2To3 * dist_Servo2To3 - lengthToTarget * lengthToTarget) / (lengthToTarget * dist_Servo2To3 * (-2.0f)));

  float alpha2 = tmpAlphaBase1 - tmpAlphaTarget;

  float tmpAlpha1 = asin((dist1 - cos(alpha2) * dist_Servo2To3) / dist_Servo3ToTip);
  float tmpAlpha2 = PI / 2 - alpha2;

  float alpha3 = tmpAlpha1 + tmpAlpha2;

  Servo1PWM = myServoPosX_Zero + (alpha1 / PI) * (float)PWM_Servo1_180;

  Servo2PWM = myServoPosY_Zero + ((PI / 2 - alpha2) / PI) * (float)PWM_Servo2_180;

  Servo3PWM = myServoPosY_End_Zero - ((PI - alpha3) / PI) * (float)PWM_Servo3_180;

  return true;
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

      case '0':
        Cal_3D_To_ServoPWM(30, 30, 30, myServoPosX, myServoPosY, myServoPosY_End);
        break;
      case '9':
        Cal_3D_To_ServoPWM(-30, 30, 30, myServoPosX, myServoPosY, myServoPosY_End);
        break;
      case '8':
        Cal_3D_To_ServoPWM(0, 30, 30, myServoPosX, myServoPosY, myServoPosY_End);
        break;

      case '7':

        DriveLinear3D(30, 50, 0, 1000);
        DriveLinear3D(10, 50, -2, 1000);
        DriveLinear3D(10, 30, -2, 1000);
        DriveLinear3D(30, 30, 0, 1000);

        DriveLinear3D(30, 30, 30, 1000);
        break;
      case 'x':
        DriveLinear3D(30, 30, 0, 1000);
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

float PosNow_X = 0;
float PosNow_Y = 30;
float PosNow_Z = 40;

void DriveLinear3D(int TargetPosX, int TargetPosY, int TargetPosZ, int TimeMS)
{
  int timePast = 0;
  int tmpStartedX = PosNow_X;
  int tmpStartedY = PosNow_Y;
  int tmpStartedZ = PosNow_Z;

  while (timePast < TimeMS)
  {
    PosNow_X = tmpStartedX + ((float)(TargetPosX - tmpStartedX) *
                              (float)timePast / (float)TimeMS);
    PosNow_Y = tmpStartedY + ((float)(TargetPosY - tmpStartedY) *
                              (float)timePast / (float)TimeMS);
    PosNow_Z = tmpStartedZ + ((float)(TargetPosZ - tmpStartedZ) *
                              (float)timePast / (float)TimeMS);

    Cal_3D_To_ServoPWM(PosNow_X, PosNow_Y, PosNow_Z, myServoPosX, myServoPosY, myServoPosY_End);

    myServoX.writeMicroseconds(myServoPosX); /* Update position of servo X */
    myServoY.writeMicroseconds(myServoPosY); /* Update position of servo Y */
    myServoY_End.writeMicroseconds(myServoPosY_End); /* Update position of servo Y */

    delay(1);
    timePast += 1;
  }
}


