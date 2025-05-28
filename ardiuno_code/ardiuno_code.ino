#include <WiFi.h>
#include <HTTPClient.h>


//wifi cred and the server endpoint
const char* ssid = "Password";
const char* password = "12345678";
const char* serverUrl = "http://YOUR_SERVER_IP:5000/api/heatmap";

//wifi connection and setup
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}


//mac detection
void loop() {
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    String mac = WiFi.BSSIDstr(i);
    int rssi = WiFi.RSSI(i);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/json");
      String body = "{\"mac\": \"" + mac + "\", \"rssi\": " + String(rssi) + "}";
      int httpResponseCode = http.POST(body);
      http.end();
      Serial.printf("Sent: %s, RSSI: %d, Response: %d\n", mac.c_str(), rssi, httpResponseCode);
    }
    delay(100); 
  }
  delay(1000); 
}