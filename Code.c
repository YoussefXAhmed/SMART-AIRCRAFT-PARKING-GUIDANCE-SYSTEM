//Youssef Elmahdi//
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------- PIN DEFINITIONS ----------------
#define LED_WHITE   4
#define LED_YELLOW  5
#define LED_RED     6
#define BUZZER      7
#define BUTTON      3      
#define TRIG        9
#define ECHO        10

// ---------------- OBJECTS ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- GLOBAL VARIABLES ----------------
volatile bool systemEnabled = true;
float distanceCM = 0;
unsigned long lastReadTime = 0;

// For blinking red LED
unsigned long lastBlinkTime = 0;
bool redState = false;

// ---------------- INTERRUPT FUNCTION ----------------
void emergencyISR() {
  systemEnabled = !systemEnabled;   // Toggle system ON / OFF
}

// ---------------- ULTRASONIC FUNCTION ----------------
float readDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);
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
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

// ---------------- LOOP ----------------
void loop() {

  // ---------------- SYSTEM OFF STATE ----------------
  if (!systemEnabled) {
    digitalWrite(LED_WHITE, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM OFF");
    lcd.setCursor(0, 1);
    lcd.print("Press Button");

    delay(300);
    return;
  }

  // ---------------- READ ULTRASONIC ----------------
  if (millis() - lastReadTime >= 100) {
    distanceCM = readDistance();
    lastReadTime = millis();
  }

  // Reset outputs
  digitalWrite(LED_WHITE, LOW);
  digitalWrite(LED_YELLOW, LOW);
  noTone(BUZZER);

  String status = "OUT";

  // ---------------- DISTANCE CONDITIONS ----------------
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
    // Blink red LED
    if (millis() - lastBlinkTime >= 300) { // change every 300ms
      redState = !redState;
      digitalWrite(LED_RED, redState);
      lastBlinkTime = millis();
    }
    // Strong buzzer pattern
    tone(BUZZER, 2000); // high frequency continuous
    status = "WARNING";
  }
  else if (distanceCM <= 5) {
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER, 2500); // very high frequency
    status = "STOP";
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

  delay(50); // smaller delay for smoother blinking
}
