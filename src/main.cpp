#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD Donanım (16x2)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

const int trigPin = 9;
const int echoPin = 10;
long sure;
int mesafe;

// --- MOTOR KONTROL KATMANI ---
// Bu kısımlar donanım bağlantısına göre (L298N DC motor sürücüye göre) doldurulacaktır.

void motorIleri() { 
  // TODO: İki motoru ileri yönde sürecek komutlar
}

void motorSag() { 
  // TODO: Eksenel sağa dönüş (Sağ teker geri, Sol teker ileri) komutlar
}

void motorDur() { 
  // TODO: Tüm motor sürücü pinlerini LOW (0) konumuna getirecek komutlar
}

void setup() {
  lcd.begin(16, 2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.print("IKA SISTEM HAZIR");
  delay(1000);
}

void loop() {
  // 1. MESAFE ÖLÇÜMÜ (Ses Hızı Hesabı Dahil)
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH);
  mesafe = sure * 0.034 / 2;

  // 2. LCD ÜST SATIR (Radar & Mesafe Verisi)
  lcd.setCursor(0, 0);
  lcd.print("D:"); lcd.print(mesafe); lcd.print("cm ");
  
  // Radar Barı (0-9 karakter arası dinamik dolum)
  int barGucu = map(constrain(mesafe, 5, 50), 5, 50, 9, 0); 
  lcd.setCursor(16 - barGucu, 0); 
  for (int i = 0; i < barGucu; i++) { lcd.print((char)255); }

  // 3. KARAR MEKANİZMASI (20cm - 30cm)
  if (mesafe < 20) {
    // --- ENGEL MODU ---
    lcd.setCursor(0, 1);
    lcd.print("! ENGEL VAR !  ");
    motorDur(); 
    delay(300); // Güvenli duruş için küçük bir bekleme
    
    // Yol 30 cm olana kadar manevra döngüsü (Akış Şeması Çıkış Koşulu)
    while (mesafe < 30) {
      motorSag(); 
      // Döngü içinde mesafe güncellemesi
      digitalWrite(trigPin, HIGH); delayMicroseconds(10); digitalWrite(trigPin, LOW);
      mesafe = pulseIn(echoPin, HIGH) * 0.034 / 2;
    }
  } else {
    // --- SERBEST SÜRÜŞ MODU ---
    lcd.setCursor(0, 1);
    lcd.print("YOL TEMIZ >>>   ");
    motorIleri(); 
  }
  
  delay(50); // İşlemci ferahlığı
}

