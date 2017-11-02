#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  dht.begin();

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(BUILTIN_LED, LOW);

    Serial.print("Temperature: ");
    Serial.print(dht.readTemperature());
    Serial.print(" *C\t");

    Serial.print("Humidity: ");
    Serial.print(dht.readHumidity());
    Serial.println(" %");

    digitalWrite(BUILTIN_LED, HIGH);

    delay(3000);
}
