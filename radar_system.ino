#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

const int trigPin = 10;
const int echoPin = 11;
const int buzzerPin = 8;     
const int ledPin = 9;        
const int servoPin = 12;     

long duration;
int distance;

Servo myServo; 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
  myServo.attach(servoPin);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("RADAR STARTING");
  delay(2000);
  lcd.clear();
}

void loop() {
  for(int i = 15; i <= 165; i++){ updateRadar(i); }
  for(int i = 165; i > 15; i--){ updateRadar(i); }
}

void updateRadar(int angle) {
  myServo.write(angle);
  delay(50); 
  
  distance = calculateDistance();

  Serial.print(angle);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");

  // LCD Line 1
  lcd.setCursor(0, 0);
  lcd.print("ANG:"); lcd.print(angle); lcd.print(" DST:");
  if(distance > 50 || distance <= 0) lcd.print("--- "); 
  else { lcd.print(distance); lcd.print("cm "); }

  lcd.setCursor(0, 1);

  // 🔴 DANGER
  if (distance > 0 && distance <= 15) {
    lcd.print("STATE: DANGER   ");
    blinkLED(80, 1200);   // FIXED
  } 

  // 🟡 WARNING
  else if (distance <= 30) {
    lcd.print("STATE: WARNING  ");
    blinkLED(150, 800);
  }

  // 🟢 CAUTION
  else if (distance <= 50) {
    lcd.print("STATE: CAUTION  ");
    blinkLED(300, 500);   // FIXED
  }

  // ⚫ SAFE
  else {
    lcd.print("STATE: SAFE     ");
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }
}

// 🔥 Blink Function
void blinkLED(int delayTime, int freq) {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, freq);
  delay(delayTime);

  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
  delay(delayTime);
}

// Distance Calculation
int calculateDistance(){ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH, 25000); 
  distance = duration * 0.034 / 2;
  
  if (distance <= 0 || distance > 100) return 999;
  return distance;
}
