#define LDR_PIN 12

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);

  Serial.println("Hello, ESP32!");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(LDR_PIN) == LOW) {
    Serial.println("Light!");
  } else {
    Serial.println("Dark  ");
  }
  delay(100);
}
