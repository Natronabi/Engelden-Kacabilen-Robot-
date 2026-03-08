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
int manevra_sayaci = 0; 

// MOTOR KONTROL KATMANI
void motorIleriHizli(){
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, 200);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, 200);
}
void motorIleriYavas(){
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, 100);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, 100);
}
void motorSag(){
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, 150);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); analogWrite(ENB, 150);
}
void motorDur(){
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW); analogWrite(ENA, 0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); analogWrite(ENB, 0);
}
void motorGeri(){
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, 200);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, 200);
}

void mesafeOlcum() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH);
  mesafe = sure * 0.034 / 2;
}
void lcdMesafeGoster() {
  lcd.setCursor(0, 0);
  lcd.print("D:"); lcd.print(mesafe); lcd.print("cm ");
  int barGucu = map(constrain(mesafe, 5, 50), 5, 50, 9, 0); 
  lcd.setCursor(16 - barGucu, 0); 
  for (int i = 0; i < barGucu; i++) { lcd.print((char)255); }
}
void lcdUyariYazdir(String mesaj) {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(mesaj);
}

void engelKacinmaManevrasiniAktifEt() {
  lcdUyariYazdir("! ENGEL VAR !");
  while (mesafe < esik) {
    motorDur();
    delay(200);
    motorGeri();
    delay(300);
    motorSag();
    delay(300);
    motorDur(); 
    mesafeOlcum(); 
    manevra_sayaci++;
    if (manevra_sayaci >= 5) {
      lcdUyariYazdir("ENGEL ASILAMADI");
      motorDur();
      break; 
    }
  }
  manevra_sayaci = 0;
}
void hizKontrolModulu() {
  if (mesafe > 50) {
    motorIleriHizli();
    lcdUyariYazdir("YOL TEMIZ >>>");
  } else if (mesafe <= 50 && mesafe > 25) {
    motorIleriYavas();
    lcdUyariYazdir("YAVAS SURUS...");
  }
}

void setup() {
  lcd.begin(16, 2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("IKA SISTEM HAZIR");
  delay(1000);
  lcd.clear();
}

void loop() {
  mesafeOlcum();
  lcdMesafeGoster();
  if (mesafe < esik) {
    engelKacinmaManevrasiniAktifEt();
  } else {
    hizKontrolModulu();
  }
  delay(50);
}