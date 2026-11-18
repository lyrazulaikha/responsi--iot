#include <WiFi.h>

// 1. Konfigurasi Jaringan Wi-Fi (Sesuaikan Hotspot Anda)
const char* ssid = "NAMA_WIFI_HOTSPOT";
const char* password = "PASSWORD_WIFI";

// 2. Pemetaan Pin Sesuai Rangkaian
const int PIN_PIR = 27;     // Sensor PIR (Pin OUT)
const int TRIG_PIN = 5;     // Sensor Ultrasonik (Pin TRIG)
const int ECHO_PIN = 18;    // Sensor Ultrasonik (Pin ECHO)

// Variabel Global
long durasi;
float jarak;
int statusGerak = LOW;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inisialisasi Pin Mode
  pinMode(PIN_PIR, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.println("\n==========================================");
  Serial.println("  SISTEM SMART PARKING BERBASIS ESP32     ");
  Serial.println("==========================================");

  // Proses Koneksi Wi-Fi
  Serial.print("Menghubungkan ke Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[BERHASIL] ESP32 Terhubung ke Jaringan!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("==========================================\n");
}

void loop() {
  // A. Pembacaan Sensor PIR (Deteksi Kehadiran / Gerak)
  statusGerak = digitalRead(PIN_PIR);

  // B. Pembacaan Sensor Ultrasonik HC-SR04 (Deteksi Jarak)
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  durasi = pulseIn(ECHO_PIN, HIGH, 30000); // Batas timeout 30 ms

  if (durasi > 0) {
    jarak = durasi * 0.034 / 2.0;
  } else {
    jarak = 50.0; // Nilai default area lapang (tidak ada halangan)
  }

  // C. Penentuan Status Slot Parkir
  String statusParkir;
  if (jarak > 0 && jarak <= 10.0) {
    statusParkir = "TERISI (Mobil Parkir)";
  } else {
    statusParkir = "KOSONG (Tersedia)";
  }

  // D. Tampilkan Telemetri di Serial Monitor
  Serial.println("---------- TELEMETRI PARKIR PINTAR ----------");
  Serial.print("Status Koneksi : Online (IP: ");
  Serial.print(WiFi.localIP());
  Serial.println(")");

  Serial.print("Sensor PIR     : ");
  if (statusGerak == HIGH) {
    Serial.println("Ada Aktivitas / Gerakan Terdeteksi!");
  } else {
    Serial.println("Area Tenang (Tidak Ada Gerakan)");
  }

  Serial.print("Jarak Objek    : ");
  Serial.print(jarak, 1);
  Serial.println(" cm");

  Serial.print("Status Slot    : ");
  Serial.println(statusParkir);
  Serial.println("---------------------------------------------\n");

  delay(1000); // Interval update data setiap 1 detik
}