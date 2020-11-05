#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
int temp = 0 ;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YOUR_BLYNK_AUTH_CODE";

// Your WiFi credentials.
char ssidb[] = "WIFI ID";
char passb[] = "WIFI PASSWORD";
int pinval ;
// SAP SCP specific configuration
const char* host = "HANA HOST"; //iotmms**********.hanatrial.ondemand.com
String device_id = "HANA IOT DEVICE ID"; //5******-*****-***-bc14-b5f1a*****
String message_type_id = "HANA IOT MESSAGE ID"; 
String oauth_token = "HANA IOT AUTH TOKEN";   
String url = "https://iotmmsp20000******trial.hanatrial.ondemand.com/com.sap.iotservices.mms/v1/api/http/data/" + device_id;
WiFiClientSecure clientTLS;
const int httpsPort = 443;
int analogPin = A0;                   
int val = 0; 

void wifiConnect() 
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssidb);

  WiFi.begin(ssidb, passb);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendMessage() 
 {
  String left = "{\"mode\":\"async\", \"messageType\":\"" + message_type_id + "\", \"messages\":[{\"timestamp\": " ;
  String right = "}]}";  
  String payload = left + "125377791" + ",  \"temp\": " + temp + right;
  //Serial.print("connecting to ");
  //Serial.println(host);
  //Serial.println(payload);
  if (!clientTLS.connect(host, httpsPort)) 
  {
    //Serial.println("connection failed");
    return;
  }
  Blynk.run(); 
  //Serial.print("requesting payload: ");
  //Serial.println(url);
  clientTLS.print(String("POST ") + url + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json;charset=utf-8\r\n" +
               "Authorization: Bearer " + oauth_token + "\r\n" +
               "Content-Length: " + payload.length() + "\r\n\r\n" +
               payload + "\r\n\r\n");  
  Serial.println("request sent");
  
  //Serial.println("reply was:");
  while (clientTLS.connected()) 
  {
    Blynk.run(); 
    String line = clientTLS.readStringUntil('\n');    
    //Serial.println(line);
  }
  Blynk.run(); 
  Serial.print("V1 Slider value is: ");
  Serial.println(pinval);    
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  wifiConnect();
  pinMode(LED_BUILTIN, OUTPUT);
  Blynk.begin(auth, ssidb, passb);
  delay(100);  
}


BLYNK_WRITE(V1)
{ 
  delay(1000); 
  pinval = param.asInt(); 
  Serial.print("V1 Slider value is: ");
  Serial.println(pinval);  
}  

void loop() 
{   
  Blynk.run(); 
  if ( pinval == 1 )
  {   
  digitalWrite(LED_BUILTIN, LOW);  
  delay(1000);           
  Blynk.run(); 
  temp = analogRead(analogPin);     
  Blynk.run(); 
  sendMessage();        
  delay(1000);   
  }
  else
  {
  digitalWrite(LED_BUILTIN, HIGH);
  }  
}
