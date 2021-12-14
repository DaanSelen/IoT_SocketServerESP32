#include <WiFi.h>
#include <SPI.h>
#include "LedMatrix.h"

#define NUMBER_OF_DEVICES 3
#define CS_PIN 15

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);
 
const char* ssid = "ScarSpot";
const char* password =  "Welkom01";
 
const uint16_t port = 7890;
const char * host = "proftaak.duckdns.org";

WiFiClient client;
String tcpData;

void setup()
{
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
 
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

  ledMatrix.init();
  ledMatrix.setIntensity(15);
 
  if (!client.connect(host, port)) {

      Serial.println("Connection to host failed");

      delay(1000);
      return;
  }
}

void engageText() {
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  delay(10);
}

void loop()
{
    while (client.available()) {
      char c = client.read();
      tcpData += c;
    }
    if (tcpData == "keepalive") {
      Serial.println("Keepalive");
      ledMatrix.setText("Active");
      for (int x = 0; x < 100; x++) {
        engageText();
      }
      ledMatrix.setText("");
      ledMatrix.drawText();
      ledMatrix.commit();
    }
    tcpData = "";

}
