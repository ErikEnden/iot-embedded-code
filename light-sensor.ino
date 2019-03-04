#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define DEBUG_PROG 

#ifdef DEBUG_PROG
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINT(x)    Serial.print(x)
#else
  #define DEBUG_PRINTLN(x) 
  #define DEBUG_PRINT(x)
#endif

#define MIN_VALUE 25
#define MAX_VALUE 800

#define SENSOR_PIN A0 

#define USE_SERIAL Serial

#define WifiUser "TLU"
#define WifiPW ""

ESP8266WiFiMulti WifiMulti;

using namespace std;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WifiMulti.addAP(WifiUser, WifiPW);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  float luxValue = 100 - ((float(analogRead(SENSOR_PIN)) - MIN_VALUE) * 100 / (MAX_VALUE - MIN_VALUE));
  
  if((WifiMulti.run() == WL_CONNECTED)){
    sendData(String(luxValue));
  }
  
  
  delay(5000);
}

void sendData(String lv){
  HTTPClient http;
  
  String postData, httpCode;
  http.begin("iot.ermine.ee:3000/update-level");
  http.addHeader("Content-type", "application/x-www-form-urlencoded");

  postData = "luxlevel=" + lv;
  String payload = http.getString();
    
  int responseCode = http.POST(postData); 

  Serial.println(httpCode);
  Serial.println(payload);
  Serial.println(luxValue);
    
  http.end();
}
