// Définition de la broche de la LED
const int ledPin = 12; // Assurez-vous que cette broche supporte le PWM

// Définition des paramètres de la variation de luminosité
const int fadeDelay = 10; // Temps entre chaque incrément (ms)

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Augmentation progressive de la luminosité
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(ledPin, brightness);
    delay(fadeDelay);
  }

  // Diminution progressive de la luminosité
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(ledPin, brightness);
    delay(fadeDelay);
  }
}
