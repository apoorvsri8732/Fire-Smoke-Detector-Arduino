#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int mq2Pin = A0;    // MQ2 sensor analog pin
const int flamePin = 8;   // Flame sensor digital pin
const int buzzerPin = 9;  // Buzzer pin

// Thresholds
const int smokeThreshold = 500;
const int flameThreshold = LOW;

// Buzzer timing
const unsigned long beepDuration = 200;
const unsigned long pauseDuration = 300;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(flamePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  lcd.init();
  lcd.backlight();

  // Intro screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Fire And Smoke ");
  lcd.setCursor(0, 1);
  lcd.print(" Detection sys  ");
  delay(3000);

  lcd.clear();
}

void loop() {
  int smokeLevel = analogRead(mq2Pin);
  int flameState = digitalRead(flamePin);

  bool smokeDetected = smokeLevel > smokeThreshold;
  bool flameDetected = (flameState == flameThreshold);

  // Debug output
  Serial.print("Smoke Level: ");
  Serial.print(smokeLevel);
  Serial.print(" | Flame Sensor: ");
  Serial.println(flameDetected ? "Flame Detected" : "No Flame");

  // LCD display
  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(smokeLevel);
  lcd.print(smokeDetected ? " ALERT " : " Normal");

  lcd.setCursor(0, 1);
  if (flameDetected) {
    lcd.print("Flame: DETECTED ");
  } else {
    lcd.print("Flame: No Flame ");
  }

  // Buzzer alert
  if (smokeDetected || flameDetected) {
    digitalWrite(buzzerPin, HIGH);
    delay(beepDuration);
    digitalWrite(buzzerPin, LOW);
    delay(pauseDuration);
  } else {
    digitalWrite(buzzerPin, LOW);
    delay(500);
  }
}
