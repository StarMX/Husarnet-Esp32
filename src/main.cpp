#include "Arduino.h"
#include <WiFi.h>
#include <Husarnet.h>
#include <WebServer.h>

#include "secret.h"

// Setup http server
WebServer server1(8001);
#include <ESPAsyncWebServer.h>
AsyncWebServer server(8000);
// WiFi credentials

void setup()
{
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(SS_ID, SS_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("done!\r\nLocal IP: %s\r\n", WiFi.localIP().toString().c_str());
  // Start Husarnet
  Husarnet.selfHostedSetup(dashboardURL);
  Husarnet.join(husarnetJoinCode, hostName);
  Husarnet.start();

  // Configure http server
  server1.on("/", []() {
    server1.send(200, "text/html", "Hello World! " + String(millis()));
  });
  server1.onNotFound([]() {
    server1.send(404, "text/html", "NotFound");
  });
  server1.begin();

  server.on("/", [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
  });
  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not Found");
  });

  server.begin();
}

void loop()
{
  // Handle connections
  server1.handleClient();
}
