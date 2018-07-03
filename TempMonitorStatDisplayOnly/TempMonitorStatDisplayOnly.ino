#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include "DHT.h"

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack

void setup()
{  
  //Pot Loads
  Serial.begin(9600);           // set up Serial library at 9600 bps
  dht.begin();
  Serial.println("Test Start");  // prints hello with ending line break 
  
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  lcd.print("                               ");
  lcd.setCursor (0,1);        // go to start of 2nd line
}

void loop()
{
  delay(2000);
 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  
  // Read temperature as Celsius
  float tc = dht.readTemperature();
  
  // Convert to F
  float tf= tc*(int(9)/double(5))+32;
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(tc)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(tc);
  Serial.print(" *C \t");
  Serial.print(tf);
  Serial.println(" *F ");

  lcd.setCursor (0,0);
  lcd.print("Temp:     ");
  lcd.print(tf);
  
  lcd.print("F");
  lcd.setCursor (0,1);  
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
  
}


