#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else
 #include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>

#define mq2ana 17

const char* ssid     = "ssid mu";
const char* password = "password mu";

ModbusIP mb;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());

  mb.server();          // Set as TCP server
  mb.addHreg(2);        // Register 0 (e.g. temperature)
}

void loop() {
  float mq2val = analogRead(mq2ana);

  uint16_t gasval = (uint16_t)(mq2val * 10);

  Serial.print("Gas Value : "); Serial.print(mq2val); Serial.println(" %");

  mb.Hreg(2, gasval);

  mb.task();

  delay(2000);
}
