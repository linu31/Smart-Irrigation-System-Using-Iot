#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"
WiFiClient client;
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "VK#18";
const char* password ="pfpq1814";

const char* serverUrl = "http://192.168.133.54:3000/sensordata";
const int port=3000;

unsigned long startMillis;

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  startMillis = millis();
}

void loop() {
  if (millis() - startMillis >= 720000) {
    Serial.println("❌ Stopping data collection after 12 minutes.");
    while (true) { 
      delay(1000); 
    }
  }

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soil = analogRead(A0);

  int soilMoisturePercentage = map(soil, 0, 1023, 0, 100);

  if (isnan(temp) || isnan(humidity)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    return;
  }

  Serial.println("------ Sensor Data ------");
  Serial.print("🌡️ Temp: "); Serial.print(temp); Serial.println(" °C");
  Serial.print("💧 Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("🌱 Soil Moisture: "); Serial.println(soilMoisturePercentage); Serial.println(" %");
  Serial.println("-------------------------");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = serverUrl;
    url += "?temp=" + String(temp);
    url += "&humidity=" + String(humidity);
    url += "&soil=" + String(soilMoisturePercentage);

    http.begin(client, url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("✅ Data sent successfully");
    } else {
      Serial.println("❌ Failed to send data");
    }
    http.end();
  } else {
    Serial.println("⚠️ WiFi not connected");
  }

  delay(5000);
}
