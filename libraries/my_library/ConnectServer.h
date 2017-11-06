#ifndef _UbidotsAndESP8266_H_
#define _UbidotsAndESP8266_H_
#endif

#include <ESP8266WiFi.h>
#include "WiFiUdp.h"

#ifndef TIME_SERVER
#define TIME_SERVER "pool.ntp.org"
#endif

#ifndef SERVER
#define SERVER "translate.ubidots.com"
#endif
#ifndef PORT
#define PORT 9012
#endif

#ifndef HTTPSERVER
#define HTTPSERVER "things.ubidots.com"
#endif
#ifndef HTTPPORT
#define HTTPPORT 80
#endif

#ifndef USER_AGENT
#define USER_AGENT "UbidotsESP8266"
#endif
#ifndef VERSION
#define VERSION "1.2"
#endif
#ifndef DEFAULT_DEVICE_NAME
#define DEFAULT_DEVICE_NAME "ESP8266"
#endif

typedef struct Value {
  char  *id;
  char *context;
  float value_id;
  unsigned long timestamp;
} Value;

class Ubidots {
 public:
    Ubidots(char* token, char* server = SERVER); //constructor
    bool sendAll(bool type = false);
    bool sendHTTP();
    bool sendTLATE();
    void add(char *variable_id, float value);
    void add(char *variable_id, float value, char *ctext);
    void add(char *variable_id, float value, unsigned long timestamp);
    void add(char *variable_id, float value, char *ctext, unsigned long timestamp);
    void setDebug(bool debug);
    bool wifiConnection(char *ssid, char *pass);
    void setDataSourceName(char* dataSoruceName);
    void setDataSourceLabel(char* dataSoruceLabel);
    unsigned long ntpUnixTime ();

 private:
    void idAsMac();
    bool _debug = false;
    char* _token;
    char* _server;
    char* _dsName;
    char* _idName;
    char* _espID = (char *) malloc(sizeof(char) * 100);
    uint8_t maxValues;
    uint8_t currentValue;
    Value * val;
    float parseValue(String body);
    WiFiClient _client;
    WiFiUDP udp;
};