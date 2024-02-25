#include <Servo.h>
Servo myservo;  // create servo object to control a servo
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogReference(INTERNAL);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}
float valPast = 0;
void loop() {
  // put your main code here, to run repeatedly:
  float valOfLightSensor = analogRead(A0);
  valOfLightSensor = valOfLightSensor + analogRead(A0);
  valOfLightSensor += analogRead(A0);
  valOfLightSensor += analogRead(A0);
  valOfLightSensor = valOfLightSensor / 4;

  valPast = (valOfLightSensor + valPast * 3) / 4;
  
  int servoPos = map(valPast, 220, 280, 0, 180);

  if(servoPos < 0) servoPos = 0;
  if(servoPos > 180) servoPos = 180;
  
  Serial.println(valPast);
  myservo.write(servoPos);  
  delay(10);
}
