#include <Servo.h>

Servo aimServo;
Servo shooterServo;

// Pins
#define S1 2
#define S2 3
#define KEY 4
#define PIR_PIN 7
#define MODE_BUTTON 8
#define SHOOTER_SERVO 6

int currentStateS1;
int lastStateS1;
int aimServoPos = 90;
int shooterServoPos = 0;
bool shooting = false;

enum Mode { MANUAL, AUTO };
Mode currentMode = MANUAL;

int autoDirection = 1;
unsigned long lastAutoMove = 0;
const unsigned long autoMoveInterval = 50;
unsigned long lastModeChange = 0;

int pirValue = 0;
bool lastPirState = false;
bool hasFiredAtCurrentTarget = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

void setup() {

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(KEY, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);
  pinMode(MODE_BUTTON, INPUT_PULLUP);

  aimServo.attach(9);
  shooterServo.attach(SHOOTER_SERVO);

  aimServo.write(aimServoPos);
  shooterServo.write(0);
 
  Serial.begin(9600);
  Serial.println("=== SENTRY GUN ===");
  Serial.println("MODE: MANUAL");
  Serial.println("==============================");

  delay(30000);
  Serial.println("System Ready");
}

void loop() {
  unsigned long currentMillis = millis();

  if (digitalRead(MODE_BUTTON) == LOW && currentMillis - lastModeChange > 500) {
    lastModeChange = currentMillis;
    currentMode = (currentMode == MANUAL) ? AUTO : MANUAL;
    
    if (currentMode == MANUAL) {
      Serial.println("MODE: MANUAL");
    } else {
      Serial.println("MODE: AUTO");
      hasFiredAtCurrentTarget = false;
    }
  }

  pirValue = digitalRead(PIR_PIN);

  if (pirValue == HIGH && !lastPirState && currentMillis - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentMillis;
    lastPirState = true;
    hasFiredAtCurrentTarget = false;
    Serial.println("<<<< MOTION DETECTED! >>>>>");

    if (currentMode == AUTO && !shooting && !hasFiredAtCurrentTarget) {
      Serial.println("Shooting!");
      shoot();
      hasFiredAtCurrentTarget = true;
    }
  } 
  else if (pirValue == LOW && lastPirState && currentMillis - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentMillis;
    lastPirState = false;
    hasFiredAtCurrentTarget = false;
  }

  if (currentMode == MANUAL) {
    manualMode();
  } else {
    autoMode();
  }
  
  lastStateS1 = currentStateS1;
}

void manualMode() {
  currentStateS1 = digitalRead(S1);
  if (currentStateS1 != lastStateS1 && currentStateS1 == LOW) {
    if (digitalRead(S2) != currentStateS1) {
      aimServoPos += 5;
    } else {
      aimServoPos -= 5;
    }
    aimServoPos = constrain(aimServoPos, 0, 180);
    aimServo.write(aimServoPos);
    
    // Only print position occasionally to reduce spam
    // static unsigned long lastPosPrint = 0;
    // if (millis() - lastPosPrint > 500) {
    //   Serial.print("Aim position: ");
    //   Serial.println(aimServoPos);
    //   lastPosPrint = millis();
    // }
  }

  if (digitalRead(KEY) == LOW && !shooting) {
    shoot();
  }
}

void autoMode() {
  if (millis() - lastAutoMove >= autoMoveInterval) {
    lastAutoMove = millis();
    
    aimServoPos += autoDirection;
    aimServo.write(aimServoPos);
    if (aimServoPos >= 180 || aimServoPos <= 0) {
      autoDirection *= -1;
    }
  }
  
  if (digitalRead(KEY) == LOW && !shooting && lastPirState) {
    Serial.println(">>> MANUAL FIRE AT TARGET!");
    shoot();
  } else if (digitalRead(KEY) == LOW && !shooting && !lastPirState) {
    Serial.println("cannot fire");
    delay(300);
  }
}

void shoot() {
  shooting = true;
  Serial.println("<<< SHOOTING! >>>");

  for (int pos = 0; pos <= 50; pos += 5) {
    shooterServo.write(pos);
    delay(15);
  }
  
  delay(100);

  for (int pos = 50; pos >= 0; pos -= 5) {
    shooterServo.write(pos);
    delay(15);
  }
  
  shooting = false;
}
