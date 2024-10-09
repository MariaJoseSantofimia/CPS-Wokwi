#include <Arduino.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

xTaskHandle xTask1Handle;


void vTask1(void* pvParam)
{
  const char *msg = "Task 1 is running\r\n";
  for(;;) {
    printf(msg);
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Espera de 10 ms
    vTaskPrioritySet(NULL, 1);  // Cambia la prioridad de la propia tarea a 1
  }
  vTaskDelete(NULL);
}

void vTask2(void* pvParam)
{
  const char *msg = "Task 2 is running\r\n";

  unsigned portBASE_TYPE uxPriority;
  uxPriority = uxTaskPriorityGet(NULL);  // Obtiene la prioridad de la propia tarea

  for(;;) {
    printf(msg);  
    vTaskPrioritySet(xTask1Handle, (uxPriority + 2));  // Incrementa la prioridad de Task 1
  }
  vTaskDelete(NULL);
}


void vTask3(void* pvParam)
{
  const char *msg = "Task 3 is running\r\n";

  for(;;) {
    printf(msg); 
  }
  vTaskDelete(NULL);
}



void app_main(void)
{
  if(xTaskGetSchedulerState()==taskSCHEDULER_RUNNING)
  printf("Scheduler is running\n");

  xTaskCreate(vTask1, "Task 1", 1500, NULL, 1, &xTask1Handle);
  //xTaskCreate(vTask1, "Task 1", 1500, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task 2", 1500, NULL, 3, NULL);
  xTaskCreate(vTask3, "Task 3", 1500, NULL, 3, NULL);

  //vTaskStartScheduler();
}

void setup() {
  // put your setup code here, to run once:
  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
}
