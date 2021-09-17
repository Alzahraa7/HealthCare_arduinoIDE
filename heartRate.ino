void heratbeat()
{
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();
     
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
       tone(buzzer,500);          //And tone the buzzer for a 100ms you can reduce it it will be better
    delay(100);
    noTone(buzzer);
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);
 
  if (irValue < 50000)
    Serial.print(" No finger?");
  Serial.println();
  if( hrtemp< beatAvg ){
    hrtemp = beatAvg;
    Firebase.setFloat("Avg BPM",hrtemp );
  } //print hrtemp IOT
  lcd.setCursor(-4,3);
  lcd.print("Avg BPM =  ");
  lcd.setCursor(7,3);
  lcd.print(beatAvg);
  lcd.print("    ");
  /*if (count1 == 0 ){
  particleSensor.setup(); //Configure sensor with default settings
  count1++;}
 long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();
     
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
      tone(buzzer,500);          //And tone the buzzer for a 100ms you can reduce it it will be better
    delay(100);
    noTone(buzzer); 
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
  lcd.setCursor(0,3);
  lcd.print("Avg BPM = ");
  lcd.setCursor(10,3);
  lcd.print(beatAvg);*/
}
