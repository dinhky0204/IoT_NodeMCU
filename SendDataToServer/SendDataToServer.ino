#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTTYPE DHT11

const char* ssid     = "caoboibk";
const char* password = "12345687";
String pubString;
const char* host = "api.devicehub.net";//api.devicehub.net
int DHTPIN = D4; // Set  pin you are using to read data. In this case it's analog for reading pH values
float pH = 0; // Initialize the value you are going to read
const int sleepTimeS = 1*10; // Set sleep time in seconds for the deep sleep cycle
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(100);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // This is your NodeMCU IP address. Could be handy for other projects

  POST(); // Post the value to the web database
  GET();  // If all you need to do is push data, you don't need this

  Serial.println("ESP8266 in sleep mode");
  // Put NodeMCU in deep sleep. When it wakes up it will run setup() again,
  // connect to WiFi, then post and/or get data, then go back to sleep and repeat
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop()
{
  // This is empty because in order for the sleep cycle to work all code is in setup()
}

void POST(void)
{
  delay(5000);  // You can get rid of this or decrease it
//  pH = analogRead(pH_pin); // Read pH value
  
  pH = dht.readHumidity();  //custom

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  pubString = "{\"value\": " + String(pH) + "}";
  String pubStringLength = String(pubString.length(), DEC);
  // We now create a URI for the request
  Serial.print("Requesting POST: ");
  // Send request to the server:
  client.println("POST /v2/project/PROJECT_ID/device/Device_UUID/sensor/SENSOR_NAME/data HTTP/1.1");
  // Note: replace the ALL CAPS parts above with info from DeviceHub.net. After setting up an account,
  // add a project, a device (NodeMCU), a sensor (like pH_Sensor), and actuator if you need. After you set
  // up the project it should show you an API key, device UUID, and of course the sensor name you entered.
  client.println("Host: api.devicehub.net");
  client.print("X-ApiKey: YOUR_API_KEY\r\n"); // Get this from DeviceHub.net
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(pubStringLength);
  client.println();
  client.print(pubString);
  client.println();
  delay(500); // Can be changed
  
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}

// If you only need to post data to the database then you don't need the code below:
void GET(void)
{
  int state_pos;
  String state;
  delay(5000);
  pH = dht.readHumidity(); // Read pH value
  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  pubString = "{\"value\": " + String(pH) + "}";
  String pubStringLength = String(pubString.length(), DEC);
  // We now create a URI for the request
  Serial.print("Requesting GET: ");
  // This will send the request to the server
  client.println("GET /v2/project/PROJECT_ID/device/Device_UUID/actuator/ACTUATOR_NAME/state HTTP/1.1");
  client.println("Host: api.devicehub.net");
  client.print("X-ApiKey: YOURAPIKEY\r\n");
  client.println("Connection: close");
  client.println();
  delay(500);
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    state_pos = line.indexOf("state");
    if (state_pos > 0)
      state = line.substring(state_pos + 7, state_pos + 8);
    else
      state = "nothing";
    //Serial.println(state);
    if (state == "1")
      Serial.println("LED ON");
      digitalWrite(BUILTIN_LED, LOW); // You can change this
    if (state == "0")
      Serial.println("LED OFF");
      digitalWrite(BUILTIN_LED, HIGH);
    //Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}