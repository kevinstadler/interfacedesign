#include <Servo.h>

Servo yaw;
Servo tilt;

#define YAW A1
uint16_t yawZero;
uint16_t yawSD;
uint16_t tiltZero;
uint16_t tiltSD;
#define TILT A0
#define SWITCH A2

#define LASER 7
bool laser = true;

//#define CALIBRATION 100
#define DEADZONE 70 // ignore joystick positions (analog 512) plus minus this

void setup() {
  digitalWrite(SWITCH, HIGH);
  pinMode(LASER, OUTPUT);
  digitalWrite(LASER, laser);

  Serial.begin(115200);
  yaw.attach(2);
  tilt.attach(3);
  yaw.write(90);
  tilt.write(90);

  delay(100);
  uint32_t yawSum = 0;
  uint32_t tiltSum = 0;
  // figure out average + sd
/*  for (uint16_t i = 0; i < CALIBRATION; i++) {
    yawSum += analogRead(YAW);
    tiltSum += analogRead(TILT);
  }
  yawZero = yawSum / CALIBRATION;
  tiltZero = tiltSum / CALIBRATION;*/
}

int16_t joystick(uint32_t pin) {
  uint32_t val = analogRead(pin);
  if (val >= 512 - DEADZONE && val <= 512 + DEADZONE) {
    return 0;
  }
  return map(val, 0, 1023, -2, 2);
}

bool switchPin = false;

#define NPOINTS 4
uint16_t yaws[NPOINTS];
uint16_t tilts[NPOINTS];
uint8_t i = 0;

#define TPOINTS 20

uint16_t interp(uint16_t from, uint16_t to, uint8_t t) {
  return ((TPOINTS - t) * from + t * to) / TPOINTS;
}

void loop() {
  if (i >= NPOINTS) {
    for (i = 0; i < NPOINTS; i++) {
      for (uint8_t j = 0; j < TPOINTS; j++) {
        yaw.write(interp(yaws[i], yaws[(i+1)%NPOINTS], j));
        tilt.write(interp(tilts[i], tilts[(i+1)%NPOINTS], j));
        delay(50);
      }
    }
    return;
  }
  uint32_t yw = analogRead(YAW);
  uint32_t tlt = analogRead(TILT);
  if (digitalRead(SWITCH)) {
    switchPin = false;
  } else if (!switchPin) {
    switchPin = true;
    yaws[i] = yaw.read();
    tilts[i] = tilt.read();
    Serial.print(yaw.read());
    Serial.print(",");
    Serial.println(tilt.read());
    i++;
//    laser = !laser;
//    digitalWrite(LASER, laser ? HIGH : LOW);
  }

  yaw.write(yaw.read() + joystick(YAW));
  tilt.write(tilt.read() + joystick(TILT));
  delay(30);
}
