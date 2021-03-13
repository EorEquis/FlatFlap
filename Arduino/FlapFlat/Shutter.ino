void SetShutter(int val)
{
  if( val == OPEN && coverStatus != OPEN )
  {
    motorStatus = RUNNING;
    for(pos = currentpos; pos >= openpos; pos -= 1)  // Open the flap 
    {                                  
      flapservo.write(pos);            
      delay(15);                       
    } 
    coverStatus = OPEN;
    motorStatus = STOPPED;
    currentpos = flapservo.read();
  }
  else if( val == CLOSED && coverStatus != CLOSED )
  {
    motorStatus = RUNNING;
    for(pos = currentpos; pos <= closedpos; pos += 1)     // Close the flap
    {                                  
      flapservo.write(pos);            
      delay(15);                       
    } 
    coverStatus = CLOSED;
    motorStatus = STOPPED;    
    currentpos = flapservo.read();
  }
}
