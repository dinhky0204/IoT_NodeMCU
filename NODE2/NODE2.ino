/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include "UbidotsMicroESP8266.h"
#include <SPI.h>
#include <ESP8266WiFi.h>

#define ledPin D2
#define pinBell D7
#define WIFISSID "caoboibk" // Your SSID
#define PASSWORD "12345687" // Your Wi-Fi password
#define TOKEN  "A1E-R5aTERNAgxNniadNasBpsk16pLumEX"


unsigned long askTimer = 0;
WiFiServer server(80);
IPAddress ip(192, 168, 0, 80);            // IP address of the server
IPAddress gateway(192,168,0,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

Ubidots client(TOKEN);
int control_value = 0;
int bell_status;
int frequency = 2000;
int light_control = 0;
void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(WIFISSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
}

void loop () {
  WiFiClient client_lan = server.available();
  light_control = client.getValueUDP("5a145d3bc03f9742d68c185f"); //get light control value
  bell_status = client.getValueUDP("5a1adfd6c03f97655733b420");
  if(light_control == 1) {
    control_value = 1;
    digitalWrite(ledPin, HIGH);
    noTone(pinBell);
  }
  else {
    if (client_lan) {
      if (client_lan.connected()) {
        digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
        Serial.println(".");
        String request = client_lan.readStringUntil('\r');    // receives the message from the client
        control_value = request.toInt();
        Serial.print("From client: "); Serial.println(request);
        
        switch(control_value) {
          case 1:
            digitalWrite(ledPin, HIGH);
            noTone(pinBell);
          break;
          case 2: //turn on bell
            tone(pinBell, frequency);
            digitalWrite(ledPin, LOW);
          break;
          case 3: //turn off bell
            noTone(pinBell);
          break;
          case 0:
            digitalWrite(ledPin, LOW);
          break;
          default:
            digitalWrite(ledPin, LOW);
          break;
        }
        client_lan.flush();
        
      }
      client_lan.stop();                // tarminates the connection with the client
    }
  }
  
}
