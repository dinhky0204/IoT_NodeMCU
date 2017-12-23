#include <ESP8266WiFi.h>

#define SIGNAL_PIN D4
 
void setup()  
{
  Serial.begin(115200);
  pinMode(SIGNAL_PIN, INPUT);
}
void loop()  
{
  if(digitalRead(SIGNAL_PIN)==HIGH) {
    Serial.println("Movement detected.");
  } else {
    Serial.println("Did not detect movement.");
  }
  delay(500);
}
