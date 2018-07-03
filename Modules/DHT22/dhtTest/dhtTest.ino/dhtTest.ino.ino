#include "DHT.h"
 
#define DHTPIN 13     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
 
int maxHum = 0;
int maxTemp = 0;
 
DHT dht(DHTPIN, DHTTYPE);
 
void setup() 
{
  Serial.begin(9600); 
  dht.begin();
}
 
void loop() 
{
  // Wait a few seconds between measurements.
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
 
}
