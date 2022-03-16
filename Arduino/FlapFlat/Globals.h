// You should not change anything here.  The enums reflect the ASCOM enums for device statuses and such.
  
Servo myservo;                // create servo object

int currentpos;               // current servo position
int targetpos;                // target servo position
int relayOn = 1;              // Holds 1 or 0 based on RELAYHIGH being defined or not.  DO NOT CHANGE THIS HERE. See config
int relayOff = 0;             // Holds 1 or 0 based on RELAYHIGH being defined or not.  DO NOT CHANGE THIS HERE. See config
int servoDirection = 1;       // Used to change direction of the servo.  DO NOT CHANGE THIS HERE. See config
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
