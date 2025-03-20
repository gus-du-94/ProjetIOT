// Définition des broches
const int analogPin = 34; // Broche pour la sortie analogique du capteur

void setup() {
  Serial.begin(115200);
}

void loop() {
  int analogValue = analogRead(analogPin);
  Serial.print("Valeur analogique: ");
  Serial.println(analogValue);

  delay(1000);
}
