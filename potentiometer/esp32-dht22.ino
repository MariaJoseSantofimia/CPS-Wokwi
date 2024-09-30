/**
   ESP32 + potentiometer Example for Wokwi
   
*/

void setup() {
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(36); // Leer el valor analógico del pin GPIO36 (ADC0)
  Serial.println(value);
  delay(100);
}