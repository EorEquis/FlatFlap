void handleServo()
{
  if ((servoDirection * currentpos) < (servoDirection * targetpos) && motorDirection == CLOSING) 
    {
      while ((servoDirection * currentpos) < (servoDirection * targetpos)) 
        {
          myservo.write(currentpos + servoDirection);
          delay(servodelay);
          currentpos = currentpos + servoDirection;
        }
      motorDirection = NONE;
      coverStatus = COVERCLOSED;
    }
  else if (currentpos > (servoDirection * targetpos) && motorDirection == OPENING) 
    {
      while ((servoDirection * currentpos) > (servoDirection * targetpos))
        {
          myservo.write(currentpos - servoDirection);
          delay(servodelay);
          currentpos = currentpos - servoDirection;
        }
      motorDirection = NONE;
      coverStatus = COVEROPEN;
    }
}

void handleSerial() {

  if ( Serial.available() >= 5 ) { 
    char* cmd;
    char* data;


    // int len = 0;

    char str[20];
    memset(str, 0, 20);
    Serial.readBytesUntil('#', str, 20);

    cmd = str;
    data = str + 1;

    // useful for debugging to make sure your commands came through and are parsed correctly.
    #ifdef DEBUG
      sprintf(response, "cmd = %s %s %c \r\n", cmd, data, *cmd);
      Serial.write(response);
    #endif

    switch(*cmd)
    {
      // Ping (P) : Ensure device is connected and what we think it is
      // Input : P000#
      // Output : P999*
      case 'P':
        sprintf(response, "P999*");
        Serial.write(response);
        break;

      // Cover (C) : Open or close the flap
      // Input : C00s#  s = coverStatus 
      // Output : C0sr* s = motordirection, r = result (0 success, 1 error)
      case 'C':
        coverReq = atoi(data);
          if (coverReq == COVEROPEN && coverStatus == COVERCLOSED)
            {
              coverStatus = COVERMOVING;
              setShutter(COVEROPEN); 
              motorDirection = OPENING;
              errorStatus = NORMAL;
            }
          else if (coverReq == COVERCLOSED && coverStatus == COVEROPEN)
            {
              coverStatus = COVERMOVING;
              setShutter(COVERCLOSED);
              motorDirection = CLOSING;
              errorStatus = NORMAL;
            }
          else if (coverReq == COVERUNKNOWN && coverStatus == COVERMOVING)
            {
              coverStatus = COVERUNKNOWN;
              errorStatus = ISERROR;
            }
          else
            {
              // This request is in error.
              motorDirection = NONE;
              errorStatus = ISERROR;

            }

        sprintf(response, "C0%i%i*", coverStatus, errorStatus);
        Serial.write(response);              
        break;


      // Light (L) : Set the light on or off
      // We don't do brightness.
      // Input : L00s#    (s = lightStatus)
      // Output : L0sr* (s is lightStatus, r is 0 (success) or 1 (error)
      case 'L':
        #ifdef CALIBRATOR
          
          lightReq = atoi(data);
            if (lightReq == CALIBRATORREADY && lightStatus == CALIBRATOROFF)
              {
                setLight(CALIBRATORREADY);
                lightStatus = CALIBRATORREADY;
                errorStatus = NORMAL;
              }
  
            else if (lightReq == CALIBRATOROFF && lightStatus == CALIBRATORREADY)
              {
                setLight(CALIBRATOROFF);
                lightStatus = CALIBRATOROFF;
                errorStatus = NORMAL;
              }
            else
              {
                errorStatus = ISERROR;
              }
          sprintf(response, "L0%i%i*", lightStatus, errorStatus);
          Serial.write(response);
          break;
        #endif
        
      // Status (S) : Query the device's status
      // Input : S000#
      // Output : S0lc* : l (Letter l, not number 1) = light status, s = cover status
      case 'S':
        sprintf(response, "S0%i%i*", lightStatus, coverStatus);
        Serial.write(response);
        break;

            
    }     //switch(*cmd)

    while ( Serial.available() > 0 ) {
      Serial.read();
    }
  }  // if Serial.available
}   // handleSerial()
