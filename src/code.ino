#include <WiFi.h>
#include "ThingSpeak.h"

const int trigPin = 5;
const int echoPin = 18;
const int ledPin = 19;

const char* ssid = "Galaxy Z Fold3 5G"; // Ganti dengan nama WiFi anda
const char* password = "qwertyui"; // Password WiFi anda

// --- Pengaturan ThingSpeak ---
unsigned long myChannelNumber = 3098571; // Ganti dengan Channel ID ThingSpeak anda
const char * myWriteAPIKey = "14R4TTTPBWE65Y5H"; // Ganti dengan Write API Key anda
WiFiClient client;

// Variabel untuk sensor
long duration;
int distance;

int thingSpeakCounter = 0;

void setup() {
  Serial.begin(115200); // di Serial Monitor Arduino, ubah variabel baud menjadi 115200

  // Inisialisasi pin-pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Menghubungkan ke WiFi
  Serial.print("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung!");

  // Inisialisasi ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // --- Mengukur Jarak dengan Sensor Ultrasonik ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // --- Tampilkan output ke terminal ---
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- Logika LED ---
  if (distance < 10 && distance > 0) {
    // Jika jarak berbahaya, kedipkan LED
    digitalWrite(ledPin, HIGH);
  } else {
    // Jika jarak aman, matikan LED
    digitalWrite(ledPin, LOW);
  }


  thingSpeakCounter++;

  // Kirim data setiap 30 kali loop (30 * 0.5 detik = 15 detik)
  if (thingSpeakCounter >= 30) {
    Serial.println("Mengirim data ke ThingSpeak...");
    int httpCode = ThingSpeak.writeField(myChannelNumber, 1, distance, myWriteAPIKey);

    if (httpCode == 200) {
      Serial.println("Data berhasil dikirim.");
    } else {
      Serial.println("Gagal mengirim data. Kode Error: " + String(httpCode));
    }
    
    // Reset counter ke 0 setelah mengirim
    thingSpeakCounter = 0; 
  }

  // --- Interval utama loop ---
  // Beri jeda 0,5 detik (500 milidetik)
  delay(500); 
}