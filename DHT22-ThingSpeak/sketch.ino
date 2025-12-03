#include <WiFi.h>
#include <WiFiClientSecure.h>  // Librería para cliente seguro
#include "ThingSpeak.h"
#include "DHTesp.h"

#define DHT_PIN 15

DHTesp dhtSensor;

const int myChannelNumber = 3191059; // ThingSpeak channel number
const char* myApiKey = "CGDAT0C37L6JWVWW"; // ThingSpeak API key
const char* server = "api.thingspeak.com"; // ThingSpeak server address

const char* ssid = "Wokwi-GUEST";
const char* password =  "";

WiFiClient wifiClient;  // Cliente seguro para SSL

void wifiConnect() {
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
Serial.begin(115200);
  delay(4000);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  wifiConnect();
  ThingSpeak.begin(wifiClient); // Initialize the ThingSpeak library
}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
   
  // Convertir valores de temperatura y humedad a cadenas de caracteres
  char tempString[8];
  char humString[8];
  dtostrf(data.temperature, 1, 2, tempString);
  dtostrf(data.humidity, 1, 2, humString);

  // Mostrar en el Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(tempString);
  Serial.print("Humidity: ");
  Serial.println(humString);

  ThingSpeak.setField(1,data.temperature); // Set the value of field 1 in the ThingSpeak channel to the temperature
  ThingSpeak.setField(2,data.humidity); // Set the value of field 2 in the ThingSpeak channel to the humidity
  
  int x = ThingSpeak.writeFields(myChannelNumber,myApiKey); // Write the data to the ThingSpeak channel 

  if(x == 200){
    Serial.println("Data pushed successfully"); // Print a message if the data was successfully pushed to ThingSpeak
  }else{
    Serial.println("Push error" + String(x)); // Print an error message with the HTTP status code if there was an error pushing the data
  }
  Serial.println("---"); // Print a separator line

  delay(10000); // Delay for 10 seconds
}
