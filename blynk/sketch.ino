#define BLYNK_TEMPLATE_ID "TMPL5DqIu527J"
#define BLYNK_TEMPLATE_NAME "Test 01"
#define BLYNK_AUTH_TOKEN "eomwV0l0yjIoP_MiCW3Vt4NlMVIrg9l-"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHTesp.h"

#define DHT_PIN 15
#define LED_PIN 4

DHTesp dhtSensor;

char auth[] = BLYNK_AUTH_TOKEN;

const char* ssid = "Wokwi-GUEST";
const char* password =  "";

BlynkTimer timer;

void setup(){
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(4000);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  Blynk.begin(auth, ssid, password);
  timer.setInterval(2500L, sendSensor);

}

BLYNK_WRITE(V1) {
  int value = param.asInt();  // 1 o 0
  digitalWrite(LED_PIN, value);
  Serial.print("LED Value from Blynk: ");
  Serial.println(value);
}

void loop(){
  Blynk.run();
  timer.run();
}
void sendSensor(){
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
 
  Blynk.virtualWrite(V2, data.humidity);
  Blynk.virtualWrite(V0, data.temperature);
  delay(2000); // Wait for a new reading from the sensor (DHT22 has ~0.5Hz sample rate)

}