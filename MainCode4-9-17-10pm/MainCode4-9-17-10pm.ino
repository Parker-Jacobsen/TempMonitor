#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include "DHT.h"

#define DHTPIN 13     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

const int upBtn = 12;     
const int downBtn =11;

int upState = 0;         // variable for reading the pushbutton status
int downState = 0;         // variable for reading the pushbutton status
int homeVal= 32;

DHT dht(DHTPIN, DHTTYPE);

Servo xAxis;
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified backpack


int Temp[]=   {65,66,67,68,69,            70,71,72,73,74,               75,76,77,78,79,     80,81,82,83,84,       85,86,87,88,89,90};
int Xvals[] = {33,39,46,52,59,            65,68,71,74,77,               80,84,87,90,93,     95,98,101,104,107,    110,113,116,119,121,123};

int buffer=2;
float h=23;
float tc=23;
float tf=65;
int desiredTemp=65;
int servoPosition=65;

void setup() 
{
  dht.begin();
  
  //servo loads
  xAxis.attach(8);
  xAxis.write(homeVal);
  servoPosition=homeVal;
  pinMode(upBtn, INPUT);
  pinMode(downBtn, INPUT);
    
  //Pot Loads
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("App Start");  // prints hello with ending line break 
  
  // activate LCD module
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.noBlink();

  //Clear Current screen
  lcd.home();
  lcd.print("                               ");
  lcd.setCursor (0,1);        // go to start of 2nd line
  lcd.print("                               ");

}

void loop() 
{
    takeReading();
    takeReading();    //Ensures a correct read has changed the variable
    takeReading();
    
    printToLCD();
    servoAlign();
    
   // Serial.print("Humidity: "); 
    //Serial.print(h);
    //Serial.print(" %\t");
    //Serial.print("Temperature: "); 
    //Serial.print(tf);
    //Serial.println(" *F ");
    delayCheckButtons(10000);
}


void servoAlign()
{
  if(tf>=(desiredTemp+buffer))
    {
      goHome();
      Serial.println("Finding Index of lower temp");
      int n=findIndex(desiredTemp);
      Serial.println(n);
      servoPosition=desiredTemp;
      xAxis.write(Xvals[n]);
    }
    if(tf<=(desiredTemp-buffer))
    {
      Serial.println("Finding Index of higher temp");
      int n=findIndex(desiredTemp);
      Serial.println(n);
      servoPosition=desiredTemp;
      xAxis.write(Xvals[n]);
    }
}

void delayCheckButtons(int duration)
{
  Serial.println("DELAYY");
  checkOutMyButt();
  
  for(int i=0;i<duration;i+=100)
  {
    delay(50);
    if(upState)
    {
      checkOutMyButt();
      if(upState)
      {
        Serial.println("IN Up State");
        delay(50);
        if(desiredTemp<90)
        {
          desiredTemp=desiredTemp+1;
          printToLCD();
          servoAlign();
        } 
      }
    }
    if(downState)
    {
      checkOutMyButt();
      if(downState)
      {
        Serial.println("IN Down State");
        delay(50);
        if(desiredTemp>65)
        {
          desiredTemp=desiredTemp-1;
          printToLCD();
          servoAlign();
        }
      }
    }
    delay(50);
  }
    Serial.println("/DELAYY");
}
void checkOutMyButt()
{
  upState = digitalRead(upBtn);
  downState = digitalRead(downBtn);
}
void printToLCD()
{
  
    //Return home and write values
    lcd.home (); // set cursor to 0,0
    lcd.print("T:");
    if(tf<100)
    {
      lcd.print(" ");
    }
    lcd.print((int)tf);
    lcd.setCursor (7,0);
    lcd.print("H:");
    lcd.print((int)h);
    lcd.setCursor(13,0);
    lcd.print("B:");
    lcd.print(buffer);
    lcd.setCursor (0,1);        // go to start of 2nd line
    lcd.print("G:");
    if(desiredTemp<100)
    {
      lcd.print(" ");
    }
    lcd.print(desiredTemp);
    lcd.setCursor (7,1);
    lcd.print("P:");
    lcd.print(servoPosition);
    
}
void takeReading()
{

    int th = dht.readHumidity();
    if(th>5&&th<100)
    {
      h = th;
    }

    // Read temperature as Celsius
    int ttc = dht.readTemperature();
    if(ttc>20&&ttc<47)
    {
          tc = ttc;
    }

    // Convert to F
    tf= tc*(int(9)/double(5))+32;


  if (isnan(h) || isnan(tc)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else
  {
    Serial.println(tf);
  }
 
}
void goHome()
{
   xAxis.write(homeVal);
   delay(500);
   Serial.println("Return Home");
}

int findIndex(int input)
{
  for(int i=0;i<26;i++)
  {
    if (Temp[i]==input)
    {
      return i; 
    }
  }
    return -1;
}

