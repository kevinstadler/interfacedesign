// Light Scanner

#include <Servo.h>
Servo myservo;  // create servo object to control a servo
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogReference(INTERNAL);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
  float brightestVal = 0;
  int brightestPos = 0;

  for (int i = 0; i < 180 ; i = i + 1)
  {
    // put your main code here, to run repeatedly:
    float valOfLightSensor = analogRead(A0);

    if (brightestVal < valOfLightSensor)
    {
      brightestVal = valOfLightSensor;
      brightestPos = i;
    }

    Serial.print(brightestVal);
    Serial.print(" ");
    Serial.println(valOfLightSensor);

    myservo.write(i);
    delay(10);
  }

  myservo.write(brightestPos);

  delay(5000);

  for (int i = brightestPos; i >= 0 ; i = i - 1)
  {
    myservo.write(i);
    delay(10);
  }
}


