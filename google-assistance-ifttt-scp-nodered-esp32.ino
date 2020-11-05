#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "deepkumardas";
const char* password = "202019931993";

const int ledPinOK = 23;
const int ledPinNOK = 22;
String url = "https://noderedonscpcf-XXXXXXXXX-XXXXXXXYYYY-va.cfapps.eu10.hana.ondemand.com/getData";


void setup() {
  pinMode(ledPinOK, OUTPUT);
  pinMode(ledPinNOK, OUTPUT);
 
  Serial.begin(115200);
  delay(1000);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.useHTTP10(true);
    http.begin(url);
   
    int httpCode = http.GET();
    if (httpCode>0) {
      DynamicJsonDocument doc(2048);
      DeserializationError error = deserializeJson(doc, http.getStream());
     
      if(error) {
        Serial.print("parse failed");
      }
      else{
        int espstatus = doc["state"];
        Serial.print(espstatus);
        if (espstatus == 0){
          digitalWrite(ledPinNOK, HIGH);  
          delay(1000);                  
          digitalWrite(ledPinNOK, LOW);  
          delay(400);                  
        }
        else{
          digitalWrite(ledPinOK, HIGH);  
          delay(1000);                  
          digitalWrite(ledPinOK, LOW);
          delay(400);                  
        }
      }
    }
    else {
      Serial.print("Error code: ");
    }
    Serial.println(httpCode);
    http.end();
  }
}
