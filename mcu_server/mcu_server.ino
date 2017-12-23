/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 */
#include <SPI.h>
#include <ESP8266WiFi.h>

#define LED_OFF_CODE 1   //to control led on
#define LED_OFF_CODE 0

byte ledPin = 2;
char ssid[] = "caoboibk";               // SSID of your home WiFi
char pass[] = "12345687";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 0, 80);            // IP address of the server
IPAddress gateway(192,168,0,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

void setup() {
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());*/
  pinMode(ledPin, OUTPUT);
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      client.flush();
      client.println("\r"); // sends the answer to the client
      digitalWrite(ledPin, HIGH);
    }
    client.stop();                // tarminates the connection with the client
  }
}

