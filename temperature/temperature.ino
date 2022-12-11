//https://github.com/adafruit/DHT-sensor-library
//https://navody.dratek.cz/navody-k-produktum/teplotni-senzor-dht11.html

//https://navody.dratek.cz/docs/texty/0/16/dht_sensor_library.zip
#include "DHT.h"

// data pin number
// D4 is atually D2 on my D1 mini ¯\_(ツ)_/¯
#define pinDHT 4
#define useSerial true
#define cycleTime 5000

//#define typDHT11 DHT11     // DHT 11
#define typDHT22 DHT22   // DHT 22 (AM2302)

// inicialization
DHT dhtSenzor(pinDHT, typDHT22);

void setup() {
  if(useSerial){
    Serial.begin(9600);   
  }
  dhtSenzor.begin();
}

void loop() {
  // reading took 250 ms
  float temp = dhtSenzor.readTemperature();
  float hum = dhtSenzor.readHumidity();
  
  if (isnan(temp) || isnan(hum)) {
    if(useSerial) { 
      Serial.println("Chyba při čtení teploty!");
    }
  } else {
    if(useSerial) {
      print(temp, hum);      
    }
  }
  
  delay(cycleTime);
}

void print(float temp, float hum) {
  if(useSerial) {
    Serial.print("Temperature: "); 
    Serial.print(temp);
    Serial.print("°C, ");
    Serial.print("humidity: "); 
    Serial.print(hum);
    Serial.println(" %");
  }
}
