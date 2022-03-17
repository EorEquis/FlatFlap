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
    2022-03-14  : Happy Pi day.  Force lightPin LOW on setup, to try to troubleshoot strange behaviour for another user.  not a bad idea either way.
    2022-03-15  : Add options for relays that expect LOW t be turned on, add comments/testing results.
    2022-03-16  : Remove hardcoded start position for servo when powered up.  Move user selectable options to their own config file.  Add toggle for servo direction
    2022-03-16  : Clean up these comments, no change to functionality

  Stuff you should do before uploading:
    * Check out Globals.h  There are several #define at the top with decisions you need to make.
    * Make sure you know if your relay expects LOW or HIGH to be turned on, and make the appropriate selection in Globals.h

  Notes:
    Be sure to check configurable options in Config.h
    
*/

#include <Servo.h>
#include "Globals.h"
#include "Config.h"


void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  // Set servo initial position closed, attach servo
  currentpos = closedPos;
  myservo.write(currentpos);
  myservo.attach(servoPin);

  #ifdef CALIBRATOR
    lightStatus = CALIBRATOROFF;
  #endif

  #ifdef RELAYLOW
    relayOn = 0;
    relayOff = 1;
  #endif    

  #ifdef SERVOREVERSE
    servoDirection = -1;
  #endif    

  pinMode(lightPin,OUTPUT);
  digitalWrite(lightPin, relayOff);

}

void loop() {
  handleSerial();
  handleServo();
}
