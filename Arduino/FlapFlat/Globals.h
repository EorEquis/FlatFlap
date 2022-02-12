
// #defines
  //#define DEBUG   // Uncomment to allow debug printout to serial
  #define CALIBRATOR    // Comment this line to return "NotPresent" to ASCOM for light/calibrator
  
Servo myservo;                // create servo object
int servoPin = 9;             // the pin the data line of the servo is attached to.
int lightPin = 5;             // the pin the EL panel's relay is attached to.
int servodelay = 15;          // ms to wait after a servo move of 1 degree.  Higher numbers slow the opening/closing speed
int currentpos;               // current servo position
int targetpos;                // target servo position
int closedPos = 104;          // change this to achieve a good flat close against the end of the scope tube.
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
