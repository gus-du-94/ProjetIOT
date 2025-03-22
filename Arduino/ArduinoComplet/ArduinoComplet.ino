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
#define GAS_SENSOR_ANALOG 33  // capteur gaz analog
#define BUZZER_PIN 25  // Buzzer
#define LED_COUNT 4 // Nb de LEDs
#define LED_PIN 26 // Leds
#define BUTTON 16 // boutton

DHT dht(DHTPIN, DHTTYPE);

const int analogPin = 34; // Capteur eau
MFRC522_I2C mfrc522(0x28, -1, &Wire);  // RFID
String password = ""; // RFID
bool start = false;
bool proximityDetector = false;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // LEDs
LiquidCrystal_PCF8574 lcd(0x27); // Adresse de l'écran LCD

const int buzzerPin = 25; 
unsigned long previousMillis = 0;
const long interval = 1000;

const int analogPinSteam = 34;

const int frequency = 300;

const int gasTrigger = 5000;
const int analogGasPin = 33;

bool isOpen = false;

const int ledYPin = 12;

boolean gaz1 = 1;
boolean gaz2 = 1;

void scanI2C() {
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      delay(10);
    }
  }
}

void alarm(float gasPercentage, int type) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (type == 0){
    lcd.print("Gaz detecte !");
    lcd.setCursor(0, 1);
    lcd.print("DANGER " + String(gasPercentage, 1) + " %");
  } else if (type == 1) {
    lcd.print("Mouvement");
    lcd.setCursor(0,1);
    lcd.print("detecte !");
  } else {
    return;
  }

  for (int i = 0; i < 4; i++) {
    for(int j = 0; j < LED_COUNT; j++) {
      strip.setPixelColor(j, strip.Color(255, 0, 0));
    }
    strip.show();
    
    tone(BUZZER_PIN, 300); 
    delay(500);
    noTone(BUZZER_PIN);
    
    for(int j = 0; j < LED_COUNT; j++) {
      strip.setPixelColor(j, strip.Color(0, 0, 0));
    }
    strip.show();
    
    delay(500);
  }
  noTone(BUZZER_PIN);
}

void open() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ouverture");
  motorRight.write(90);
  motorLeft.write(90);
}

void close() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("fermeture");
  motorRight.write(0);
  motorLeft.write(0);
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
  delay(500);

  lcd.begin(16, 2); // Initialise l'écran avec 16 colonnes et 2 lignes
  lcd.setBacklight(255);
  lcd.setCursor(0, 0); // LCD
  lcd.print("Demarrage..."); // LCD

  strip.setPixelColor(1, strip.Color(0, 5, 0));
  strip.show();
  delay(500);

  pinMode(BUZZER_PIN, OUTPUT); // buzzer
  pinMode(GAS_SENSOR_ANALOG, INPUT); // détecteur gazs
  pinMode(PIR_PIN, INPUT); // détecteur mouvements
  pinMode(ledYPin, OUTPUT); // Mono led jaune
  motorRight.attach(SERVO_PIN_R); // Moteur Droit
  motorLeft.attach(SERVO_PIN_L); // Moteur Gauche

  strip.setPixelColor(2, strip.Color(0, 5, 0));
  strip.show();
  delay(500);
  
  scanI2C(); // RFID
  mfrc522.PCD_Init(); // RFID

  pinMode(BUTTON, INPUT_PULLUP);

  strip.setPixelColor(3, strip.Color(0, 5, 0));
  strip.show();
  delay(500);

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
  unsigned long currentMillis = millis();

  if (buttonState == LOW) {  
    start = !start;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(start ? "" : "Arret");
    delay(300);
  }

  static unsigned long lastUpdate = 0;
  static bool showTempHum = true;

  if (start && (currentMillis - lastUpdate >= 4000)) {
    lastUpdate = currentMillis;
    float temperature = dht.readTemperature();

    strip.setPixelColor(1, strip.Color(15, 0, 0));
    strip.show();
    delay(250);

    float humidity = dht.readHumidity();

    strip.setPixelColor(2, strip.Color(15, 6, 0));
    strip.show();
    delay(250);

    int steamValue = analogRead(analogPinSteam);
    float steamPercentage = ((steamValue / 4095.0)) * 100.0;

    strip.setPixelColor(3, strip.Color(10, 15, 0));
    strip.show();
    delay(250);

    int gasValue = analogRead(analogGasPin);
    float gasPercentage = ((gasValue / 4095.0)) * 100;

    strip.setPixelColor(0, strip.Color(0, 15, 0));
    strip.show();
    delay(250);

    if (gasPercentage > 5) {
      alarm(gasPercentage, 0);
    } else {
      for(int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(0, 5, 0));
    }
    strip.show();
    }

    if (proximityDetector) {
      int pirState = digitalRead(PIR_PIN);
      if (pirState == HIGH) {
        alarm(1, 1);
      }
    }
  

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Erreur capteur température/humidité");
      return;
    }

    lcd.clear();
    if (showTempHum) {
      lcd.setCursor(0, 0);
      lcd.print("Temp : " + String(temperature) + " C ");
      lcd.setCursor(0, 1);
      lcd.print("Humi : " + String(humidity) + " %");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Vap : " + String(steamPercentage) + " %");
      lcd.setCursor(0, 1);
      lcd.print("Gaz : " + String(gasPercentage) + " %");
    }
    
    Serial.println("Température : " + String(temperature) + " °C");
    Serial.println("Humidité : " + String(humidity) + " %");
    Serial.println("Vapeur : " + String(steamPercentage) + " %");
    Serial.println("Gaz : " + String(gasPercentage) + " %");

    delay(100);

    showTempHum = !showTempHum;
  }

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  password = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    password += String(mfrc522.uid.uidByte[i]);
    if (i < mfrc522.uid.size - 1) {
      password += " ";
    }
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (password == "38 69 198 60") {
    lcd.print("Card Valid");
    if (isOpen) {
      close();
      isOpen = false;
      delay(200);
    } else{
      open();
      isOpen = true;
      delay(200);
    }
  } else if (password = "186 67 94 181") {
    if (proximityDetector) {
      proximityDetector = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("alarme mouvement");
      lcd.setCursor(0, 1);
      lcd.print("desactive");
      digitalWrite(ledYPin, LOW);
      delay(200);
    } else {
      proximityDetector = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("alarme mouvement");
      lcd.setCursor(0, 1);
      lcd.print("active");
      digitalWrite(ledYPin, HIGH);
      delay(200);
    }
  } else {
    lcd.print("Card Invalid");
  }
  delay(200);
}
