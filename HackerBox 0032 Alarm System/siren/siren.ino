int buzz=12; // Buzzer Pin

void setup() 
{
}

void loop()
{
  //siren();
}



void siren() 
{         
  for(int hz = 440; hz < 1000; hz+=25)
  {
    tone(buzz, hz, 50);
    delay(5);
  }
  for(int hz = 1000; hz > 440; hz-=25)
  {
    tone(buzz, hz, 50);
    delay(5);
  }
}
