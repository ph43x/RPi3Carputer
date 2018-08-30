/*
 This is the new Arduino code to replace 'steeringWheelControls.ino'
 It read signals from a 350Z steering wheel control module. 
 So it can then pass a signal to the RPi3 usb port, via serial connection
*/

//int carAcc = 32;    // Comment out for testing with car being off 1/4
int signalWire1 = A6;
int signalWire2 = A7;
int sentShutdown = 0;
float wire1 = 0;
float wire2 = 0;

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
  //pinMode(carAcc, INPUT);     // Comment out for testing with car being off 2/4
}

void loop() {
    /*
    Little known issue when using multiple ADC readings on the same board
    Take the reading twice, due to the multiplexer switching to the other
    circuit creating a false reading, 10ms has been tested to be an acceptable
    delay for the internal capacitors to drain for a clear reading.
    */
  /*
  // Comment out for testing with car being off --start 3.5/5
  // This portion identifies that the shutdown signal has not been sent
  // and the ignition is now off
  if ((sentShutdown = 0) && (digitalRead(carAcc) == LOW)) {
    Serial.println(111);
    sentShutdown = 1;
    }
  // Ok now we need to be able to send the startup signal when car is on
  if ((sentShutdown = 1) && (digitalRead(carAcc) == HIGH)) {
    Serial.println(222);
    sentShutdown = 0;
  }
  
  // This statement after understands the startup signal has been sent and the car is on
  if ((digitalRead(carAcc) == HIGH) && sentShutdown == 0) {
  */ // Comment out for testing with car being off --end 4/5
  
    wire1 = analogRead(signalWire1) * (5.0 / 1023.0);
    delay(10);
    wire1 = analogRead(signalWire1) * (5.0 / 1023.0);
    delay(10);
    wire2 = analogRead(signalWire2) * (5.0 / 1023.0);
    delay(10);
    wire2 = analogRead(signalWire2) * (5.0 / 1023.0);
    delay(10);
    
    // With the new logic of reading the ADC, unsure if deboucne logic is needed.
    // Will leave in for now and test each scenario


    if ((millis() - lastDebounceTime) > debounceDelay) {
     
      if ((wire1 < 800) && (wire1 >= 128)) {
        Serial.println(103);
        lastDebounceTime = millis();
      }
      if ((wire1 < 128) && (wire1 >= 25)) {
        Serial.println(102);
        lastDebounceTime = millis();
      }
      if ((wire1 < 25) && (wire1 >= 0)) {
        Serial.println(101);
        lastDebounceTime = millis();
      }
      if ((wire2 < 800) && (wire2 >= 128)) {
        Serial.println(203);
        lastDebounceTime = millis();
      }
      if ((wire2 < 128) && (wire2 >= 25)) {
        Serial.println(202);
        lastDebounceTime = millis();
      }
      if ((wire2 < 25) && (wire2 >= 0)) {
        Serial.println(201);
        lastDebounceTime = millis();
      }
  // Comment the next three lines when done debugging 1/2
      Serial.print(wire1);
      Serial.print(" ");
      Serial.println(wire2);
    }
  // Comment the next line when done debugging 2/2
    delay(500);
  //} // Comment out for testing with car being off 5/5
}
