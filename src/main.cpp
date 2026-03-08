#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD Donanım (16x2) Bağlantıları (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); 

// HC-SR04 Sensör Bağlantıları
const int trigPin = 12; // PB4
const int echoPin = 13; // PB5

// L298N Motor Sürücü Bağlantıları
const int IN1 = 9;  // PB1 (Sol İleri)
const int IN2 = 8;  // PB0 (Sol Geri)
const int ENA = 10; // PB2 (Sol Motor Hız - PWM)

const int IN3 = 7;  // PD7 (Sağ İleri)
const int IN4 = 6;  // PD6 (Sağ Geri)
const int ENB = 5;  // PD5 (Sağ Motor Hız - PWM)

long sure;
int mesafe;
int esik = 25;

// ŞURZAN GÖREV 1: Mesafe ölçüm fonksiyonu
void mesafeOlcum() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH);
  mesafe = sure * 0.034 / 2;
}

// ŞURZAN GÖREV 3: LCD mesafe fonksiyonu
void lcdMesafeGoster() {
  lcd.setCursor(0, 0);
  lcd.print("D:"); lcd.print(mesafe); lcd.print("cm ");
  
  // Radar Barı (Eski koddaki görsel yapı)
  int barGucu = map(constrain(mesafe, 5, 50), 5, 50, 9, 0); 
  lcd.setCursor(16 - barGucu, 0); 
  for (int i = 0; i < barGucu; i++) { lcd.print((char)255); }
}

// ŞURZAN GÖREV 2: LCD uyarı fonksiyonu
void lcdUyariYazdir(String mesaj) {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(mesaj);
}

// TUBA GÖREV 1: Manevra fonksiyonlarının oluşturulması
void engelKacinmaManevrasiniAktifEt() {
  // TUBA: Buraya akış şemasına göre motorDur, motorGeri ve motorSag fonksiyonlarını/kodlarını ekle.
  lcdUyariYazdir("! ENGEL VAR !");
}

// TUBA GÖREV 2: Hız kontrol fonksiyonlarının oluşturulması
void hizKontrolModulu() {
  // TUBA: Buraya mesafeye göre (mesafe > 50 ise hızlı, 25-50 arası yavaş) motorIleri kodlarını ekle.
  if (mesafe > 50) {
    lcdUyariYazdir("YOL TEMIZ >>>");
  } else if (mesafe <= 50 && mesafe > 25) {
    lcdUyariYazdir("YAVAS SURUS...");
  }
}

void setup() {
  // LCD Başlatma
  lcd.begin(16, 2);
  
  // Sensör Pin Ayarları
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Motor Pin Ayarları
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Açılış Ekranı
  lcd.setCursor(0,0);
  lcd.print("IKA SISTEM HAZIR");
  delay(1000);
  lcd.clear();
}

void loop() {
  // 1. MESAFE ÖLÇÜMÜ
  mesafeOlcum();

  // 2. LCD GÖSTERİMİ
  lcdMesafeGoster();

  // 3. KARAR MEKANİZMASI (Tuba'nın fonksiyonları çağrılıyor)
  if (mesafe < esik) {
    engelKacinmaManevrasiniAktifEt();
  } else {
    hizKontrolModulu();
  }

  delay(50); // İşlemci ferahlığı
}