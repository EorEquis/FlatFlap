/*************************************************************************
 * TriStar Observatory ASCOM FlatFlap Firmware
 * 2021MAR13 - EOR : EorEquis@tristarobservatory.space
 * v 0.1.0
 * 
 * 
 * Open and close the flap is all we do now.  It's basically an automated scope cap.
 * Eventually we'll control an EL panel as well, for flats.
 * 
 *  Version     Date        By    Comment
 *  0.1.0       2021MAR13   EOR   Initial build
 **************************************************************************/

#include <Servo.h> 
#include <EEPROM.h>
#include "Globals.h" 


void setup() 
{ 

  // Serial line
    Serial.begin(9600);

  // Set light pin mode
    pinMode(ledPin, OUTPUT);
    analogWrite(ledPin, 0);           
    
  // attach servo to servo pin
    flapservo.write(closedpos);    // Going to change this to set initial position to last stored position, which will be in EEPROM
    flapservo.attach(SERVOPIN);   // Initialize the servo
    currentpos = closedpos;
    coverStatus = CLOSED;

} 

 
 
void loop() 
{ 
  handleSerial();
}

    
