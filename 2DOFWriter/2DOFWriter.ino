#include <Servo.h>

#define LASER 0
Servo yaw;
Servo tilt;

#define X0YAW 105
#define Y0TILT 160
#define DISTANCE 300.0 // in cm

void setup() {
  pinMode(LASER, OUTPUT);
  laser(false);
  yaw.attach(4);
  tilt.attach(5);
  Serial.begin(115200);
}

void laser(bool on) {
  digitalWrite(LASER, on);
}

#define PI 3.141592654
double coordToAngle(double flatCoord) {
    return atan2(flatCoord, DISTANCE) * 180 / PI;
}

#define N 20
float sequence[N][2] = {
  // H
  { 0, 100 },
  { 0, 0 },
  { 0, 50 },
  { 20, 50 },
  { 20, 100 },
  { 20, 0 },
  { 0, 100 },
};

void goTo(float coords[]) {
  if (coords[0] < 0) {
    yaw.write(X0YAW - coordToAngle(coords[0]));
  } else {
    yaw.write(X0YAW + coordToAngle(coords[0]));
  }
  if (coords[1] < 0) {
    tilt.write(Y0TILT + coordToAngle(coords[1]));
  } else {
    tilt.write(Y0TILT - coordToAngle(coords[1]));
  }
}

#define STEP 20 // ms
void drawLine(float from[], float to[], bool ls, float cmPerStep = 1) {
  goTo(from);
  laser(ls);
  float dist = sqrt(pow(to[0] - from[0], 2) + pow(to[1] - from[1], 2));
  uint16_t nsteps = dist / cmPerStep;
  Serial.println(dist);
  Serial.println(ls);
  for (uint16_t i = 0; i < nsteps; i++) {
    goTo(new float[2]{ from[0] + i * (to[0] - from[0]) / nsteps, from[1] + i * (to[1] - from[1]) / nsteps });
    delay(STEP);
  }
}

void loop() {
  for (uint8_t i = 0; i < 6; i++) {
    Serial.println(i);
    drawLine(sequence[i], sequence[i+1], (i % 2) == 0);
  }
  delay(2000);
}
