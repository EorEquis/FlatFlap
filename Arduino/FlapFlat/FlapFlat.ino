/*
  What: 
    Motorized ScopeCover - To be used as an ASCOM Switch to open/close a scope cover

  Who:
    Created By:  eorequis@stuffupthere.com

  When:
    Last modified:  2022-02-10

  Current State:
  V2 Branch begun, light control and consistent comms protocol

  Tested with these servos: 
    HiTec HS-635HB - https://hitecrcd.com/products/servos/discontinued-servos-servo-accessories/hs-635hb-karbonite-high-torque-servo/product
    Aero Sport CS-28R - They're so old there is no URL.
      
  And these boards:
    Gikfun Arduino Nano Clone - https://www.amazon.com/gp/product/B00SGMEH7G
    Arduino Uno - https://www.amazon.com/gp/product/B008GRTSV6/
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
}

void loop() {
  handleSerial();
  handleServo();
}
