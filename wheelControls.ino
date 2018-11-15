/*
 This is the new Arduino code to replace 'steeringWheelControls.ino'
 It read signals from a 350Z steering wheel control module. 
 So it can then pass a signal to the RPi3 usb port, via serial connection
*/

const int carAcc = 2;    // Comment out for testing with car being off 1/4
const int signalWire1 = A6; // Well, with testing, it doesn't seem that I can read digitally from an analog pin
const int signalWire2 = A7;
float wire1 = 0;
float wire2 = 0;
int sentShutdown = 0;
int testOn = 1;    // Set this value to 1 for testing main loop, 0 otherwise
int i = 0;
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
  pinMode(signalWire1, INPUT_PULLUP);
  pinMode(signalWire2, INPUT_PULLUP);
  pinMode(carAcc, INPUT);     // Comment out for testing with car being off 2/4
}

void loop() {
  //*
  // Comment out for testing with car being off --start 3.5/5
  // This portion identifies that the shutdown signal has not been sent
  // and the ignition is now off
  if ((sentShutdown == 0) && (digitalRead(carAcc) == LOW)) {
    Serial.println(100);
    sentShutdown = 1;
    }
  // Ok now we need to be able to send the startup signal when car is on
  if ((sentShutdown == 1) && (digitalRead(carAcc) == HIGH)) {
    Serial.println(200);
    sentShutdown = 0;
  }
 
  // This statement after understands the startup signal has been sent and the car is on
  if ((digitalRead(carAcc) == HIGH) && (sentShutdown == 0) || (testOn == 1)) {
  // Comment out for testing with car being off --end 4/5
  
    /*
      Little known issue when using multiple ADC readings on the same board
      Take the reading twice, due to the multiplexer switching to the other
      circuit creating a false reading, 10ms has been tested to be an acceptable
      delay for the internal capacitors to drain for a clear reading.
    */
    wire1 = analogRead(signalWire1);
    delay(10);
    wire1 = analogRead(signalWire1);
    wire2 = analogRead(signalWire2);
    delay(10);
    wire2 = analogRead(signalWire2);

    if ((millis() - lastDebounceTime) > debounceDelay) {
     
      if ((wire1 < 300) && (wire1 >= 55)) {
        while (analogRead(signalWire1) < 300) {
          i = i + 1; // comment out to negate long press
          if (i >= 20){
            Serial.println(113); //volUpLong
            i = 0;
            delay(500);
            lastDebounceTime = millis();
            break;
          }
          Serial.println(103); //volUp
          delay(250);
        }
        if (i >= 20){
          Serial.println(113); //volUpLong
          i = 0;
        }
        else{
          Serial.println(103); //volUp
          i = 0;
        }
        lastDebounceTime = millis();
      }

      if ((wire1 < 55) && (wire1 >= 11)) {
        while (analogRead(signalWire1) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(112); //seekUpLong
            i = 0;
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i >= 20){
          Serial.println(112); //seekUpLong
          i = 0;
        }
        else{
          Serial.println(102); //seekUp
          i = 0;
        }
        lastDebounceTime = millis();
      }

      if ((wire1 < 10) && (wire1 >= 0)) {
        while (analogRead(signalWire1) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(111); //modeUpLong
            i = 0;
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i >= 20){
          Serial.println(111); //modeUpLong
          i = 0;
          }
        else{
          Serial.println(101); //modeUp
          i = 0;
        }
        lastDebounceTime = millis();
      }

      if ((wire2 < 300) && (wire2 >= 55)) {
        while (analogRead(signalWire2) < 300) {
          i = i + 1; // comment out to negate long press
          if (i >= 20){
            Serial.println(213); //voldownLong
            i = 0;
            lastDebounceTime = millis();
            break;
          }
          Serial.println(203); //voldown
          delay(100);
        }
        if (i >= 20){
          Serial.println(213); //voldownLong
          i = 0;
          }
        else{
          Serial.println(203); //voldown
          i = 0;
        }
        lastDebounceTime = millis();
      }

      if ((wire2 < 55) && (wire2 >= 11)) {
        while (analogRead(signalWire2) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(212); //seekdownLong
            i = 0;
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i >= 20){
          Serial.println(212); //seekdownLong
          i = 0;
          }
        else{
          Serial.println(202); //seekdown
          i = 0;
        }
        lastDebounceTime = millis();
      }

      if ((wire2 < 10) && (wire2 >= 0)) {
        while (analogRead(signalWire2) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(211); //powerLong
            i = 0;
            lastDebounceTime = millis();
            break;
          }
          delay(100);
          digitalRead(signalWire2);
        }
        if (i >= 20){
          Serial.println(211); //powerLong
          i = 0;
          }
        else{
          Serial.println(201); //power
          i = 0;
        }
        lastDebounceTime = millis();
      }

  // Comment the next three lines when done debugging 1/2
     Serial.print("Debug: W1- ");
     Serial.print(wire1);
     Serial.print(" W2- ");
     Serial.print(wire2);
     Serial.print(" carAcc- ");
     Serial.print(digitalRead(carAcc));
     Serial.print(" sS- ");
     Serial.print(sentShutdown);
    }
  // Comment the next line when done debugging 2/2
  //  delay(200);
  } // Comment out for testing with car being off 5/5
}
