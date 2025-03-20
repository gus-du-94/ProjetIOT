#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 17
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("Démmarage du capteur température/humidité...");
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();  

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erreur lors de la récupération des données température/humidité");
    return;
  }
  Serial.print("🌡 Température : ");
  Serial.print(temperature);
  Serial.print(" °C | 💧 Humidité : ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}
