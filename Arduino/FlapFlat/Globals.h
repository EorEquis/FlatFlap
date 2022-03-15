
// #defines
  //#define DEBUG       // Uncomment to allow debug printout to serial
  #define CALIBRATOR    // Comment this line to return "NotPresent" to ASCOM for light/calibrator
  #define RELAYHIGH   // Comment this line if your relay turns on with the signal pin driven LOW
  
  
Servo myservo;                // create servo object
int servoPin = 5;             // the pin the data line of the servo is attached to.
int lightPin = 9;             // the pin the EL panel's relay is attached to.
int servodelay = 15;          // ms to wait after a servo move of 1 degree.  Higher numbers slow the opening/closing speed
int currentpos;               // current servo position
int targetpos;                // target servo position
int closedPos = 104;          // change this to achieve a good flat close against the end of the scope tube.
int openPos = 0               // Generally this should not be changed, but maybe your mounting style is different.
                              // The idea here is to start with a known physical location : Servo position 0
                              // Attach cover at that point, in a condition that would be "open" for you...then swing to 
                              // closedPos of your choice, that makes a nice fit.
int relayOn;                  // Holds 1 or 0 based on RELAYHIGH being defined or not.
int relayOff;                 // Holds 1 or 0 based on RELAYHIGH being defined or not.
String strCmd;                // holds the command sent via serial
char response[50];

    
enum coverRequests {
  COVCLOSE,
  COVOPEN
};

enum coverStatuses {        // Mirror ASCOM CoverStatus Enum
  COVERNOTPRESENT = 0,
  COVERCLOSED,
  COVERMOVING,
  COVEROPEN,
  COVERUNKNOWN,
  COVERERROR
};

enum lightRequests {
  LIGHTOFF,
  LIGHTON
};

enum lightStatuses {        // Mirror ASCOM CalibratorStatus Enum
  CALUBRATORNOTPRESENT = 0,
  CALIBRATOROFF,
  CALIBRATORNOTREADY,
  CALIBRATORREADY,    // On
  CALIBRATORUNKNOWN,
  CALIBRATORERROR
};

enum motorDirection {
  CLOSING,
  OPENING,
  NONE
};

enum errorStatuses {
  NORMAL,
  ISERROR
};


int coverStatus = COVERCLOSED;
int lightStatus = CALUBRATORNOTPRESENT;
int motorDirection = NONE;
int errorStatus = NORMAL;
int coverReq = COVCLOSE;
int lightReq = LIGHTOFF;
