#include <Servo.h>
Servo myservoX;  
Servo myservoY;  
void setup() {
  myservoX.attach(6);  
  myservoY.attach(9);
}

void loop() {
  int xVal = analogRead(A0);
  int yVal = analogRead(A1);
  int posX = map(xVal,0, 1023, 0 ,180);
  myservoX.write( posX );
  myservoY.write( map(yVal,0, 1023, 0 ,180) );
}
