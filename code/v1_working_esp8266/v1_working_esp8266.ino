#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "ssid";
const char* password = "pass";

const char* api_host = "api.helium.io";
const char* hotspot_name = "112jHrQYpAHmQHgukcv485i226Ci4kwFSK84NLnSsnVELhj6E44B"; //your gateway id, here is mine for an example

IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);
  //WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
client.setInsecure();
    
    HTTPClient https;
    Serial.println("Requesting ");
    
    if (https.begin(client, "https://api.helium.io/v1/hotspots/" + String(hotspot_name) + "/rewards/sum?min_time=-1%20day")) {
      int httpCode = https.GET();
      Serial.println(String(httpCode));
      if (httpCode > 0) {
        String dataOneDay = https.getString();
        Serial.println(dataOneDay);
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, dataOneDay);
        float earningsDay = doc["data"]["total"];
        Serial.print("LAST 24h: ");
        Serial.println(earningsDay, 8);
        //https.end();
        https.begin(client, "https://api.helium.io/v1/hotspots/" + String(hotspot_name) + "/rewards/sum?min_time=-30%20day");
        https.GET();
        String dataOneMonth = https.getString();
        Serial.println(dataOneMonth);
        deserializeJson(doc, dataOneMonth);
        float earningsMonth = doc["data"]["total"];
        Serial.print("PAST MONTH: ");
        Serial.println(earningsMonth, 8);
      }
      https.end();
    } else {
      Serial.printf("Unable to connect");
    }

  }
  delay(600000); //update every 10 minutes
}