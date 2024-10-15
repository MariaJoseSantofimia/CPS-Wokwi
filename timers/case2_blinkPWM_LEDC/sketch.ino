#define D13 4


int brightness = 0;
int fadeAmount = 5;


void setup() {
  // attach the channel to the GPIO to be controlled
  ledcAttachChannel(D13, 5000, 8, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWrite(D13, brightness);
  // cambiar el valor de brillo del led:
  brightness = brightness + fadeAmount;
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  delay(60);
}
