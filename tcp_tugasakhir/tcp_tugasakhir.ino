#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

#include <ModbusIP_ESP8266.h>

#define MQ2_PIN 5   
#define GAS_MAX 100

const char* ssid     = "ssid mu";
const char* password = "password mu";

ModbusIP mb;

enum GasStatus {
  GAS_LOW = 0,
  GAS_MEDIUM,
  GAS_HIGH
};

GasStatus gasStatus = GAS_LOW;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected. IP address:");
  Serial.println(WiFi.localIP());

  mb.server();     // Inisialisasi sebagai server TCP
  mb.addHreg(0);   // Register untuk low gas
  mb.addHreg(1);   // Register untuk mid gas
  mb.addHreg(2);   // Register untuk high gas
}

void loop() {
  int mq2ADC = analogRead(MQ2_PIN);              
  int gasPercent = map(mq2ADC, 0, 4095, 0, 100);

  Serial.print("Gas Value: ");
  Serial.print(gasPercent);
  Serial.println(" %");

  updateGasStatus(gasPercent);
  updateModbusRegisters();
  mb.task();

  delay(2000);
}

void updateGasStatus(int val) {
  if (val <= 30) {
    gasStatus = GAS_LOW;
    Serial.println("Status Gas: Normal");
  } else if (val <= 60) {
    gasStatus = GAS_MEDIUM;
    Serial.println("Status Gas: Gas Bocor Ringan");
  } else {
    gasStatus = GAS_HIGH;
    Serial.println("Status Gas: Gas Bocor Berat");
  }
}

void updateModbusRegisters() {
  mb.Hreg(0, gasStatus == GAS_LOW    ? 1 : 0);
  mb.Hreg(1, gasStatus == GAS_MEDIUM ? 1 : 0);
  mb.Hreg(2, gasStatus == GAS_HIGH   ? 1 : 0);
}
