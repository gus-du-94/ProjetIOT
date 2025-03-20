#include <ESP32Servo.h>

Servo motorRight;

#define SERVO_PIN 13

void setup() {
  motorRight.attach(SERVO_PIN);
  Serial.begin(115200);
}

void loop() {
  Serial.println("➡ Rotation à 0°");
  motorRight.write(0);
  delay(2000);        

  Serial.println("➡ Rotation à 90°");
  motorRight.write(90);
  delay(2000);         
}
