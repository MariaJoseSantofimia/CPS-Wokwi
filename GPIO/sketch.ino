#define onSwitch 15
#define offSwitch 21

void IRAM_ATTR on_handleInterrupt(){
  digitalWrite(25, 1);
  digitalWrite(12, 1);
}


void IRAM_ATTR off_handleInterrupt(){
  digitalWrite(25, 0);
  digitalWrite(12, 0);  
}


void setup() {
  // put your setup code here, to run once:
  
  pinMode(25, OUTPUT);
  pinMode(12, OUTPUT);
  
  pinMode(onSwitch, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(onSwitch), &on_handleInterrupt, FALLING);


  pinMode(offSwitch, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(offSwitch), &off_handleInterrupt, FALLING); 
}



void loop() {
  // put your main code here, to run repeatedly:
}
