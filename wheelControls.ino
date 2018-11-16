/*
 This code can probably be written better with the use of functions, but for now...
 This is the new Arduino code to replace 'steeringWheelControls.ino'
 It read signals from a 350Z steering wheel control module. 
 So it can then pass a signal to the RPi3 usb port, via serial connection
 The 350Z wheel controls work off of two wires with 3 buttons each
 Wire 1: Short: Mode,  165 ohms: Volume Up,   652 ohms: Seek Up,   Color: Green
 Wire 2: Short: Power, 165 ohms: Volume Down, 652 ohms: Seek Down, Color: Yellow
*/

const int carAcc = 2;
const int signalWire1 = A6; // Well, with testing, I can't use digitalRead on an analog pin
const int signalWire2 = A7; // So I just created the while loop for when the voltage reading is lowish
float wire1 = 0;
float wire2 = 0;
int sentShutdown = 0;
int testOn = 1;    // Set this value to 1 for testing main loop, 0 otherwise
int i = 0;
long lastDebounceTime = 0;
long debounceDelay = 50;   //time in ms of how long to wait before confirm

void setup() {
  // Set baud rate, wheel buttons, and vehicle Accessory with corresponding pins
  
  Serial.begin(9600);
  pinMode(signalWire1, INPUT_PULLUP);
  pinMode(signalWire2, INPUT_PULLUP);
  pinMode(carAcc, INPUT);
}

void loop() {
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
  // testOn variable is used here to test when outside of the car
  if ((digitalRead(carAcc) == HIGH) && (sentShutdown == 0) || (testOn == 1)) {
  
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
        i = 0;
        while (analogRead(signalWire1) < 300) {
          i = i + 1; // comment out to negate long press
          if (i >= 20){
            Serial.println(113); //volUpLong
            delay(500);
            lastDebounceTime = millis();
            break;
          }
          delay(250);
        }
        if (i < 20){
          Serial.println(103); //volUp
          lastDebounceTime = millis();
        }
      }

      if ((wire1 < 55) && (wire1 >= 11)) {
        i = 0;
        while (analogRead(signalWire1) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(112); //seekUpLong
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i < 20){
          Serial.println(102); //seekUp
          lastDebounceTime = millis();
        }
      }

      if ((wire1 < 10) && (wire1 >= 0)) {
        i = 0;
        while (analogRead(signalWire1) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(111); //modeUpLong
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i < 20){
          Serial.println(101); //modeUp
          lastDebounceTime = millis();
          }
      }

      if ((wire2 < 300) && (wire2 >= 55)) {
        i = 0;
        while (analogRead(signalWire2) < 300) {
          i = i + 1; // comment out to negate long press
          if (i >= 20){
            Serial.println(213); //voldownLong
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i < 20){
          Serial.println(203); //voldown
          lastDebounceTime = millis();
          }
      }

      if ((wire2 < 55) && (wire2 >= 11)) {
        i = 0;
        while (analogRead(signalWire2) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(212); //seekdownLong
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i < 20){
          Serial.println(202); //seekdown
          lastDebounceTime = millis();
          }
      }

      if ((wire2 < 10) && (wire2 >= 0)) {
        i = 0;
        while (analogRead(signalWire2) < 300) {
          i = i + 1;
          if (i >= 20){
            Serial.println(211); //powerLong
            lastDebounceTime = millis();
            break;
          }
          delay(100);
        }
        if (i < 20){
          Serial.println(201); //power
          lastDebounceTime = millis();
          }
      }

  // Comment the following print lines when done debugging
     Serial.print("Debug: W1- ");
     Serial.print(wire1);
     Serial.print(" W2- ");
     Serial.print(wire2);
     Serial.print(" carAcc- ");
     Serial.print(digitalRead(carAcc));
     Serial.print(" sS- ");
     Serial.println(sentShutdown);
    }
  }
}
