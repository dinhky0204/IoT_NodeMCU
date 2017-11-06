#include "UbidotsMicroESP8266.h"  //import ubiots library
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

#define DEVICE_LABEL "..." // Put here your Device Label
#define VARIABLE_LABEL "..." // Put here your Variable Label
#define TOKEN  "A1E-R5aTERNAgxNniadNasBpsk16pLumEX"  // Put here your Ubidots TOKEN
#define WIFISSID "caoboibk" // Your SSID
#define PASSWORD "12345687" // Your Wi-Fi password
#define CLIENTNAME "Nguyen Dinh Ky"

DHT dht(DHTPIN, DHTTYPE);

Ubidots client(TOKEN);

void setup() {
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
  //client.setDebug(true); // Uncomment this line to set DEBUG on
}

void loop() {

    float value1 = dht.readHumidity();
    float value2 = dht.readTemperature();
    Serial.print(dht.readHumidity());
    client.add("temperature", value2);
    client.add("humidity", value1);
    //client.add("switch", value2);
    client.sendAll(true);
    delay(5000);
}
