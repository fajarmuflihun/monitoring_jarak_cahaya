
#include <WiFi.h>

// ==========================================
// MONITORING JARAK DAN CAHAYA BERBASIS ESP32
// SENSOR ULTRASONIC HC-SR04 + LDR
// ==========================================

// Konfigurasi WiFi
const char* ssid = "MR-FK-01";
const char* password = "adzanmasjid";

// Konfigurasi pin
#define TRIG_PIN 5
#define ECHO_PIN 18
#define LDR_PIN 34

// Batas jarak objek (cm)
const float batasJarak = 50.0;

// Interval pembacaan
const unsigned long intervalSensor = 1000;
const unsigned long intervalWiFi = 10000;

unsigned long waktuSensor = 0;
unsigned long waktuWiFi = 0;

// ==========================================
// FUNGSI KONEKSI WIFI
// ==========================================

void koneksiWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  unsigned long mulai = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - mulai < 20000) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi berhasil terhubung!");
    Serial.print("SSID       : ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI       : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("WiFi gagal terhubung!");
    Serial.println("Sistem berjalan offline.");
  }
}

// ==========================================
// FUNGSI MEMBACA JARAK ULTRASONIC
// ==========================================

float bacaJarak() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  // Timeout 30 ms
  unsigned long durasi =
      pulseIn(ECHO_PIN, HIGH, 30000UL);

  // Tidak ada pantulan yang valid
  if (durasi == 0) {
    return -1;
  }

  // Menghitung jarak dalam cm
  float jarak = durasi * 0.0343 / 2.0;

  return jarak;
}

// ==========================================
// SETUP
// ==========================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.println();
  Serial.println("====================================");
  Serial.println(" SISTEM MONITORING JARAK DAN CAHAYA");
  Serial.println(" ESP32 + ULTRASONIC + LDR");
  Serial.println("====================================");

  koneksiWiFi();

  Serial.println();
  Serial.println("Sistem siap digunakan!");

  waktuSensor = millis();
  waktuWiFi = millis();
}

// ==========================================
// LOOP UTAMA
// ==========================================

void loop() {
  unsigned long sekarang = millis();

  // Cek koneksi WiFi setiap 10 detik
  if (sekarang - waktuWiFi >= intervalWiFi) {
    waktuWiFi = sekarang;

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi terputus, reconnect...");
      WiFi.reconnect();
    }
  }

  // Pembacaan sensor setiap 1 detik
  if (sekarang - waktuSensor >= intervalSensor) {
    waktuSensor = sekarang;

    float jarak = bacaJarak();
    int nilaiLDR = digitalRead(LDR_PIN);

    bool gelap = (nilaiLDR == HIGH);
    bool jarakValid = (jarak > 0);
    bool objekDekat =
        jarakValid && (jarak <= batasJarak);

    Serial.println();
    Serial.println("====================================");

    // Status WiFi
    Serial.print("WiFi   : ");
    Serial.println(
      WiFi.status() == WL_CONNECTED
        ? "Terhubung"
        : "Terputus"
    );

    // Status Ultrasonic
    Serial.print("JARAK  : ");

    if (jarakValid) {
      Serial.print(jarak, 1);
      Serial.println(" cm");
    } else {
      Serial.println("Tidak terbaca");
    }

    // Status LDR
    Serial.print("LDR    : ");
    Serial.println(gelap ? "Gelap" : "Terang");

    // Logika gabungan sensor
    Serial.print("STATUS : ");

    if (!jarakValid) {
      Serial.println("Pengukuran jarak gagal");
    }
    else if (objekDekat && gelap) {
      Serial.println(
        "PERINGATAN! Objek dekat saat gelap"
      );
    }
    else if (objekDekat && !gelap) {
      Serial.println(
        "Objek terdeteksi dalam jarak dekat"
      );
    }
    else if (!objekDekat && gelap) {
      Serial.println(
        "Ruangan gelap, tidak ada objek dekat"
      );
    }
    else {
      Serial.println("Kondisi normal");
    }

    Serial.println("====================================");
  }
}
