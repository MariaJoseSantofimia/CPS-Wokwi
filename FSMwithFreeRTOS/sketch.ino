//FreeRTOS library:
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


// ====== Pines ======
const int PIR_PIN = 14;   // PIR (movimiento) - digital
const int LDR_PIN = 34;   // LDR (salida digital DO) - digital
const int LED_PIN = 13;   // LED de la "luz"

// ====== Temporización ======
const uint32_t INACTIVITY_MS = 7000; // 5 segundos (cambia a 5UL*60UL*1000UL para 5 minutos)


// ====== Estados ======
#define LUZ_APAGADA   0
#define LUZ_ENCENDIDA 1

int estadoActual = LUZ_APAGADA;

// ====== Eventos ======
enum EventType : uint8_t {
  EVT_MOTION,   // Se detectó movimiento (flanco)
  EVT_LUX,      // Cambio en LDR digital: 1=oscuro, 0=claro
  EVT_TIMER     // Expiró inactividad
};

struct Event {
  EventType type;
  int value; // Para EVT_LUX: 1=oscuro, 0=claro. Para otros puede ser 0/1 informativo.
};

// ====== RTOS handles ======
QueueHandle_t xEventQueue = nullptr;
TimerHandle_t xInactivityTimer = nullptr;

// ====== Prototipos de tareas ======
void TareaSensorPIR(void* pv);
void TareaSensorLDR(void* pv);
void TareaControl(void* pv);

// ====== API luz ======
inline void encenderLuz() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Luz encendida");
  estadoActual = LUZ_ENCENDIDA;

}
inline void apagarLuz() {
  digitalWrite(LED_PIN, LOW);
  Serial.println("Luz apagada");
  estadoActual = LUZ_APAGADA;
}

// ====== Callback del temporizador de inactividad ======
void vInactivityTimerCallback(TimerHandle_t) {
  Event ev{EVT_TIMER, 0};
  xQueueSend(xEventQueue, &ev, 0);
}

// ====== Setup ======
void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);  // DO del módulo LDR
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Cola de eventos
  xEventQueue = xQueueCreate(16, sizeof(Event));
  if (!xEventQueue) {
    Serial.println("ERROR: no se pudo crear la cola de eventos.");
    while (true) delay(1000);
  }

  // Temporizador de inactividad (one-shot)
  xInactivityTimer = xTimerCreate("inactivity",
                                  pdMS_TO_TICKS(INACTIVITY_MS),
                                  pdFALSE, // one-shot
                                  nullptr,
                                  vInactivityTimerCallback);
  if (!xInactivityTimer) {
    Serial.println("ERROR: no se pudo crear el temporizador.");
    while (true) delay(1000);
  }

  // Tareas
  xTaskCreate(TareaSensorPIR, "PIR", 2048, nullptr, 2, nullptr);
  xTaskCreate(TareaSensorLDR, "LDR", 2048, nullptr, 2, nullptr);
  xTaskCreate(TareaControl,   "CTL", 3072, nullptr, 3, nullptr);

  Serial.println("Sistema iniciado (FreeRTOS).");
}

// El loop no se usa con FreeRTOS y por eso le mete un delay "infinito"
void loop() {
  vTaskDelay(portMAX_DELAY);
}

// ====== Tarea: Sensor PIR (movimiento) ======
// Lee el PIR y emite EVT_MOTION en flanco ascendente (LOW->HIGH).
// Resetea/arranca el temporizador de inactividad en cada detección.
void TareaSensorPIR(void* pv) {
// Triggering the sensor will drive the OUT pin high for 5 seconds (delay time),
// and then go low again. The sensor will ignore any further input for the next 
// 1.2 seconds (inhibit time), and then start sensing for motion again.

  const TickType_t PIR_PAUSA_LECTURA = pdMS_TO_TICKS(5000); // 5 segundos
  int lastMotion = LOW;  // Estado anterior del PIR
  int motion = digitalRead(PIR_PIN);

  for (;;) {
    
    int motion = digitalRead(PIR_PIN);

    if (motion != lastMotion) {
      lastMotion = motion;
      if (motion == HIGH) {
        // Movimiento detectado
        Event ev{EVT_MOTION, 1};
        xQueueSend(xEventQueue, &ev, 0);
        Serial.println("[PIR] Movimiento detectado");

        // (Re)iniciar temporizador de inactividad
        if (xTimerIsTimerActive(xInactivityTimer)) {
          xTimerReset(xInactivityTimer, 0);
        } else {
          xTimerStart(xInactivityTimer, 0);
        }
        Serial.println("[PIR] (Re)inicio temporizador de inactividad");
      } else {
        // No hay movimiento (LOW)
        Event ev{EVT_MOTION, 0};
        xQueueSend(xEventQueue, &ev, 0);
        Serial.println("[PIR] Sin movimiento");
      }
    }
    vTaskDelay(PIR_PAUSA_LECTURA );
  }
}

// ====== Tarea: Sensor LDR (digital DO) ======
// DO alto = oscuro; DO bajo = claro. Emite EVT_LUX solo cuando hay cambio.
void TareaSensorLDR(void* pv) {
  
  const TickType_t PERIODO_LECTURA = pdMS_TO_TICKS(5000); // 5 segundos

  for (;;) {
    int val = digitalRead(LDR_PIN);  // Lectura actual
    Event ev{EVT_LUX, val};          // Crear evento con valor 1=oscuro, 0=claro
    xQueueSend(xEventQueue, &ev, 0); // Enviar evento sin bloquear
    Serial.printf("[LDR] Lectura cada 5s: DO=%d -> %s\n", val, val ? "oscuro" : "claro");

    vTaskDelay(PERIODO_LECTURA);     // Esperar 5 segundos antes de la siguiente lectura
  }
}
  


// ====== Tarea: Control (FSM) ======
void TareaControl(void* pv) {

  int valorLDR = 0; // 1=oscuro (luz baja), 0=claro (luz suficiente)

  // Estado inicial
  apagarLuz();
  Serial.println("[CTL] Estado inicial: LUZ_APAGADA");


  Event ev;
  for (;;) {
    // Si recibe correctamente un evento
    if (xQueueReceive(xEventQueue, &ev, portMAX_DELAY) == pdTRUE) {
      switch (ev.type) {
        case EVT_LUX: {
          // 1=oscuro (luz baja), 0=claro (luz suficiente)
          valorLDR = ev.value;
          Serial.printf("[CTL] EVT_LUX: %s\n", valorLDR ? "oscuro" : "claro");

          // Si ahora hay luz suficiente (claro) y la luz artificial estaba encendida, apaga
          if (!valorLDR && estadoActual == LUZ_ENCENDIDA) {
            apagarLuz();
            // ya no necesitamos el temporizador si está apagado
            if (xTimerIsTimerActive(xInactivityTimer)) xTimerStop(xInactivityTimer, 0);
          }
        } break;

        case EVT_MOTION: {
          Serial.println("[CTL] EVT_MOTION");
          // Encender solo si la luz ambiental es baja (oscuro)
          if (valorLDR) {
            if (estadoActual != LUZ_ENCENDIDA) {
              encenderLuz();
            }
            // El temporizador se (re)inicia en la tarea del PIR
          } else {
            Serial.println("[CTL] Ambiente claro: no se enciende.");
          }
        } break;

        case EVT_TIMER: {
          Serial.println("[CTL] EVT_TIMER (inactividad)");
          // Si no hubo movimiento durante el periodo, apaga
          if (estadoActual == LUZ_ENCENDIDA) {
            apagarLuz();
          }
          // El siguiente movimiento volverá a arrancar el temporizador
        } break;
      }
    }
  }
}
