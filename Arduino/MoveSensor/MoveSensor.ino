#define PIR_PIN 14  // GPIO où est connecté le capteur PIR

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);  // Définir le PIR comme entrée
}

void loop() {
  int pirState = digitalRead(PIR_PIN);  // Lire l'état du capteur

  if (pirState == HIGH) {
    Serial.println("👀 Mouvement détecté !");
  } else {
    Serial.println("😴 Aucun mouvement...");
  }

  delay(1000);  // Attendre un peu pour éviter le spam
}
