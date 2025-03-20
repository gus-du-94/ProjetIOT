#include <Wire.h>
#include <MFRC522.h>

#define SDA_PIN 21 // Broche SDA pour I2C sur ESP32
#define SCL_PIN 22 // Broche SCL pour I2C sur ESP32
#define RST_PIN 4  // Broche de réinitialisation du module RFID

MFRC522 mfrc522(SDA_PIN, RST_PIN); // Initialisation du module RFID

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  mfrc522.PCD_Init();
  Serial.println("Scanner RFID prêt.");
}

void loop() {
  // Vérifie la présence d'une carte RFID
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Carte détectée, UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  
  mfrc522.PICC_HaltA(); // Met fin à la communication avec la carte
}
