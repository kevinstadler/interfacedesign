#include <Servo.h>

Servo basis;
Servo y;
Servo tip;

// from 1000 to 2000 -- 90deg = 500ms, 5.55ms per degree
uint32_t basis0 = 1500;
uint32_t y0 = 1500;
uint32_t tip0 = 1500;

uint32_t bss;
uint32_t yy;
uint32_t tp;

// a = 10mm (offset der y-achse gegenüber der z-achse)
// b = 25mm (position der y-achse über der base plate)
// y = 50mm (länge des y arms)
// o = 0mm (tip offset!)

void setup() {
  Serial.begin(115200);
  moveBasis(0);
  moveY(0);
  moveTip(0);

  basis.attach(2);
  y.attach(3);
  tip.attach(4);
}

void moveBasis(float angle) {
  bss = angle;
  basis.writeMicroseconds(basis0 + 500*angle/90);
}
void moveY(float angle) {
  yy = angle;
  y.writeMicroseconds(y0 + 500*angle/90);
}
void moveTip(float angle) {
  tp = angle;
  tip.writeMicroseconds(tip0 + 500*angle/90);
}

void loop() {
  char data = Serial.read();
  switch (data) {
    case 'a':
      moveBasis(bss + 1);
      break;
    case 'd':
      moveBasis(bss - 1);
      break;
    case 'w':
      moveTip(tp - 1);
      break;
    case 's':
      moveTip(tp + 1);
      break;
    case 'q':
      moveY(yy + 1);
      break;
    case 'e':
      moveY(yy - 1);
      break;
  }
}
