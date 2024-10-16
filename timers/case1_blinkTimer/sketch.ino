#include "driver/periph_ctrl.h"
#include "driver/timer.h"

#define D13 4
hw_timer_t * timer = NULL;


void IRAM_ATTR timeout_handler()
{
  int val = digitalRead(D13);
  val = (~val & 0x00000001);
  digitalWrite(D13, val);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(D13, OUTPUT);
  digitalWrite(D13,0);

  timer = timerBegin(8000);
  timerAttachInterrupt(timer, &timeout_handler);
  timerWrite(timer, 10000);
  timerAlarm(timer, 10000, true, 0);
}


void loop() {
  // put your main code here, to run repeatedly:
  
}
