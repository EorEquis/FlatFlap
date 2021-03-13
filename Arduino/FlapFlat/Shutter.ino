void SetShutter(int val)
{
  if( val == OPEN && coverStatus != OPEN && motorStatus != RUNNING)
  {
    motorStatus = RUNNING;                                    // So the driver knows the motor is running
    coverStatus = UNKNOWN;                                    // We don't know if the shutter is open or closed anywhere in between
    EEPROM.write(saddr, coverStatus);                         // Store the unknown status in case things die during movement
    for(currentpos; currentpos >= openpos; currentpos -= 1)   // Open the flap 
    {                                  
      flapservo.write(currentpos);            
      EEPROM.write(paddr, currentpos);                        // Store the current position in case things die during movement
      delay(15);                       
    } 
    coverStatus = OPEN;                                       // Cover is now open
    motorStatus = STOPPED;                                    // Motor is stopped
    EEPROM.write(saddr, coverStatus);                         // Store the status so next time we know we left it open
  }
  else if( val == CLOSED && coverStatus != CLOSED && motorStatus != RUNNING )
  {
    motorStatus = RUNNING;                                    // So the driver knows the motor is running
    coverStatus = UNKNOWN;                                    // We don't know if the shutter is open or closed anywhere in between
    EEPROM.write(saddr, coverStatus);                         // Store the unknown status in case things die during movement
    for(currentpos; currentpos <= closedpos; currentpos += 1) // Close the flap
    {                                  
      flapservo.write(currentpos);            
      EEPROM.write(paddr, currentpos);                        // Store the current position in case things die during movement
      delay(15);                       
    } 
    coverStatus = CLOSED;                                     // Cover is now closed
    motorStatus = STOPPED;                                    // Motor is stopped
    EEPROM.write(saddr, coverStatus);                         // Store the status so next time we know we left it open
  }

    #ifdef DEBUG
      Serial.print("Current Stored Position After Move : ");
      Serial.println(EEPROM.read(paddr));
      Serial.print("Current Cover Status : ");
      Serial.println(EEPROM.read(saddr));
    #endif
    
}
