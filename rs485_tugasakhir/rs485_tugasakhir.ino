#include <ModbusRTU.h>
#include <DHT.h>

ModbusRTU mb;

#define SLAVE_ID  1
#define DE_RE_PIN 4
// Sensor DHT
#define DHTPIN  5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

enum HumiState { HUMI_LOW = 0, HUMI_MID, HUMI_HIGH };
enum TempState { TEMP_LOW = 0, TEMP_MID, TEMP_HIGH };

HumiState humiState;
TempState tempState;

bool cbRead(TRegister* reg, float val) {
  // Callback saat master membaca
  return true;
}

bool cbWrite(TRegister* reg, float val) {
  // Callback saat master menulis (tidak digunakan di sini)
  return true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("ESP32 Modbus RTU Slave dengan DHT11");

  // RS485 Modbus
  Serial2.begin(9600, SERIAL_8N1, 16, 17); //RX = 16, TX= 17. RX masuk RO dan TX masuk DO
  mb.begin(&Serial2, DE_RE_PIN);
  mb.slave(SLAVE_ID);

  mb.addHreg(0x0000); // humi low (0-40%)
  mb.addHreg(0x0001); // Humi mid (40-60%)
  mb.addHreg(0x0002); // humi high (60-100%)

  mb.addHreg(0x0003); // temp low (0-30 C) 
  mb.addHreg(0x0004); // temp mid (30-40 C)
  mb.addHreg(0x0005); // temp high (40-100 C)

  dht.begin();
}

void loop() {
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Gagal membaca dari sensor DHT11!");
    return;
  }

  if (hum <= 40) humiState = HUMI_LOW;
  else if (hum <= 60) humiState = HUMI_MID;
  else humiState = HUMI_HIGH;

  if (temp <= 30) tempState = TEMP_LOW;
  else if (temp <= 40) tempState = TEMP_MID;
  else tempState = TEMP_HIGH;

  mb.Hreg(0x0000, humiState == HUMI_LOW   ? 1 : 0);
  mb.Hreg(0x0001, humiState == HUMI_MID   ? 1 : 0);
  mb.Hreg(0x0002, humiState == HUMI_HIGH  ? 1 : 0);

  mb.Hreg(0x0003, tempState == TEMP_LOW   ? 1 : 0);
  mb.Hreg(0x0004, tempState == TEMP_MID   ? 1 : 0);
  mb.Hreg(0x0005, tempState == TEMP_HIGH  ? 1 : 0);

  mb.task();

  // Debug serial
  Serial.println("-------------------------------");
  Serial.print("Temperature : "); Serial.print(temp); Serial.println(" °C");
  Serial.print("Humidity    : "); Serial.print(hum); Serial.println(" %");
  Serial.print("Status Temp : "); Serial.println(tempState);
  Serial.print("Status Humi : "); Serial.println(humiState);
  Serial.println("-------------------------------");

  delay(2000);
}
