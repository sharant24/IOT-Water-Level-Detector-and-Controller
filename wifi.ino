
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <PubSubClient.h>
// defines pins numbers
const int TRIGGER = 14;  //D5
const int ECHO = 12;  //D6
const int buzzer = 4; // d2

// defines variables
long duration;
float distance;
const char *ssid =  "ST dada";        
const char *pass =  "12345678";   
const char* serverTS = "api.thingspeak.com";
String apiKey = "E6AD24K0BYEPDJ2Y";          
void setup() 
{
  
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);         //Recommended speed is 115200
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
  connectWifi();
 }

void loop() 
{
  // The PING is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.

  duration = pulseIn(ECHO, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  
  if (distance <= 10){
       Serial.print("Tank is full (distance): ");
       Serial.println(distance);
       digitalWrite(buzzer, HIGH);
       delay(200);
       digitalWrite(buzzer, LOW);
       delay(200);
  }
 else if (distance >=200){ 
      Serial.print("Tank is empty (distance): ");
      Serial.println(distance);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
  }
  else{
      Serial.print("cool tank (Distance): ");
      Serial.println(distance);
      delay(200);
     }
  sendHeight(distance);
}
void connectWifi()
{ 
  Serial.print("Connecting to "+*ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect


 void sendHeight(float distance)
{  
  WiFiClient tclient;//not to be confused with "client" in PubSub{}, and wclient for mqtt
   if (tclient.connect(serverTS, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   //Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(distance);
   postStr += "\r\n\r\n";
   
   tclient.print("POST /update HTTP/1.1\n");
   tclient.print("Host: api.thingspeak.com\n");
   tclient.print("Connection: close\n");
   tclient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   tclient.print("Content-Type: application/x-www-form-urlencoded\n");
   tclient.print("Content-Length: ");
   tclient.print(postStr.length());
   tclient.print("\n\n");
   tclient.print(postStr);
   delay(200);
   
   }//end if

 tclient.stop();
}//end send to ts
