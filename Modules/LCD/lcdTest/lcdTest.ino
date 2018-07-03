#include <Wire.h>

#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack

void setup()
{
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  
}

void loop()
{
  lcd.home (); // set cursor to 0,0
  lcd.print("LCD MODULE"); 
  lcd.setCursor (0,1);        // go to start of 2nd line
  lcd.print("TEST");
  delay(1000);
}
