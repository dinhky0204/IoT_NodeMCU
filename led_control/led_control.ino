#include "UbidotsMicroESP8266.h"  //import ubiots library

#define LED_PIN 12
#define DELAY_TIME 2000

#define DEVICE_LABEL "..." // Put here your Device Label
#define VARIABLE_LABEL "..." // Put here your Variable Label
#define TOKEN  "A1E-R5aTERNAgxNniadNasBpsk16pLumEX"  // Put here your Ubidots TOKEN
#define WIFISSID "caoboibk" // Your SSID
#define PASSWORD "12345687" // Your Wi-Fi password
#define CLIENTNAME "Nguyen Dinh Ky"


Ubidots client(TOKEN);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {
//  digitalWrite(LED_PIN, LOW);
//  delay(DELAY_TIME);
//  digitalWrite(LED_PIN, HIGH);
//  delay(DELAY_TIME);
  float value = client.getValueUDP("5a145d3bc03f9742d68c185f");
//  Serial.print("The value received form Ubidots is: ");
//  Serial.print(value);
  if(value == 1) {
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
}
