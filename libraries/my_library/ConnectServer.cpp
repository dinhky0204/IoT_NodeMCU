#include <ConnectServer.h>

//Constructor
Ubidots::Ubidots(char* token, char* server) {

    _token = token;
    _server = server;
    _dsName = DEFAULT_DEVICE_NAME;
    maxValues = 5;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
    idAsMac();
}

void Ubidots::idAsMac(){
    unsigned char mac[6];
    WiFi.macAddress(mac);
    for (int i = 0; i < 6; i++){
        if ((int)mac[i] < 10){
            sprintf(_espID, "%s0%X", _espID, mac[i]);
        } else {
            sprintf(_espID, "%s%2X", _espID, mac[i]);
        }
    }
}
void Ubidots::setDataSourceName(char *dataSourceName) {

    _dsName = dataSourceName;
}
void Ubidots::setDataSourceLabel(char* dataSourceLabel) {

    _espID = dataSourceLabel;
}

// Add variable
void Ubidots::add(char *variable_id, float value) {
    return add(variable_id, value, NULL, NULL);
}
void Ubidots::add(char *variable_id, float value, char *ctext) {
    return add(variable_id, value, ctext, NULL);
}
void Ubidots::add(char *variable_id, float value, unsigned long timestamp) {
    return add(variable_id, value, NULL, timestamp);
}
void Ubidots::add(char *variable_id, float value, char *ctext, unsigned long timestamp) {
    (val+currentValue)->id = variable_id;
    (val+currentValue)->value_id = value;
    (val+currentValue)->context = ctext;
    (val+currentValue)->timestamp = timestamp;
    currentValue++;
    if (currentValue>maxValues) {
        Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
        currentValue = maxValues;
    }
}

//Send all data

bool Ubidots::sendAll(bool type) {
    if (type) {
        return sendTLATE();
    } else {
        return sendHTTP();
    }
}
bool Ubidots::sendTLATE() {
    uint8_t i;
    String str;
    char* data = (char *) malloc(sizeof(char) * 700);

    sprintf(data, "%s/%s|POST|%s|%s:%s=>", USER_AGENT, VERSION, _token, _espID, _dsName);

    for (i = 0; i < currentValue;) {
         str = String(((val+i)->value_id), 5);

        sprintf(data, "%s%s:%s", data, (val + i)->id, str.c_str());

        if ((val + i)->timestamp != NULL) {
            sprintf(data, "%s@%lu%s", data, (val + i)->timestamp, "000");
        }
        if ((val + i)->context != NULL) {
            sprintf(data, "%s$%s", data, (val + i)->context);
        }

        i++;

        if (i < currentValue) {
            sprintf(data, "%s,", data);
        } else {
            sprintf(data, "%s|end", data);
            currentValue = 0;
        }
    }

    Serial.println("");

    if (_debug){
     Serial.println(data);
    }

    if (_client.connect(SERVER, PORT)) {
        _client.print(data);
    }
    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    free(data);
}

bool Ubidots::sendHTTP() {
    uint16_t i;
    String all;
    String str;
    all = "[";
    for (i = 0; i < currentValue; ) {
        str = String(((val+i)->value_id), 4);
        all += "{\"variable\": \"";
        all += String((val + i)->id);
        all += "\", \"value\":";
        all += str;
        all += "}";
        i++;
        if (i < currentValue) {
            all += ", ";
        }
    }

    all += "]";
    i = all.length();

   String toPost = "POST /api/v1.6/collections/values/?force=true HTTP/1.1\r\n"
                    "Host: things.ubidots.com\r\n"
                    "User-Agent:" + String(USER_AGENT) + "/" + String(VERSION) + "\r\n"
                    "X-Auth-Token: " + String(_token) + "\r\n"
                    "Connection: close\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: " + String(i) + "\r\n"
                    "\r\n"
                    + all +
                    "\r\n";

    if (_client.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Posting your variables: "));
        Serial.println(toPost);
        _client.print(toPost);
    }

    int timeout = 0;
    while(!_client.available() && timeout < 5000) {
        timeout++;
        delay(1);
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    return true;
}

void Ubidots::setDebug(bool debug){
     _debug = debug;
}


bool Ubidots::wifiConnection(char* ssid, char* pass) {
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    WiFi.setAutoReconnect(true);
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
}

