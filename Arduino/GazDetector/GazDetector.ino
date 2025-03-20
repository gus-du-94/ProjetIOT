#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define GAS_SENSOR_DIGITAL 23  // Sortie numérique du capteur (D0)
#define BUZZER_PIN         25  // Buzzer
#define GAS_SENSOR_ANALOG 34

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialisation du LCD à l'adresse 0x27

boolean i = 1;
boolean j = 1;

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GAS_SENSOR_DIGITAL, INPUT);
  pinMode(GAS_SENSOR_ANALOG, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Systeme OK");
  lcd.setCursor(0, 1);
  lcd.print("✅ Secure");

  Serial.println("Système démarré | État : Sécurisé");
  delay(2000);
  lcd.clear();
}

void loop() {
  boolean gasDetected = digitalRead(GAS_SENSOR_DIGITAL);  // Lecture numérique
  int gasLevel = analogRead(GAS_SENSOR_ANALOG);           // Lecture analogique (0-4095)
  float voltage = gasLevel * (3.3 / 4095.0);              // Conversion en tension (0-3.3V)

  Serial.print("💨 Taux de gaz (ADC) : ");
  Serial.print(gasLevel);
  Serial.print(" | Tension : ");
  Serial.print(voltage, 2);
  Serial.println(" V");

  lcd.setCursor(0, 0);
  lcd.print("Gaz:");
  lcd.print(gasLevel);
  lcd.print("   ");  // Effacer les chiffres en trop

  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(voltage, 2);
  lcd.print("V ");  // Effacer les chiffres en trop

  if (gasDetected == 0) {  // Gaz détecté
    if (i == 1) {
      Serial.println("⚠️ DANGER : Gaz détecté !");
      lcd.setCursor(10, 0);
      lcd.print("⚠️");
      lcd.setCursor(10, 1);
      lcd.print("DANGER ");
      i = 0;
      j = 1;
    }
    // Alarme sonore
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  } 
  else {  // Pas de gaz
    digitalWrite(BUZZER_PIN, LOW);
    if (j == 1) {
      Serial.println("✅ Sécurité : Aucun gaz détecté.");
      lcd.setCursor(10, 0);
      lcd.print("  ");
      lcd.setCursor(10, 1);
      lcd.print("Secure  ");
      i = 1;
      j = 0;
    }
  }

  delay(1000); // Rafraîchir toutes les secondes
}
