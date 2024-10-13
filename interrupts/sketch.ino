#include <Arduino.h>

#define onSwitch 21
#define offSwitch 14
#define LED 4

volatile bool ledOn = false;
volatile bool ledChangeFlag = false;

void IRAM_ATTR on_handleInterrupt() {
  if (!ledOn) {
     ledOn = true;
     ledChangeFlag = true;
  }
}

void IRAM_ATTR off_handleInterrupt() {
  if (ledOn) {
    ledOn = false;
    ledChangeFlag = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  // Usamos INPUT sin resistencias internas
  pinMode(onSwitch, INPUT);  // Resistencia pull-down externa
  attachInterrupt(digitalPinToInterrupt(onSwitch), on_handleInterrupt, RISING);  // Interrupción en flanco de subida

  pinMode(offSwitch, INPUT);  // Resistencia pull-down externa
  attachInterrupt(digitalPinToInterrupt(offSwitch), off_handleInterrupt, RISING);  // Interrupción en flanco de subida
}

void loop() {
  if (ledChangeFlag) {
    if (ledOn) {
      digitalWrite(LED, HIGH);
      Serial.println("LED Encendido");
    } else {
      digitalWrite(LED, LOW);
      Serial.println("LED Apagado");
    }
    ledChangeFlag = false;
  }
  delay(100);
}
