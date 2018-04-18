#define TIMEOUT     5000 // mS
#define SOURCE      1002
#define SENSOR_INT  10  // seconds

#include <Wire.h>
#include "WiFiEsp.h"
#include <Adafruit_BMP085.h>

const char *ssid = "Clayton’s iPhone";
const char *pass = "emng3008";
const char *server = "93.188.165.221";
int status = WL_IDLE_STATUS;          // the Wifi radio's status

// Create an instance of the sensor object
Adafruit_BMP085 bmp;
WiFiEspClient client;

void setup()
{
  Wire.begin();             // Join i2c bus
  Serial.begin(115200);     // Communication with PC monitor via USB
  Serial1.begin(115200);

  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  printWifiStatus();

  // initialize sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  Serial.println("Wireless Temperature Sensor");
}

void loop() {
  // take sensor reading
  float temp = bmp.readTemperature();

  Serial.print("Temp(c):");
  Serial.println(temp, 2);

  // Establish TCP connection
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");

    // Build HTTP request.
    String cmd = "/emng3008/add.php?source=";
    cmd += SOURCE;
    cmd += "&value=";
    cmd += temp;

    Serial.println(cmd);

    client.print(String("GET ") + cmd + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("Waiting for response...");
    while (!client.available()) { }

    // print out the response
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    // drop the client connection
    Serial.println("Disconnecting from server...");
    client.stop();
  }
  else {
    Serial.println("connection to host failed");
    return;
  }

  delay(5000);
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
