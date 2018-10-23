/*
 This is the new Arduino code to replace 'steeringWheelControls.ino'
 It read signals from a 350Z steering wheel control module. 
 So it can then pass a signal to the RPi3 usb port, via serial connection
*/

int carAcc = 2;    // Comment out for testing with car being off 1/4
int activeWire1 = 12;
int signalWire1 = A6;
int activeWire2 = 13;
int signalWire2 = A7;
float wire1 = 0;
float wire2 = 0;
int sentShutdown = 0;
int longPowerPressed = 0;
int i = 0;
int tempResume = 0;
long tempResumeExpire = 0;
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
  pinMode(activeWire1, INPUT);
  pinMode(signalWire2, INPUT);
  pinMode(activeWire2, INPUT);
  pinMode(carAcc, INPUT);     // Comment out for testing with car being off 2/4
}

void loop() {
  /* 
    Here lets be able to identify if the power is held down for 2 seconds then
    if the car is currently in a sleeping state, lets resume it, and sleep after 10 min
    just in case.
  */

  if ((sentShutdown == 1) && (digitalRead(carAcc) == LOW) && (longPowerPressed == 1)) {
    // Set the number in milli seconds for how long to wait before shutting down
    tempResumeExpire = (millis() + 600000);  // 10 minutes
    Serial.println(200); // Wake up for a short time
    longPowerPressed = 0;
    tempResume = 1;
    }
  if ((sentShutdown == 1) && (digitalRead(carAcc) == LOW) && (longPowerPressed == 1) && (tempResume == 1)) {
    Serial.println(100); // Go back asleep
    longPowerPressed = 0;
    tempResume = 0;
    }
  if ((tempResumeExpire <= millis()) && (tempResume == 1)){
    Serial.println(100); // 10 minutes have passed sleep again
    tempResume = 0;
  }

  /*
    I realize that longPowerPressed would never get set if it's behind the main if
    So i created this little thing to just check if the power is held down for 2 sec
  */
  if ((sentShutdown == 1) && (digitalRead(carAcc) == LOW) && (tempResume == 0)){
    wire2 = analogRead(signalWire2);
    delay(10);
    wire2 = analogRead(signalWire2);
  
    if ((wire2 < 10) && (wire2 >= 0)) {
      while (digitalRead(activeWire2) == LOW) {
        i = i + 1;
        if (i >= 20){
          Serial.println(211); //powerLong
          i = 0;
          longPowerPressed = 1;
          lastDebounceTime = millis();
          break;
        }
        delay(100);
      }
      i = 0;
    }
  }
  //*
  // Comment out for testing with car being off --start 3.5/5
  // This portion identifies that the shutdown signal has not been sent
  // and the ignition is now off
  if ((sentShutdown == 0) && (digitalRead(carAcc) == LOW)) {
    Serial.println(100);
    sentShutdown = 1;
    tempResume = 0;
    }
  // Ok now we need to be able to send the startup signal when car is on
  if ((sentShutdown == 1) && (digitalRead(carAcc) == HIGH)) {
    Serial.println(200);
    sentShutdown = 0;
    tempResume = 0;
  }
  /*
    This statement after understands the startup signal has been sent and the car is on
    Or if the car is in tempResume mode
  */
  if ((digitalRead(carAcc) == HIGH) && (sentShutdown == 0) || (tempResume == 1)) {
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
        while (digitalRead(activeWire1) == LOW) {
          //i = i + 1; // comment out to negate long press
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
        /* // Comment out, if you have use for long Volume press
        if (i >= 20){
          Serial.println(113); //volUpLong
          i = 0;
        }
        else{
          Serial.println(103); //volUp
          i = 0;
        }
        */
        lastDebounceTime = millis();
      }

      if ((wire1 < 55) && (wire1 >= 11)) {
        while (digitalRead(activeWire1) == LOW) {
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
        while (digitalRead(activeWire1) == LOW) {
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
          Serial.println(101); //modekUp
          i = 0;
        }
        lastDebounceTime = millis();
      }

      if ((wire2 < 300) && (wire2 >= 55)) {
        while (digitalRead(activeWire2) == LOW) {
          //i = i + 1; // comment out to negate long press
          if (i >= 20){
            Serial.println(213); //voldownLong
            i = 0;
            lastDebounceTime = millis();
            break;
          }
          Serial.println(203); //voldown
          delay(250);
        }
        /* // Comment out, if you have use for long Volume press
        if (i >= 20){
          Serial.println(213); //voldownLong
          i = 0;
          }
        else{
          Serial.println(203); //voldown
          i = 0;
        }
        */
        lastDebounceTime = millis();
      }

      if ((wire2 < 55) && (wire2 >= 11)) {
        while (digitalRead(activeWire2) == LOW) {
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
        while (digitalRead(activeWire2) == LOW) {
          i = i + 1;
          if (i >= 20){
            Serial.println(211); //powerLong
            i = 0;
            longPowerPressed = 1;
            lastDebounceTime = millis();
            break;
          }
          delay(100);
          digitalRead(activeWire2);
        }
        if (i >= 20){
          Serial.println(211); //powerLong
          longPowerPressed = 1;
          i = 0;
          }
        else{
          Serial.println(201); //power
          i = 0;
        }
        lastDebounceTime = millis();
      }

  // Comment the next three lines when done debugging 1/2
  //  Serial.print(wire1);
  //  Serial.print(" ");
  //  Serial.println(wire2);
    }
  // Comment the next line when done debugging 2/2
  //  delay(200);
  } // Comment out for testing with car being off 5/5
}
