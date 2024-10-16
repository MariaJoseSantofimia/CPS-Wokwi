const int buzzerPin = 26;  // Pin donde está conectado el buzzer
const int frequency = 440;  // Frecuencia del tono (en Hz)

void setup() {
  // Configura PWM para el pin del buzzer
  ledcAttachChannel(buzzerPin, frequency, 8, 0);
}

void loop() {
  // Genera una onda cuadrada con PWM
  ledcWrite(buzzerPin, 127);  // Genera un ciclo de trabajo al 50% para onda cuadrada
  delay(1000);  // El sonido suena por 1 segundo
  ledcWrite(buzzerPin, 0);  // Apaga el sonido
  delay(1000);  // Espera 1 segundo antes de repetir
}
