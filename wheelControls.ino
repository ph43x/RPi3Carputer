/*
 This is the new Arduino code to replace 'steeringWheelControls.ino'
 It read signals from a 350Z steering wheel control module. 
 So it can then pass a signal to the RPi3 usb port, via serial connection
*/

int signalWire1 = 19;
int signalWire2 = 22;
float wire1 = 0;
float wire2 = 0;

int buttonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;   //time in ms of how long to wait before confirm

void setup() {
/* 
  Set baud rate and wheel buttons with corresponding pins
  The 350Z wheel controls work off of two wires with 3 buttons each
  Wire 1: Short: Mode,  165 ohms: Volume Up,   652 ohms: Seek Up
  Wire 2: Short: Power, 165 ohms: Volume Down, 652 ohms: Seek Down
*/
  Serial.begin(9600);
  pinMode(signalWire1, INPUT);
  pinMode(signalWire2, INPUT);
}

void loop() {
  /*
  Little known issue when using multiple ADC readings on the same board
  Take the reading twice, due to the multiplexer switching to the other
  circuit creating a false reading, 10ms has been tested to be an acceptable
  delay for the internal capacitors to drain for a clear reading.
  */
  wire1 = analogRead(signalWire1);
  delay(10);
  wire1 = analogRead(signalWire1);
  delay(10);
  wire2 = analogRead(signalWire2);
  delay(10);
  wire2 = analogRead(signalWire2);
  delay(10);
  
 // With the new logic of reading the ADC, unsure if deboucne logic is needed.
 // Will leave in for now and test each scenario
  if ( (millis() - lastDebounceTime) > debounceDelay) {
   
    if (wire1 >= 128) {
      Serial.println(103);
      lastDebounceTime = millis();
    }
    if ((wire1 < 128) && (wire1 > 25)) {
      Serial.println(102);
      lastDebounceTime = millis();
    }
    if ((wire1 < 25) && (wire1 > 0)) {
      Serial.println(101);
      lastDebounceTime = millis();
    }
    if (wire2 >= 128) {
      Serial.println(203);
      lastDebounceTime = millis();
    }
    if ((wire2 < 128) && (wire2 > 25)) {
      Serial.println(202);
      lastDebounceTime = millis();
    }
    if ((wire2 < 25) && (wire2 > 0)) {
      Serial.println(201);
      lastDebounceTime = millis();
    }
// Comment the two next lines when done debugging
    Serial.println(wire1);
    Serial.println(wire2);
  }
// Comment the next line when done debugging
  delay(1000);
}
