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
    pinMode(elPin, OUTPUT);
    analogWrite(elPin, 0);           
    
  // attach servo to servo pin
    currentpos = EEPROM.read(paddr);      // Last stored position is our current position
    coverStatus = EEPROM.read(saddr);     // Last stored cover status is current status
    #ifdef DEBUG
      Serial.print("Current Position : ");
      Serial.println(currentpos);
      Serial.print("Current Cover Status : ");
      Serial.println(coverStatus);
    #endif
    flapservo.write(currentpos);  // Tell servo where it was last.  Prevents automatic initialization to 93 degrees.
    flapservo.attach(SERVOPIN);   // Initialize the servo
} 

 
 
void loop() 
{ 
  handleSerial();
}

    
