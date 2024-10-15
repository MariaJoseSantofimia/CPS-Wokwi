// Servo Sweep example for the ESP32
// https://wokwi.com/arduino/projects/323706614646309460

#include <ESP32Servo.h>

const int servoPin = 14;

Servo servo;

void setup() {
  servo.attach(servoPin, 500, 2400);
}

int pos = 0;

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
}
