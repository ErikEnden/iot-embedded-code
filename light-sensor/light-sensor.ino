#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

Servo servoRed;
Servo servoBlue;

#define timeDelay 1UL * 60UL * 1000UL // 60 minutes each of 60 seconds each of 1000 milliseconds all unsigned longs = 1hr

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
  servoRed.attach(4);
  servoBlue.attach(2);
  servoRed.write(180);
  servoBlue.write(0);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly

  if((WifiMulti.run() == WL_CONNECTED)){
    measureLoop();
  }


  /*if(colorStatus == "red"){
    servo.write(90);
      delay(1000);
    } else {
      servo.write(0);
      delay(1000);
    }*/
  
  delay(timeDelay);
}

void sendData(String lv, String sensorMode){
  HTTPClient http;
  
  String postData, httpCode;
  http.begin("http://iot.ermine.ee:3000/update-level");
  http.addHeader("Content-type", "application/x-www-form-urlencoded");

  postData = "luxlevel=" + lv + "&sensormode=" + sensorMode;
  String payload = http.getString();
    
  int responseCode = http.POST(postData); 

  Serial.println(httpCode);
  Serial.println(payload);
  Serial.println(postData);
    
  http.end();
}

void measureLoop() {
  float luxValue;
  
  luxValue = 100 - ((float(analogRead(SENSOR_PIN)) - MIN_VALUE) * 100 / (MAX_VALUE - MIN_VALUE));
  sendData(String(luxValue), "none");
  rotateServo(servoRed, 60);
  luxValue = 100 - ((float(analogRead(SENSOR_PIN)) - MIN_VALUE) * 100 / (MAX_VALUE - MIN_VALUE));
  sendData(String(luxValue), "red");
  rotateServo(servoRed, 180);
  rotateServo(servoBlue, 45);
  luxValue = 100 - ((float(analogRead(SENSOR_PIN)) - MIN_VALUE) * 100 / (MAX_VALUE - MIN_VALUE));
  sendData(String(luxValue), "blue");
  rotateServo(servoBlue, 0);
}

void rotateServo(Servo servo, int rotation) {
  Serial.println("Rotating");
  servo.write(rotation);
  delay(1000);
}
