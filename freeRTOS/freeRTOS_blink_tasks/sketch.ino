//FreeRTOS library:
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define RED_LED_PIN 4 // Define el pin donde está conectado el LED
#define GREEN_LED_PIN 14 // Define el pin donde está conectado el LED
#define BLUE_LED_PIN 12 // Define el pin donde está conectado el LED

void vTask1(void* pvParam){
  for(;;){
    int val = digitalRead(RED_LED_PIN);
    val = (~val & 0x00000001);
    digitalWrite(RED_LED_PIN, val);    
    delay(2000);
  }
  vTaskDelete(NULL);
}

void vTask2(void* pvParam){
  for(;;){
    int val = digitalRead(GREEN_LED_PIN);
    val = (~val & 0x00000001);
    digitalWrite(GREEN_LED_PIN, val);    
    delay(1000);
  }
  vTaskDelete(NULL);
}

void vTask3(void* pvParam){
  for(;;){
    int val = digitalRead(BLUE_LED_PIN);
    val = (~val & 0x00000001);
    digitalWrite(BLUE_LED_PIN, val);    
    delay(1500);
  }
  vTaskDelete(NULL);
}

void app_main(){
  Serial.begin(115200);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  xTaskCreatePinnedToCore(vTask1, "Task 1", 1500, NULL, 5, NULL,0);
  xTaskCreatePinnedToCore(vTask2, "Task 2", 1500, NULL, 1, NULL,0);
  xTaskCreatePinnedToCore(vTask3, "Task 3", 1500, NULL, 10, NULL,0);

}

void setup() {
  app_main();
}

void loop() {
}
