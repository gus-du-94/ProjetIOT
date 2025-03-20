#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MFRC522_I2C.h"

// Initialisation des périphériques I2C
LiquidCrystal_I2C mylcd(0x27, 16, 2);
MFRC522_I2C mfrc522(0x28, -1, &Wire);  // Utilisation correcte avec ESP32

String password = "";

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

  // Scan I2C pour vérifier les connexions
  scanI2C();

  // Initialisation de l'écran LCD
  mylcd.init();
  mylcd.backlight();
  mylcd.setCursor(0, 0);
  mylcd.print("Scan Card");

  // Initialisation du module RFID
  mfrc522.PCD_Init();
  delay(100);  // Attendre après l'initialisation pour éviter les erreurs
}

void loop() {
  // Vérifie s'il y a une nouvelle carte
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Récupère l'UID de la carte sous le bon format
  password = "";
  Serial.print("Card UID: ");
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    password += String(mfrc522.uid.uidByte[i]);  // Ajout de chaque byte
    if (i < mfrc522.uid.size - 1) {
      password += " ";  // Ajout d'un espace sauf pour le dernier élément
    }
  }
  
  Serial.println(password);  // Affichage correct de l'UID

  // Vérification de la carte
  mylcd.clear();
  mylcd.setCursor(0, 0);
  
  if (password == "176 22 18 152" || password == "38 69 198 60" || password == "148 227 121 124" || password == "186 67 94 181") {  // Comparaison au bon format
    mylcd.print("Card Valid");
    Serial.println("✅ Carte correcte");
  } else {
    mylcd.print("Card Invalid");
    Serial.println("❌ Carte incorrecte");
  }
}
