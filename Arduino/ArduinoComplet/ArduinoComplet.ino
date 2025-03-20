//Gestion ESP 32 collecte donée atmosphérique 
#include <Wire.h>
#include <ESP32Servo.h>
#include "MFRC522_I2C.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_PCF8574.h>

Servo motorRight;
Servo motorLeft;

#define DHTPIN 17 //capteur température/humidité
#define DHTTYPE DHT11 //capteur température/humidité
#define PIR_PIN 14 // Détecteur de mouvements 
#define SERVO_PIN_R 13 // Moteur 90 droit
#define SERVO_PIN_L 5 // Moteur 90 gauche 
#define GAS_SENSOR_DIGITAL 23  // Capteur gazs
#define BUZZER_PIN 25  // Buzzer
#define LED_COUNT 4 // Nb de LEDs
#define LED_PIN 26 // Leds
#define BUTTON 16 // boutton

DHT dht(DHTPIN, DHTTYPE);

const int analogPin = 34; // Capteur eau
MFRC522_I2C mfrc522(0x28, -1, &Wire);  // RFID
String password = ""; // RFID
bool start = false;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // LEDs
LiquidCrystal_PCF8574 lcd(0x27); // Adresse de l'écran LCD

const int buzzerPin = 25; 
unsigned long previousMillis = 0;
const long interval = 1000;

const int analogPinSteam = 34;

const int frequency = 300;

boolean gaz1 = 1;
boolean gaz2 = 1;

void scanI2C() {
  Serial.println("🔍 Scan I2C...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("📡 I2C device found at address 0x");
      Serial.println(addr, HEX);
      delay(10);
    }
  }
  Serial.println("🔎 Scan terminé.\n");
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht.begin();


  strip.begin();
  for(int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(5, 0, 0));
    }
  strip.show();

  lcd.begin(16, 2); // Initialise l'écran avec 16 colonnes et 2 lignes
  lcd.setBacklight(255);
  lcd.setCursor(0, 0); // LCD
  lcd.print("Demarrage..."); // LCD

  pinMode(BUZZER_PIN, OUTPUT); // buzzer
  pinMode(GAS_SENSOR_DIGITAL, INPUT); // détecteur gazs
  pinMode(PIR_PIN, INPUT); // détecteur mouvements
  motorRight.attach(SERVO_PIN_R); // Moteur Droit
  motorLeft.attach(SERVO_PIN_L); // Moteur Gauche
  
  scanI2C(); // RFID
  mfrc522.PCD_Init(); // RFID

  pinMode(BUTTON, INPUT_PULLUP);

  delay(6000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pret"); 
  for(int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(0, 5, 0));
    }
  strip.show();
}

void loop() {
  int buttonState = digitalRead(BUTTON);
  boolean gasDetected = digitalRead(GAS_SENSOR_DIGITAL);
  unsigned long currentMillis = millis();

  if (buttonState == LOW) {  
    start = !start;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(start ? "" : "Arret");
    delay(300);
  }

  if (gasDetected == 0) {
    if (gaz1 == 1) {
      Serial.println("⚠️ DANGER : Gaz détecté !");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gaz detecte !");
      lcd.setCursor(0, 1);
      lcd.print("DANGER");
      gaz1 = 0;
      gaz2 = 1;
      int i = 0; 
      while (i < 2) {
        tone(buzzerPin, frequency);
        delay(1000);
        noTone(buzzerPin);
        delay(500);
        i++;
      }
    }
  } else {
    noTone(buzzerPin);
    if (gaz2 == 1 && start) {
      Serial.println("✅ Sécurité : Aucun gaz détecté.");
      lcd.clear();
      gaz1 = 1;
      gaz2 = 0;
    }
  }

  static unsigned long lastUpdate = 0;
  static bool showTempHum = true;

  if (start && (currentMillis - lastUpdate >= 5000)) {
    lastUpdate = currentMillis;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int steamValue = analogRead(analogPinSteam);
    float steamPercentage = (steamValue / 4095.0) * 100.0;

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Erreur capteur température/humidité");
      return;
    }

    lcd.clear();
    if (showTempHum) {
      lcd.setCursor(0, 0);
      lcd.print("Temp : " + String(temperature) + " C ");
      lcd.setCursor(0, 1);
      lcd.print("Humi : " + String(humidity) + " % ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Vapeur : " + String(steamPercentage) + " % ");
    }
    
    Serial.println("🌡 Température : " + String(temperature) + " °C");
    Serial.println("💧 Humidité : " + String(humidity) + " %");
    Serial.println("💨 Vapeur : " + String(steamPercentage) + " %");

    showTempHum = !showTempHum;
  }
}




