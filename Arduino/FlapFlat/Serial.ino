// Shamelessly stolen from Jared Wellman 
// https://github.com/jwellman80/ArduinoLightbox/blob/master/LEDLightBoxAlnitak.ino

void handleSerial()
{
  if( Serial.available() >= 6 )  // all incoming communications are fixed length at 6 bytes including the \n
  {
    char* cmd;
    char* data;
    char temp[10];
    
    int len = 0;

    char str[20];
    memset(str, 0, 20);
    
  // I don't personally like using the \n as a command character for reading.  
  // but that's how the command set is.
    Serial.readBytesUntil('\n', str, 20);

  cmd = str + 1;
  data = str + 2;
  
  // debugging to make sure your commands came through and are parsed correctly.

    #ifdef DEBUGCMD
      sprintf( temp, "cmd = >%c%s;", cmd, data);
      Serial.write(temp);
    #endif
    


    switch( *cmd )
    {
    /*
    Ping device
      Request: >P000\n
      Return : *Pii000\n
        id = deviceId
    */
      case 'P':
      sprintf(temp, "*P%d000", deviceId);
      Serial.write(temp);
      break;

      /*
    Open shutter
      Request: >O000\n
      Return : *Oii000\n
        id = deviceId
      This command is only supported on the Flip-Flat!
    */
      case 'O':
      sprintf(temp, "*O%d000", deviceId);
      SetShutter(OPEN);
      Serial.write(temp);
      break;


      /*
    Close shutter
      Request: >C000\n
      Return : *Cii000\n
        id = deviceId
      This command is only supported on the Flip-Flat!
    */
      case 'C':
      sprintf(temp, "*C%d000", deviceId);
      SetShutter(CLOSED);
      Serial.write(temp);
      break;

    /*
    Turn light on
      Request: >L000\n
      Return : *Lii000\n
        id = deviceId
    */
      case 'L':
      sprintf(temp, "*L%d000", deviceId);
      Serial.write(temp);
      lightStatus = ON;
      analogWrite(ledPin, brightness);
      break;

    /*
    Turn light off
      Request: >D000\n
      Return : *Dii000\n
        id = deviceId
    */
      case 'D':
      sprintf(temp, "*D%d000", deviceId);
      Serial.write(temp);
      lightStatus = OFF;
      analogWrite(ledPin, 0);
      break;

    /*
    Set brightness
      Request: >Bxxx\n
        xxx = brightness value from 000-255
      Return : *Biiyyy\n
        id = deviceId
        yyy = value that brightness was set from 000-255
    */
      case 'B':
      brightness = atoi(data);    
      if( lightStatus == ON ) 
        analogWrite(ledPin, brightness);   
      sprintf( temp, "*B%d%03d", deviceId, brightness );
      Serial.write(temp);
        break;

    /*
    Get brightness
      Request: >J000\n
      Return : *Jiiyyy\n
        id = deviceId
        yyy = current brightness value from 000-255
    */
      case 'J':
        sprintf( temp, "*J%d%03d", deviceId, brightness);
        Serial.write(temp);
        break;
      
    /*
    Get device status:
      Request: >S000\n
      Return : *SidMLC\n
        id = deviceId
        M  = motor status( 0 stopped, 1 running)
        L  = light status( 0 off, 1 on)
        C  = Cover Status( 0 moving, 1 closed, 2 open)
    */
      case 'S': 
        sprintf( temp, "*S%d%d%d%d",deviceId, motorStatus, lightStatus, coverStatus);
        Serial.write(temp);
        break;

    /*
    Get firmware version
      Request: >V000\n
      Return : *Vii001\n
        id = deviceId
    */
      case 'V': // get firmware version
      sprintf(temp, "*V%d010", deviceId);
      Serial.write(temp);
      break;
    }    

  while( Serial.available() > 0 )
    Serial.read();

  }
}
