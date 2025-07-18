# Tugas Akhir SCADA - Monitoring Suhu, Kelembapan, dan Gas Berbasis Modbus RTU & TCP/IP

Proyek ini merupakan implementasi sistem SCADA (Supervisory Control and Data Acquisition) berbasis ESP32 yang mampu membaca sensor suhu, kelembapan, dan gas, kemudian mengirimkan datanya melalui dua metode komunikasi: **Modbus RTU (RS485)** dan **Modbus TCP/IP (WiFi)**.

## 🔧 Fitur Utama

- Pembacaan data sensor:
  - DHT11 (suhu & kelembapan)
  - MQ-2 (kadar gas)
- Komunikasi Modbus RTU (Serial RS485) sebagai *slave*
- Komunikasi Modbus TCP/IP (WiFi) sebagai *server*
- Output data dalam satuan standar dan dikonversi ke register Modbus Holding Register
- Kompatibel dengan SCADA HMI (seperti Node-RED, SCADA software, atau PLC yang support Modbus)

## 📁 Struktur File
```text
├── rs485_tugasakhir/rs485_tugasakhir.ino # Kode ESP32 sebagai slave Modbus RTU
├── tcp_tugasakhir/tcp_tugasakhir.ino # Kode ESP32 sebagai server Modbus TCP
```


## 🧰 Teknologi & Library

- [ESP32](https://www.espressif.com/en/products/socs/esp32)
- Library:
  - `ModbusRTU.h`
  - `ModbusIP_ESP8266.h`
  - `DHT.h`
  - `WiFi.h` / `ESP8266WiFi.h`

## ⚙️ Instalasi dan Penggunaan

### Persyaratan

- Board ESP32
- Arduino IDE atau PlatformIO
- Sensor:
  - DHT11 di pin GPIO 5
  - MQ-2 di pin GPIO 19 (Modbus RTU) dan GPIO 17 (Modbus TCP)
- Koneksi RS485 (untuk Modbus RTU)
- Jaringan WiFi (untuk Modbus TCP)

### Cara Menggunakan

#### 1. Modbus RTU (Slave)
1. Unggah file `modbus_rtu_slave.ino` ke ESP32.
2. Hubungkan RS485 ke pin GPIO 16 (RX) dan GPIO 17 (TX).
3. Jalankan Modbus Master dari PC/PLC dengan ID `1`, dan baca Holding Register:
   - `0x0000`: Suhu (x10)
   - `0x0001`: Kelembapan (x10)

#### 2. Modbus TCP/IP (Server)
1. Unggah file `modbus_tcp_server.ino` ke ESP32.
2. Ubah `ssid` dan `password` sesuai jaringan WiFi kamu.
3. Baca Holding Register:
   - `0x0002`: Nilai gas (x10)

### Output Serial Contoh
```text
Temp: 28.5 °C | Hum: 62.0 %
Gas Value : 60.0 %
```

## 🔌 Contoh Konfigurasi di SCADA

| Register | Deskripsi     | Format  |
|----------|---------------|---------|
| 0x0000   | Suhu (°C × 10) | UINT16  |
| 0x0001   | Kelembapan (%×10) | UINT16 |
| 0x0002   | Kadar Gas (×10) | UINT16 |

## 🤝 Kontribusi

Pull request sangat diterima. Untuk perubahan besar, mohon buka issue terlebih dahulu agar bisa didiskusikan.

---
