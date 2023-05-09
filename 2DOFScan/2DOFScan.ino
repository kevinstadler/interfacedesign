#include <Servo.h>

Servo yaw;
Servo tilt;

void setup() {
  Serial.begin(115200);
  yaw.attach(2);
  tilt.attach(3);
}

#define SCANRESOLUTION 5

void loop() {
  uint32_t topValue = 0;
  uint8_t topYaw;
  uint8_t topTilt;

  for (uint8_t tlt = 0; tlt <= 180; tlt += SCANRESOLUTION) {
    tilt.write(tlt);
    int16_t direction = ((tlt / SCANRESOLUTION) % 2 == 1) ? 1 : -1;
    int16_t yw = (direction == 1) ? 0 : 180;
    for (; yw >= 0 && yw <= 180; yw += direction * SCANRESOLUTION) {
      yaw.write(yw);
      delay(30);
      uint32_t val = readSensor();
      Serial.print(tlt);
      Serial.print(",");
      Serial.print(yw);
      Serial.print(",");
      Serial.print(direction);
      Serial.print(",");
      Serial.println(val);
      if (val > topValue) {
        topValue = val;
        topYaw = yw;
        topTilt = tlt;
      }
    }
  }

  yaw.write(topYaw);
  tilt.write(topTilt);
  delay(5000);
}

#define NSAMPLES 5
uint32_t readSensor() {
  // 800 in bright sunlight
  // 400k in total darkness
  // 10 bit = [0, 1023]
  uint32_t ret = 0;
  for (uint8_t i = 0; i < NSAMPLES; i++) {
    ret += analogRead(A0);
  }
  return ret / NSAMPLES;
}
