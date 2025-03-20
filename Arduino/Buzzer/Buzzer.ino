const int buzzerPin = 25; 

const int frequency = 1000; 
const int duration = 500;

void setup() {
  pinMode(buzzerPin, OUTPUT);

  tone(buzzerPin, frequency);
  delay(duration);
  
  noTone(buzzerPin);
  delay(duration);
}

void loop() {
  
}