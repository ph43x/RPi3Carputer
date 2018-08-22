/*
 This is the new Arduino code to replace 'steeringWheelControls.ino'
 It read signals from a 350Z steering wheel control module. 
 So it can then pass a signal to the RPi3 usb port, via serial connection
*/

int signalWire1 = 19;
int signalWire2 = 22;

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
  // I decided to use values that could be sent in one byte and
  // that would indicate which wire sent the signal and what button
  // Also I need to put in some debounce logic to prevent false positives
  int wire1 = map(analogRead(signalWire1), 0, 1023, 0, 255);
  int wire2 = map(analogRead(signalWire2), 0, 1023, 0, 255);
  if (wire1 >= 128) {
    Serial.println(103);
  }
  if ((wire1 < 128) && (wire1 > 25)) {
    Serial.println(102);
  }
  if ((wire1 < 25) && (wire1 > 0)) {
    Serial.println(101);
  }
  if (wire2 >= 128) {
    Serial.println(203);
  }
  if ((wire2 < 128) && (wire2 > 25)) {
    Serial.println(202);
  }
  if ((wire2 < 25) && (wire2 > 0)) {
    Serial.println(201);
  }
  delay(10);
}
