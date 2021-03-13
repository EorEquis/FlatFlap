// #define DEBUG;

// Setup enums
// Shamelessly stolen from Jared Wellman 
// https://github.com/jwellman80/ArduinoLightbox/blob/master/LEDLightBoxAlnitak.ino

enum devices
{
  FLAT_MAN_L = 10,
  FLAT_MAN_XL = 15,
  FLAT_MAN = 19,
  FLIP_FLAT = 99
};

enum lightStatuses
{
  OFF = 0,
  ON
};

enum motorStatuses
{
  STOPPED = 0,
  RUNNING
};

enum shutterStatuses
{
  UNKNOWN = 0, // ie not open or closed...could be moving
  CLOSED = 1,
  OPEN = 2
};



// Other variables
  String strCmd;
  volatile int ledPin = 11;      // the pin that the LED is attached to, needs to be a PWM pin.

// FlapFlat variables
  int brightness = 0;
  int deviceId = FLIP_FLAT;
  int lightStatus = OFF;
  int coverStatus = UNKNOWN;
  int motorStatus = STOPPED;
  

// Servo variables
  Servo flapservo;
  const int SERVOPIN = 9;
  int closedpos = 102;  // Adjust for fit
  int openpos = 5;      // Adjust for clearance  
  int currentpos;       // Current position  
  int pos;              // Position used for movement loops
  int paddr = 0;         // EEPROM address for storing current servo position
  int saddr = 1;         // EEPROM address for storing current shutter status

// Command set documentation
  /*******************************************************
    Alnitak command set, documented at https://www.optecinc.com/astronomy/catalog/alnitak/resources/Alnitak_GenericCommandsR4.pdf
    Command     Send        Receive       Description
    Ping        >POOOCR     *PiiOOOLF     Used to find device
    Open        >OOOOCR     *OiiOOOLF     Open cover (FF only)
    Close       >COOOCR     *CiiOOOLF     Close cover(FF only)
    Light on    >LOOOCR     *LiiOOOLF     Turn on light
    Light off   >DOOOCR     *DiiOOOLF     Turn off light
    Brightness  >BxxxCR     *BiixxxLF     Set brightness (xxx = 000-255)
    Brightness  >JOOOCR     *JiixxxLF     Get brightness from device
    State       >SOOOCR     *SiiqrsLF     Get device status
    Version     >VOOOCR     *ViivvvLF     Get firmware version

    Where:
      > is the leading character sent to the device
      CR is carriage return, LF is the line feed c haracter
      xxx is a three digit integer ranging from 0-255
      * is the leading character returned from the device

    ii is a two digit product id,
      10 = Flat-Man_XL
      15 = Flat-Man_L
      19 = Flat-Man
      98 = Flip-Mask/Remote Dust Cover
      99 = Flip-Flat.

    qrs is device status where:
      q = 0 motor stopped
      q = 1 motor running
      r = 0 light off
      r = 1 light on
      s = 0 cover not open/closed
      s = 1 cover closed
      s = 2 cover open
      s = 3 timed out (open/closed not detected)

    vvv is firmware version     
  
  *******************************************************/
