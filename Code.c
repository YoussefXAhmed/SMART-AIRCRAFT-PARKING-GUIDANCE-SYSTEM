#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- PIN DEFINITIONS ----------------
#define LED_WHITE 2
#define LED_YELLOW 3
#define LED_RED 4
#define BUZZER 6
#define BUTTON 7
#define TRIG 9
#define ECHO 10

// ---------------- OBJECTS ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- GLOBAL VARIABLES ----------------
volatile bool emergencyStop = false;
float distanceCM = 0;
unsigned long lastReadTime = 0;

// ---------------- INTERRUPT FUNCTION ----------------
void emergencyISR() {
  emergencyStop = true;
}

// ---------------- ULTRASONIC FUNCTION ----------------
float readDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 999;

  return (duration * 0.0343) / 2.0;
}

// ---------------- SETUP ----------------
void setup() {
  pinMode(LED_WHITE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON), emergencyISR, FALLING);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Auto Parking");
  delay(1500);
  lcd.clear();
}

// ---------------- LOOP ----------------
void loop() {

  // Read ultrasonic every 100ms
  if (millis() - lastReadTime >= 100) {
    distanceCM = readDistance();
    lastReadTime = millis();
  }

  // Reset outputs
  digitalWrite(LED_WHITE, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  noTone(BUZZER);

  String status = "OUT";

  // ---------------- EMERGENCY STOP ----------------
  if (emergencyStop) {
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER, 2000);
    status = "STOP";
  }
  else {

    // ----------- DISTANCE CONDITIONS -----------
    if (distanceCM > 30) {
      status = "OUT";
    }
    else if (distanceCM <= 30 && distanceCM > 20) {
      digitalWrite(LED_WHITE, HIGH);
      status = "CONTINUE";
    }
    else if (distanceCM <= 20 && distanceCM > 10) {
      digitalWrite(LED_YELLOW, HIGH);
      status = "SLOW";
    }
    else if (distanceCM <= 10 && distanceCM > 5) {
      digitalWrite(LED_RED, HIGH);
      tone(BUZZER, 1500);
      status = "WARNING";
    }
    else if (distanceCM <= 5) {
      digitalWrite(LED_RED, HIGH);
      tone(BUZZER, 2500);
      status = "STOP";
    }
  }

  // ---------------- LCD DISPLAY ----------------
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print((int)distanceCM);
  lcd.print(" cm");

  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  lcd.print(status);

  delay(100);
}