//https://github.com/adafruit/DHT-sensor-library
//https://navody.dratek.cz/navody-k-produktum/teplotni-senzor-dht11.html

//https://navody.dratek.cz/docs/texty/0/16/dht_sensor_library.zip
#include "DHT.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include "secret.h"
// secret.h looks like
// #define URL "http://bla.bla/api/"
// #define WIFISSID "NameOfMyWifi"
// #define PSSWD "wifiPassword"

// data pin number
// D4 is atually D2 on my D1 mini ¯\_(ツ)_/¯
#define pinDHT 4
#define useSerial true
#define cycleTime 60000

//#define typDHT11 DHT11     // DHT 11
#define typDHT22 DHT22   // DHT 22 (AM2302)

// inicialization
DHT dhtSenzor(pinDHT, typDHT22);
ESP8266WiFiMulti WiFiMulti;

void setup() {
  if (useSerial) {
    Serial.begin(115200);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  blink();
  
  dhtSenzor.begin();
  delay(4000);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFISSID, PSSWD);
}

void loop() {
    // reading took 250 ms
  float temp = dhtSenzor.readTemperature();
  float hum = dhtSenzor.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    if (useSerial) {
      Serial.println("Error while reading temperature!");
    }
  } else {
    if (useSerial) {
      print(temp, hum);
    }
    send(temp, hum);
  }
  delay(cycleTime);
}

void print(float temp, float hum) {
  if (useSerial) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C, ");
    Serial.print("humidity: ");
    Serial.print(hum);
    Serial.println(" %");
  }
}

void blink(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);                       
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
}

void send(float temp, float hum) {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    //blink();
    WiFiClient client;
    HTTPClient http;

    String url = URL;
    url += "?temperature=" + String(temp);
    url += "&";
    url += "humidity=" + String(hum);
    
    if (http.begin(client, url)) {      
      int start=millis();
      int statusCode = http.GET();
      int end =millis();
      Serial.println(String("Effective timeout: ") + (end-start));
      if (statusCode > 0) {
        Serial.println("sending ok1");        
        Serial.println(String(statusCode));        
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(statusCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
}
