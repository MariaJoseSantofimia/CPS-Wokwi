#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LDR_PIN 12

void vTaskPeriodicCheckLight(void* pvParam)
{
  portTickType xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  for(;;){
    if (digitalRead(LDR_PIN) == LOW) {
      Serial.println("Light!");
    } else {
      Serial.println("Dark  ");
    }
    vTaskDelayUntil( &xLastWakeTime, (5000 / portTICK_PERIOD_MS));
  }
}

void app_main(){
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
}

void setup() {
  app_main();
  xTaskCreate(vTaskPeriodicCheckLight, "Task 4", 1500, NULL, 10, NULL);

}

void loop() {
  
}
