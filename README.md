# 🔧 Proyek SCADA IoT – Monitoring Suhu, Kelembapan, dan Gas dengan 2 ESP32

Repositori ini berisi implementasi sistem SCADA berbasis IoT menggunakan **dua unit ESP32** yang bekerja secara terpisah:

- **ESP32 A** → Berfungsi sebagai **slave Modbus RTU (RS485)** untuk pembacaan sensor suhu & kelembapan dari DHT11  
- **ESP32 B** → Berfungsi sebagai **server Modbus TCP/IP (WiFi)** untuk pembacaan sensor gas (MQ-2)

Proyek ini ditujukan sebagai simulasi sistem pengawasan terdistribusi menggunakan protokol industri standar.
---

## 🧱 Arsitektur Sistem

```plaintext
[SENSOR DHT11] → ESP32 A (Modbus RTU Slave)
                                      ↘ RS485
                                    [SCADA/HMI]

[SENSOR MQ-2]  → ESP32 B (Modbus TCP Server)
                                      ↘ WiFi
                                    [SCADA/HMI]
````

---

## 📂 Struktur Proyek

```
tugasakhirscada/
├── rs485_tugasakhir/
│   └── rs485_tugasakhir.ino   # ESP32 A - RTU Slave
├── tcp_tugasakhir/
│   └── tcp_tugasakhir.ino     # ESP32 B - TCP Server
└── README.md
```

---

## ⚙️ Spesifikasi

### 1️⃣ ESP32 A – RTU Slave

* Sensor: **DHT11** (GPIO 5)
* Komunikasi: **RS485** (Serial2: TX=17, RX=16)
* ID Slave: `1`
* Holding Register:

  * `0x0000` → Suhu (°C ×10)
  * `0x0001` → Kelembapan (% ×10)

### 2️⃣ ESP32 B – TCP Server

* Sensor: **MQ-2** (GPIO 17)
* Koneksi: **WiFi**
* IP lokal diberikan oleh DHCP
* Holding Register:

  * `0x0002` → Gas (ADC ×10)

---

## 🔌 Instalasi dan Penggunaan

### 🔧 ESP32 A (Modbus RTU)

1. Sambungkan DHT11 ke pin GPIO 5
2. Hubungkan RS485 ke GPIO 16 (RX) dan GPIO 17 (TX)
3. Unggah `rs485_tugasakhir.ino`
4. Gunakan software `modbuspoll` untuk membaca data dari ID slave `1`

### 🌐 ESP32 B (Modbus TCP/IP)

1. Sambungkan MQ-2 ke GPIO 17
2. Ubah SSID dan password WiFi pada bagian berikut:
   ```cpp
   const char* ssid     = "ssid mu";
   const char* password = "password mu";
   ```
3. Unggah `tcp_tugasakhir.ino`
4. Gunakan software `modbuspoll.exe` untuk membaca data dari IP yang ditampilkan di serial monitor

---

## 🖥️ Output Serial (Contoh)

```text
ESP32 Modbus RTU Slave dengan DHT & MQ-2
Temp: 28.5 °C | Hum: 62.0 %
Gas Value : 1372.0 %
```

---

## 📊 Tabel Register

| Register | Parameter  | ESP32   | Satuan |
| -------- | ---------- | ------- | ------ |
| 0x0000   | Suhu       | RTU (A) | °C ×10 |
| 0x0001   | Kelembapan | RTU (A) | % ×10  |
| 0x0002   | Gas        | TCP (B) | % ×10  |

---

## 📚 Library yang Digunakan

* [`ModbusRTU`](https://github.com/esp8266/ModbusRTU)
* [`ModbusIP_ESP8266`](https://github.com/emelianov/arduino-modbus)
* `DHT`
* `WiFi` / `ESP8266WiFi`

---

## 🤝 Kontribusi

Pull request terbuka untuk penambahan fitur atau perbaikan bug. Silakan buat issue jika ada saran atau masalah.

---

## 👤 Penulis

**Sanzy23**
[GitHub – @Sanzy23](https://github.com/Sanzy23)

---

## 📝 Lisensi

MIT License. Bebas digunakan untuk proyek pembelajaran maupun pengembangan lebih lanjut.

```
Silakan salin langsung ke file `README.md` di root repositorimu. Jika kamu ingin saya kirimkan versi `.md` atau menambahkan diagram visual, logo, atau dokumentasi wiring, tinggal bilang ya!
```
---
