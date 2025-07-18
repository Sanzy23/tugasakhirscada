cp_#include <ModbusRTU.h>
#include <DHT.h>

ModbusRTU mb;

#define SLAVE_ID  1
#define DE_RE_PIN 4
// Sensor DHT
#define DHTPIN  5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Sensor MQ-2 (gas analog)
#define MQ_PIN 19

// Data register
uint16_t temp = 0, hum = 0, gasVal = 0;

// // Callback baca register (opsional)
// bool cb(Modbus::FunctionCode fc, uint16_t addr, uint16_t& data) {
//   Serial.print("[READ] FC: ");
//   Serial.print((uint8_t)fc);
//   Serial.print(" | Address: ");
//   Serial.print(addr);
//   Serial.print(" | Value: ");
//   Serial.println(data);
//   return true;
// }

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
  Serial.println("ESP32 Modbus RTU Slave dengan DHT & MQ-2");

  // RS485 Modbus
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  mb.begin(&Serial2, DE_RE_PIN);
  mb.slave(SLAVE_ID);

  // Tambahkan Holding Register
  mb.addHreg(0x0000); // Temp
  mb.addHreg(0x0001); // Hum
  // mb.addHreg(0x0002); // Gas Value

  // Inisialisasi sensor
  dht.begin();
}

void loop() {
  // bacaSensor();
  float temp = dht.readTemperature(); // Celsius
  float hum  = dht.readHumidity();

  // Validasi pembacaan
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Baca sensor gas (MQ-2)
  // int gas = analogRead(MQ_PIN); // Nilai ADC 0–4095

  // int gasVal = gas * 100 / 4095;
  
  // Konversi ke format Modbus (x10 misalnya)
  uint16_t t      = (uint16_t)(temp * 10);
  uint16_t h      = (uint16_t)(hum * 10);
  // uint16_t gVal   = (uint16_t)(gasVal * 10);

  // Update register
  mb.Hreg(0x0000, temp);
  mb.Hreg(0x0001, hum);
  // mb.Hreg(0x0002, gVal);

  // Modbus task
  mb.task();

  // Debug
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" °C | ");
  Serial.print("Hum: "); Serial.print(hum); Serial.println(" %");
  // Serial.print("Gas: "); Serial.print(gasVal); Serial.println(" %");
  delay(2000);
}

// void kirimData() {
//   // Konversi ke format Modbus (x10 misalnya)
//   temp   = (uint16_t)(temp * 10);
//   hum    = (uint16_t)(hum * 10);
//   gasVal = (uint16_t)(gasVal * 10);

//   // Update register
//   mb.Hreg(0x0000, temp);
//   mb.Hreg(0x0001, hum);
//   mb.Hreg(0x0002, gasVal);

//   // Modbus task
//   mb.task();
// }

// void DebugSerial() {
//   // Debug
//   Serial.print("Temp: "); Serial.print(temp); Serial.print(" °C | ");
//   Serial.print("Hum: "); Serial.print(hum); Serial.print(" % | ");
//   Serial.print("Gas: "); Serial.print(gasVal); Serial.println(" %");
// } 