/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

#define LED_OFF_CODE 1   //to control led on
#define LED_OFF_CODE 0   //TO CONTROL LED OFF
#define ledPin D2
#define pinBell D7
#define WIFISSID "nguyendinhky" // Your SSID
#define PASSWORD "12345687" // Your Wi-Fi password


unsigned long askTimer = 0;
WiFiServer server(80);
IPAddress ip(172, 20, 10, 6);            // IP address of the server 172.20.10.12
IPAddress gateway(172,20,10,1);           // gateway of your network 172.20.10.1
IPAddress subnet(255,255,255,240);          // subnet mask of your network
//255.255.255.240

//IPAddress server(192,168,0,80);       // the fix IP address of the server
//WiFiClient client;

int control_value = 0;
int frequency = 2000;
void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(WIFISSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
  server.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
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
      client.flush();
      
    }
    client.stop();                // tarminates the connection with the client
  }
}
