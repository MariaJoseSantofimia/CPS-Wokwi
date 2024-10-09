#include <Arduino.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void vTaskPeriodic(void* pvParam)
{
  const char *msg = "Periodic Task is running\r\n";
  portTickType xLastWakeTime;

  xLastWakeTime = xTaskGetTickCount();

  for(;;) {
    printf(msg); 
    vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_RATE_MS ) );
  }
}


void vTask1(void* pvParam)
{
  const char *msg = "Continuous Task 1 is running\r\n";

  // Crear la tarea periódica una vez al inicio
  xTaskCreate(vTaskPeriodic, "Task Periodic", 1500, NULL, 2, NULL);

  for(;;) {
    printf(msg);
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Añadir retardo de 1 segundo
  }
  vTaskDelete(NULL);  // Eliminar tarea (aunque no se ejecutará)
}

void vTask2(void* pvParam)
{
  const char *msg = "Continuous Task 2 is running\r\n";
 
  for(;;) {
    printf(msg); 
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Añadir retardo para liberar CPU
  }
  vTaskDelete(NULL);  // Eliminar tarea (aunque no se ejecutará)
}



void app_main(void)
{
  if(xTaskGetSchedulerState()==taskSCHEDULER_RUNNING)
  printf("Scheduler is running\n");

  xTaskCreate(vTask1, "Task Continuous 1", 1000, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task Continuous 2", 1000, NULL, 3, NULL);

  //vTaskStartScheduler();
}

void setup() {
  // put your setup code here, to run once:
  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
}
