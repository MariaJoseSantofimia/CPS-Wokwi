#include <Arduino.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\t\n";
const char *pcTextForTask3 = "Task 3 is running\r\n";

void vTask1(void* pvParam)
{
  const char *msg = "Task 1 is running\r\n";
  for(;;) {
    printf(msg);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void vTask2(void* pvParam)
{
  const char *msg = "Task 2 is running\r\n";
  for(;;) {
    printf(msg);
    //delay(1000);
    vTaskDelay(1000/portTICK_PERIOD_MS); 
  }
  vTaskDelete(NULL);
}


void vTask3(void* pvParam)
{
  const char *msg = "Task 3 is running\r\n";
 
  for(;;) {
    printf(msg); 
    vTaskDelay(1000/portTICK_PERIOD_MS); 
  }
  vTaskDelete(NULL);
}



void app_main(void)
{
  if(xTaskGetSchedulerState()==taskSCHEDULER_RUNNING)
  printf("Scheduler is running\n");
   
  xTaskCreate(vTask1, "Task 1", 1500, NULL, 3, NULL);
  xTaskCreate(vTask2, "Task 2", 1500, NULL, 3, NULL);
  xTaskCreate(vTask3, "Task 3", 1500, NULL, 3, NULL);

}

void setup() {
  app_main();
}

void loop() {

}
