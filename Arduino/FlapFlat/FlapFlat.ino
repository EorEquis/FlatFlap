/*
  What: 
    FlapFlat : A motorized scope cover and flat panel in one

  Who:
    Created By:  eorequis@stuffupthere.com

  History:
    2022-02-09  : Initial v 0.1, simple "open"/"clos" string commands, scope cover only
    2022-02-10  : V2 comms protocol, char handling
    2022-02-10  : Addition of light control, debugging/improving V2 protocol, mirroring ASCOm CoverCalibrator enums for cover and light, debug char handling
    2022-02-12  : Return "NotPresent" value for calibrator if light is not present, based on #define set by user before upload

  Tested with these servos: 
    HiTec HS-635HB - https://hitecrcd.com/products/servos/discontinued-servos-servo-accessories/hs-635hb-karbonite-high-torque-servo/product
    Aero Sport CS-28R - They're so old there is no URL.
      
  And these boards:
    Gikfun Arduino Nano Clone - https://www.amazon.com/gp/product/B00SGMEH7G
    Arduino Uno - https://www.amazon.com/gp/product/B008GRTSV6/

  And this EL panel/inverter:
    Adafruit 10cm x 10cm EL Panel - https://www.adafruit.com/product/625
    Adafruit 12V EL Inverter - https://www.adafruit.com/product/448

  Notes:
    Besure to #define (or comment out) CALIBRATOR in Globals.h 
    
*/

#include <Servo.h>
#include "Globals.h"


void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  // Set servo initial position closed, attach servo
  currentpos = 100;
  myservo.write(currentpos);
  myservo.attach(servoPin);
  pinMode(lightPin,OUTPUT);
  #ifdef CALIBRATOR
    lightStatus = CALIBRATOROFF;
  #endif
    
}

void loop() {
  handleSerial();
  handleServo();
}
