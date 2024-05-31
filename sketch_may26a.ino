#include <SoftwareSerial.h>

#define zil 13
#define RX 7          
#define TX 6  
// Ultrasonik sensör pinleri
const int trigPin = 9;
const int echoPin = 10;

// LED pinleri
const int redLEDPin = 12;
const int whiteLEDPin = 11;

// Bluetooth Modülü için SoftwareSerial Port Tanımlaması
SoftwareSerial bluetooth(RX, TX); // RX, TX

// Mesafe Eşiği (cm)
const float distanceThreshold = 10.0;

// LED'lerin açık olup olmadığını takip eden değişkenler
bool redLEDState = false;
bool whiteLEDState = false;

void setup() {
  // Seri iletişimi başlatma
  bluetooth.begin(9600);
  Serial.begin(9600);
  pinMode(zil, OUTPUT);
  

  // Ultrasonik sensör pinlerini ayarlama
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LED pinlerini çıkış olarak ayarlama
  pinMode(redLEDPin, OUTPUT);
  pinMode(whiteLEDPin, OUTPUT);

  // LED'leri başlangıçta kapalı hale getirme
  digitalWrite(redLEDPin, LOW);
  digitalWrite(whiteLEDPin, LOW);
}

void loop() {
  // Bluetooth üzerinden gelen veri olup olmadığını kontrol et
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');
    command.trim(); // Başında ve sonundaki boşlukları temizle

    if (command.equalsIgnoreCase("kapat")) {
      digitalWrite(redLEDPin, LOW);
      digitalWrite(whiteLEDPin, LOW);
      digitalWrite(zil,LOW);
      redLEDState = false;
      whiteLEDState = false;
      Serial.println("LED'ler kapatıldı.");
      delay(5000);

    }
  }

  // Mesafeyi ölçme
  long duration;
  float distance;

  // Trig pinine 10 mikrosaniye boyunca HIGH sinyali gönder
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Echo pininde yüksek süreyi ölçme
  duration = pulseIn(echoPin, HIGH);

  // Mesafeyi santimetre cinsine çevirme
  distance = (duration * 0.034) / 2;

  // Mesafe değerine göre LED'leri kontrol etme
  if (distance > distanceThreshold) {
    if (!whiteLEDState) {
      digitalWrite(whiteLEDPin, HIGH);
      digitalWrite(redLEDPin, LOW);
      whiteLEDState = true;
      redLEDState = false;
      digitalWrite(zil,LOW);
    }
  } else {
    if (!redLEDState) {
      digitalWrite(whiteLEDPin, LOW);
      digitalWrite(redLEDPin, HIGH);
      whiteLEDState = false;
      redLEDState = true;
      digitalWrite(zil,HIGH);
     
    }
  }

  // Okunan değerleri seri port ve bluetooth ile gönderme
  String data = "Mesafe: " + String(distance) + " cm";
  Serial.println(data);
 

  // Biraz bekle ve tekrar et
  delay(1000);
}