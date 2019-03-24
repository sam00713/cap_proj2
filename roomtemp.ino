#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFiMulti.h>

#define ONE_WIRE_BUS D2  

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define server "api.thingspeak.com"
String apiKey = "8KWNRSDF66WAJXRZ";
#ifndef STASSID
#define STASSID  "AndroidHotspot8939";
#define STAPSK "12344321"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;


ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("Connected");  
}

void loop() {
  float temp;
  sensors.requestTemperatures(); 
  temp = sensors.getTempCByIndex(0);
  
  WiFiClient client;
  if (client.connect(server, 80)) { 
    Serial.println("WiFi Client connected ");
    String postStr = apiKey + "&field1=" + String(temp)+"\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    delay(1000);
    if (client.connect("18.191.206.148", 8000)){
      String req = "GET /data?temp=" + String(temp) + " HTTP/1.1\r\n" + "Host: " + "18.191.206.148" + "\r\n" + "Connection: close\r\n\r\n";
      client.print(req);
      Serial.print(req);
    }
  }

  client.stop();
  int count =20;
  while(count--)
  delay(1000);
}
