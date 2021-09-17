void LM35()
{
  /*for (int i =0 ; i<10 ; i++){
    value += analogRead(sens);
    delay(50);
    }*/
  lcd.setCursor(-4, 3);
   lcd.print("Temp:  ");
  delay(500);

  // Send the command for all devices on the bus to perform a temperature conversion:
  sensors.requestTemperatures();

  // Fetch the temperature in degrees Celsius for device index:
  float tempC = sensors.getTempCByIndex(0); // the index 0 refers to the first device
  // Fetch the temperature in degrees Fahrenheit for device index:
  float tempF = sensors.getTempFByIndex(0);

  // Print the temperature in Celsius in the Serial Monitor:
  Serial.print("Temperature: ");
  float tempp= tempC+2;
  Serial.print(tempp);
  lcd.setCursor(3, 3);
  lcd.print(tempp);
  lcd.print("          ");
  delay(500);
  Firebase.setFloat("Tempreture", tempp);
      // handle error 
      if (Firebase.failed()) {
          Serial.print("setting / Tempreture failed:");
          Serial.println(Firebase.error());
          return;
      }
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.print("C  |  ");

  // Print the temperature in Fahrenheit
  Serial.print(tempF);
  Serial.print(" \xC2\xB0"); // shows degree symbol
  Serial.println("F");

  // Wait 1 second:
  delay(1000);
  
  /*value = analogRead(sens);
  volt =(3.5/1023.0)*value;
  temp = (volt/0.01); // 10mv/c
  //temp = (value-32)*5/9;
  if (temp <=40){
  lcd.setCursor(3, 3);
  lcd.print(temp);
  lcd.print("          ");
  delay(500);
  }
  Serial.print("temp: ");
  Serial.println(temp);*/
}
