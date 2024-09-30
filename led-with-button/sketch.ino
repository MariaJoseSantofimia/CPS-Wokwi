
#define LED_PIN 4 // Define el pin donde está conectado el LED
#define BUTTON_PIN 5 // Define the pin where the button is connected

int buttonState= LOW;
int ledState = LOW;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

}

void loop() {
  //Read the value of pin 5
  int value = digitalRead(BUTTON_PIN);
  if(value != buttonState)
  {
    if(value == HIGH)
    {
      Serial.println("The button is pressed.");
      if(ledState== LOW){
        digitalWrite(LED_PIN, HIGH);   // set the LED to the opposite state
        ledState=HIGH;
        Serial.println("The led is ON.");
      }
      else{
        digitalWrite(LED_PIN, LOW);
        ledState = LOW;
        Serial.println("The led is OFF.");
      }
    }
    else
    {
      Serial.println("The button is released.");
    }
    // Remember the value for the next time.
    buttonState = value;
  }

  // Slow down the sketch.
  // Also for debouncing the button.
  delay(100);
}
