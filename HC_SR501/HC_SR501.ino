#include "UbidotsMicroESP8266.h"
#include <SPI.h>
#include <ESP8266WiFi.h>

#define ledPin D7 // Red LED
#define pirPin D1 // Input for HC-SR501
#define DEVICE_LABEL "..."
#define VARIABLE_LABEL "..."
#define TOKEN  "A1E-R5aTERNAgxNniadNasBpsk16pLumEX"
char ssid[] = "nguyendinhky";               // SSID of your home WiFi
char pass[] = "12345687";
#define CLIENTNAME "Nguyen Dinh Ky"

unsigned long askTimer = 0;
IPAddress server(172,20,10,6);       // the fix IP address of the server
WiFiClient client_lan;
//subnetMask: 255.255.255.240

int pirValue; // variable to store read PIR Value
Ubidots client(TOKEN);
int frequency = 2000;
int protect_status = 0;
int light_control = 0;
int check_value = 0;
int protect = 0;
int protect_value = 0;
int bell_status;

void setup() 
{
  Serial.begin(115200);
  delay(100);
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(pirPin, INPUT_PULLUP);
  digitalWrite(ledPin, LOW);
}



void loop() 
{
//  check_value = digitalRead(pirPin); //check value of SR501
  light_control = client.getValueUDP("5a145d3bc03f9742d68c185f"); //get light control value
  protect_status = client.getValueUDP("5a3140e9c03f972373180845"); //get protection control value
  bell_status = client.getValueUDP("5a1adfd6c03f97655733b420");
  Serial.println("Bell: ");
  Serial.println(bell_status);
  Serial.println("Light: ");
  Serial.println(light_control);
  Serial.println("Protect: ");
  Serial.println(protect_status);
  if(bell_status==1) {
    client_lan.connect(server, 80);   // Connection to the server
    Serial.println(".");
    Serial.println("send: 2");
    client_lan.println("2\r"); 
    client_lan.flush();
  }
  else {
    client_lan.connect(server, 80);   // Connection to the server
    Serial.println(".");
    Serial.println("send: 3");
    client_lan.println("3\r"); 
    client_lan.flush();
  }
  
  if(light_control==1) {
    client_lan.connect(server, 80);   // Connection to the server
    Serial.println(".");
    client_lan.println("1\r");
    Serial.println("send: 1");
    digitalWrite(ledPin, HIGH);
    client_lan.flush();
  }
  else
  {
    if(protect_status == 0) {
      check_value = digitalRead(pirPin);
      Serial.println(pirValue);
//      digitalWrite(ledPin, check_value);
      if(check_value == 1) {
        client_lan.connect(server, 80);   // Connection to the server
        Serial.println(".");
        client_lan.println("1\r"); 
        client_lan.flush();
        Serial.println("send: 1,3");
        client_lan.println("3\r"); 
        client_lan.flush();
//        digitalWrite(ledPin, LOW);
//        send value to set light status to "on"
//        client.add("LED", 1);
//        client.sendAll();
      }
      else {
        client_lan.connect(server, 80);   // Connection to the server
        Serial.println(".");
        client_lan.println("0\r"); 
        client_lan.flush();
      }  
    }
    else {
      if(check_value == 1) {
//        client.add("PROTECT", protect_value);
//        client.sendAll(true);
        //bell
        client_lan.connect(server, 80);   // Connection to the server
        Serial.println("send: 2");
        client_lan.println("2\r"); 
        client_lan.flush();
        //send message  
        // send value to set value of bell to "on"
        
      } 
      else {
        client_lan.connect(server, 80);   // Connection to the server
        Serial.println(".");
        client_lan.println("0\r"); 
        client_lan.flush();
      }
    }
      
  }
    
}
