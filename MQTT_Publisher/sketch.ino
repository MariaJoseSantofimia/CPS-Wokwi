#include <WiFi.h>
#include <WiFiClientSecure.h>  // Librería para cliente seguro
#include <PubSubClient.h>

#include "DHTesp.h"

#define DHT_PIN 15

DHTesp dhtSensor;

const char* ssid = "Wokwi-GUEST";
const char* password =  "";

#define TOPIC_HUM "/sensor/humidity"
#define TOPIC_TEMP "/sensor/temperature"

// Configuración del servidor MQTT
const char* mqtt_server = "9c5d28e4b50e4761b380271f23d90f8c.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;  // Puerto para conexión segura
const char* mqtt_user = "hivemq.webclient.1730285135170";
const char* mqtt_password = "IS#J6:2EbD8aQ>5o<eqg";

WiFiClientSecure wifiClient;  // Cliente seguro para SSL
PubSubClient client(wifiClient);

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

void mqttConnect() {
  // Configurar la conexión al servidor MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Configurar cliente seguro para evitar verificación de certificado
  wifiClient.setInsecure();

  while (!client.connected()) {
      Serial.print("Conectando a MQTT...");
      if (client.connect("ESP32_Client", mqtt_user, mqtt_password)) {
        Serial.println("Conectado!");
        client.subscribe(TOPIC_TEMP);  // Suscribirse a un tema
        client.subscribe(TOPIC_HUM);  // Suscribirse a un tema
      } else {
        Serial.print("Error de conexión. Estado = ");
        Serial.println(client.state());
        delay(2000);
      }
  }
}

void setup() {
  Serial.begin(115200);
  delay(4000);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  wifiConnect();
  mqttConnect();
}

void loop() {
  if (!client.connected()) {
    mqttConnect();  // Reintentar conexión si se pierde
  }

  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  // Convertir valores de temperatura y humedad a cadenas de caracteres
  char tempString[8];
  char humString[8];
  dtostrf(data.temperature, 1, 2, tempString);
  dtostrf(data.humidity, 1, 2, humString);

  // Publicar los datos como cadenas de caracteres
  client.publish(TOPIC_TEMP, tempString);
  client.publish(TOPIC_HUM, humString);

  // Mostrar en el Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(tempString);
  Serial.print("Humidity: ");
  Serial.println(humString);

  delay(3000);
}

