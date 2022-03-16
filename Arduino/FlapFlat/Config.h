// #define DEBUG              // Uncomment to allow debug printout to serial

// #define SERVOREVERSE       // Uncomment this to reverse the direction the servo moves, in case you have some fancy left handed Lithuanian servo

// #define RELAYLOW           // Uncomment this line if your relay turns on with the signal pin driven LOW

#define CALIBRATOR            // Comment this line to return "NotPresent" to ASCOM for light/calibrator

int closedPos = 104;          // Change this to achieve a good flat close against the end of the scope tube.

int openPos = 0;              // The servo position for the flap to be open.
                              // USUALLY this is 0, and a move to close (based on the printed servo mount and arrangement)
                              // is positive, so you start at 0, mount the to the servo at a position that represents "Open"
                              // and tweak closedPos for a good fit.  However, if your servo runs the other way
                              // or you've engineered your own mount, you may want to uncomment SERVOREVERSE above, and
                              // flip this logic.

int servoPin = 5;             // The pin the data line of the servo is attached to.

int lightPin = 9;             // The pin the EL panel's relay is attached to.

int servodelay = 15;          // ms to wait after a servo move of 1 degree.  Higher numbers slow the opening/closing speed                                
