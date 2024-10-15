#include <Arduino.h>
#include "driver/periph_ctrl.h"
#include "driver/timer.h"

//#define ESPIDF

#define D13 4 


#ifdef ESPIDF
void IRAM_ATTR timeout_handler(void *para)
{
  TIMERG0.int_clr_timers.t0 = 1;

  int val = gpio_get_level(GPIO_NUM_18);
  val = (~val & 0x00000001);
  gpio_set_level(GPIO_NUM_18, val);

  timer_set_alarm(TIMER_GROUP_0, TIMER_0, TIMER_ALARM_EN);
}
#else
hw_timer_t * timer = NULL;


void IRAM_ATTR timeout_handler()
{
  int val = digitalRead(D13);
  val = (~val & 0x00000001);
  digitalWrite(D13, val);
}
#endif


void setup() {
#ifdef ESPIDF
  pinMode(D13, OUTPUT);
  gpio_set_level(GPIO_NUM_18, 0);

  timer_config_t config; 
    config.alarm_en = TIMER_ALARM_EN; 
    config.counter_en = TIMER_PAUSE; 
    config.intr_type = TIMER_INTR_LEVEL;
    config.counter_dir = TIMER_COUNT_UP; 
    config.auto_reload = true; 
    config.divider = 8000; 

  
  timer_init(TIMER_GROUP_0,TIMER_0,&config);

  timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);

  timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 10000);

  timer_isr_register(TIMER_GROUP_0, TIMER_0, timeout_handler,
                       (void *) TIMER_0, ESP_INTR_FLAG_IRAM, NULL);

  timer_start(TIMER_GROUP_0, TIMER_0);

#else
  // put your setup code here, to run once:
  pinMode(D13, OUTPUT);
  digitalWrite(D13,0);

  timer = timerBegin(8000);
  timerAttachInterrupt(timer, &timeout_handler);
  timerWrite(timer, 10000);
  timerAlarm(timer, 10000, true, 0);
#endif
}


void loop() {
  // put your main code here, to run repeatedly:
  
}