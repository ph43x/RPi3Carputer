/*
 This is some Arduino code to read signals from a 350Z steering
 wheel control module. So it can then pass a signal to the
 Raspberry Pi 3 GPIO pins, taking place of the factory head unit
*/
int volDown = 23;
int seekDown = 24;
int buttonPower = 25;
int volUp = 26;
int seekUp = 27;
int buttonMode = 28;

int signalWire1 = 19;
int signalWire2 = 22;

void setup() {
  // Setup the buttons with corresponding pins
  Serial.begin(9600);
  pinMode(signalWire1, INPUT);
  pinMode(signalWire2, INPUT);
  pinMode(volDown, OUTPUT);
  pinMode(volUp, OUTPUT);
  pinMode(seekDown, OUTPUT);
  pinMode(seekUp, OUTPUT);
  pinMode(buttonPower, OUTPUT);
  pinMode(buttonMode, OUTPUT);
}

void loop() {
  // Now this is the running portion of the program
  int wheelWire1 = analogRead(signalWire1);
  int wheelWire2 = analogRead(signalWire2);
  if(wheelWire1 > 900){
    digitalWrite(buttonMode, HIGH);
  }
  if((wheelWire1 > 600) && (wheelWire1 < 900)){
    digitalWrite(seekUp, HIGH);
  }
  if((wheelWire1 > 200) && (wheelWire1 < 600)){
    digitalWrite(volUp, HIGH);
  }
  else{
    digitalWrite(volUp, LOW);
    digitalWrite(seekUp, LOW);
    digitalWrite(buttonMode, LOW);
  }
  
  if(wheelWire2 > 900){
    digitalWrite(buttonPower, HIGH);
  }
  if((wheelWire2 > 600) && (wheelWire2 < 900)){
    digitalWrite(seekDown, HIGH);
  }
  if((wheelWire2 > 200) && (wheelWire2 < 600)){
    digitalWrite(volDown, HIGH);
  }
  else{
    digitalWrite(volDown, LOW);
    digitalWrite(seekDown, LOW);
    digitalWrite(buttonPower, LOW);
  }
  Serial.println("Wire 1: " + wheelWire1);
  Serial.println("Wire 2: " + wheelWire2);
  delay(100);
}
