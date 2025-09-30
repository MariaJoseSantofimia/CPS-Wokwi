// Definir pines
const int PIR_PIN = 14;  // Pin del sensor PIR (movimiento)
const int LDR_PIN = 34;  // Pin de la salida digital del sensor de luz ambiental (LDR)
const int LED_PIN = 13;  // Pin del LED que simula la luz

// NotE from the LDR help page
// The digital output ("DO") pin goes high when it's dark, and low when there's light. 


// Definir estados usando #define
#define LUZ_APAGADA 0
#define LUZ_ENCENDIDA 1

// Variables para representar los sensores y estados
int estadoActual = LUZ_APAGADA;
int valorPIR = 0;               // Almacena el valor del sensor PIR (movimiento)
int valorLDR = 0;               // Almacena el valor digital del sensor LDR (luz ambiental)
unsigned long tiempoUltimoMov;  // Almacena el tiempo de la última detección de movimiento
const unsigned long tiempoInactividad = 5000;  // 5 segundos en milisegundos (5000 ms)

// Funciones para controlar el sistema
void encenderLuz() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Luz encendida");
}

void apagarLuz() {
  digitalWrite(LED_PIN, LOW);
  Serial.println("Luz apagada");
}

void setup() {
  // Configuración de pines
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);  // Configura el pin de la salida digital del LDR
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  encenderLuz();  // Inicialmente enciende la luz
}

void loop() {
  // Leer el valor del sensor PIR
  valorPIR = digitalRead(PIR_PIN);
  
  // Leer el valor digital del sensor LDR
  valorLDR = digitalRead(LDR_PIN); // HIGH (ALTO) cuando la luz ambiental es baja.
                                   // LOW (BAJO) cuando la luz ambiental es suficiente.
  
  // Obtener el tiempo actual
  unsigned long tiempoActual = millis();
  
  // Verificar el estado actual del sistema
  switch (estadoActual) {
    case LUZ_APAGADA:
      if (valorPIR == HIGH && valorLDR == HIGH) {  // HIGH significa que la luz ambiental es baja
        estadoActual = LUZ_ENCENDIDA;
        encenderLuz();
        tiempoUltimoMov = millis();  // Reiniciar el temporizador
      }
      break;
      
    case LUZ_ENCENDIDA:
      // Si hay movimiento, reiniciar el temporizador
      if (valorPIR == HIGH) {
        tiempoUltimoMov = millis();
      }

      // Apagar la luz si no hay movimiento durante 5 minutos o la luz ambiental es suficiente
      if (millis() - tiempoUltimoMov > tiempoInactividad || valorLDR == LOW) {
        estadoActual = LUZ_APAGADA;
        apagarLuz();
      }
      break;
  }
  
  // Pequeña pausa para evitar lecturas continuas muy rápidas
  delay(100);
}
