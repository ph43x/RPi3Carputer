/*
 This is the new-new Arduino code to replace 'steeringWheelControls.ino'
 It read signals from a 350Z steering wheel control module. 
 So it can then pass a signal to the RPi3 usb port, via serial connection
 The 350Z wheel controls work off of two wires with 3 buttons each
 Wire 1: Short: Mode,  165 ohms: Volume Up,   652 ohms: Seek Up,   Color: Green
 Wire 2: Short: Power, 165 ohms: Volume Down, 652 ohms: Seek Down, Color: Yellow
*/
const int carAcc = 2;
const int signalWire1 = A6; // Well, with testing, I can't use digitalRead on an analog pin
const int signalWire2 = A7; // So I just created the while loop for when the voltage reading is lowish
const int wakeUpPin = 13;   // Pin attached to the power relay for RPi3
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
  pinMode(wakeUpPin, OUTPUT);
}

void buttonPress(int sendVal, int sendWire){
  i = 0;
  while (analogRead(sendWire) < 300){
    i = i + 1;
    if (i >= 20){
      sendVal = sendVal + 10;
      Serial.println(sendVal);
      lastDebounceTime = millis();
      break;
    }
    delay(100);
  }
  if (i < 20){
    Serial.println(sendVal);
    lastDebounceTime = millis();
  }
}

void loop() {
  // This portion identifies that the shutdown signal has not been sent
  // and the ignition is now off. So shutdown signal is sent, 15 second timer, then power off RPi3
  if ((sentShutdown == 0) && (digitalRead(carAcc) == LOW)) {
    Serial.println(100);
    sentShutdown = 1;
    delay(15000);
    digitalWrite(wakeUpPin) = LOW;
    }
  // Ok now we need to be able to send the startup signal when car is on
  // Sense car is on, set the pin to the relay high to let the RPi boot
  // Wait 15 seconds, then send 200 over serial and check if a response is had
  // If not, send 200 again every second until a response is received.
  if ((sentShutdown == 1) && (digitalRead(carAcc) == HIGH)) {
    digitalWrite(wakeUpPin) = HIGH;
    delay(15000);
    Serial.println(200);
    delay(500);
    while (sentShutdown == 1){
      if (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '1'){
          sentShutdown = 0;
        }
      }
      else {
        Serial.flush();
        Serial.println(200);
        delay(1000);
      }
    }
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
        buttonPress(102, signalWire1); // Seek Up
      }

      if ((wire1 < 55) && (wire1 >= 11)) {
        buttonPress(103, signalWire1); // Volume Up
      }

      if ((wire1 < 10) && (wire1 >= 0)) {
        buttonPress(101, signalWire1); // Mode
      }

      if ((wire2 < 300) && (wire2 >= 55)) {
        buttonPress(202, signalWire2); // Seek Down
      }

      if ((wire2 < 55) && (wire2 >= 11)) {
        buttonPress(203, signalWire2); // Volume Down
      }

      if ((wire2 < 10) && (wire2 >= 0)) {
        buttonPress(201, signalWire2); // Power
      }
  // Add the debug prints between here and the next close brace
     
    }
  }
}
