#include <CapacitiveSensor.h>
#include <Servo.h>

CapacitiveSensor   cs_4_2 = CapacitiveSensor(4, 2);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

Servo myServo;

void setup()
{
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(115200);
  myServo.attach(9);
}

#define READING_BUFFER_SIZE    40

int readingBuffer[READING_BUFFER_SIZE] = {0};
int currentIndex = 0;

float BufferedReadings()
{
  readingBuffer[currentIndex] = MakeReadings(10);

  currentIndex = currentIndex + 1;
  if (currentIndex == READING_BUFFER_SIZE) currentIndex = 0;

  float total1 = 0;

  for (int i = 0 ; i < READING_BUFFER_SIZE; i++)
  {
    total1 =  total1 + readingBuffer[i];
  }
  return total1 / READING_BUFFER_SIZE;
}


float MakeReadings(int howOften)
{
  float total1 = 0;
  for (int i = 0 ; i < howOften; i++)
  {
    total1 =  total1 + cs_4_2.capacitiveSensor(10);
  }
  return total1 / howOften;
}

float result = 0;

void loop()
{
  result = (result * 2.0 + BufferedReadings()) / 3.0;

  Serial.print( cs_4_2.capacitiveSensor(10));
  Serial.print("\t");
  Serial.println(result);                  // print sensor output 1

  int servoPos = result;

  if( servoPos > 180) servoPos = 180;
  
  myServo.write(servoPos);
}
