void setLight(int val)
{
  if ( val == CALIBRATORREADY)
  {
    digitalWrite(lightPin,HIGH);
    #ifdef DEBUG
      Serial.println("Light on");
    #endif     
  }
  else if ( val == CALIBRATOROFF)
  {
    digitalWrite(lightPin,LOW);
    #ifdef DEBUG
      Serial.println("Light off");
    #endif              
    
  }
}

void setShutter(int val)
{
  if ( val == COVEROPEN)
  {
    targetpos = 0;
  }
  else if ( val == COVERCLOSED)
  {
    targetpos = closedPos;
  }
}
