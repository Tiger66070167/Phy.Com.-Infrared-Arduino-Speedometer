#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MPU6050.h>
#include "Arduino_LED_Matrix.h"

const int trigPin = 3;
const int echoPin = 4;
const int buttonPin = 2;

LiquidCrystal_I2C lcd(0x27, 16, 2);

MPU6050 gyro;

ArduinoLEDMatrix matrix;

float firstDis, lastDis;
unsigned long startTime, endTime;
float firstAngle, lastAngle;
bool measuring = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  
  Wire.begin();
  gyro.initialize();
  if (!gyro.testConnection()) {
    matrix.loadFrame(LEDMATRIX_EMOJI_SAD);
    lcd.setCursor(0, 1);
    lcd.print("MPU6050 Error!");
    while (1);
  }else{
    matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
    lcd.setCursor(0, 1);
    lcd.print("Ready");
  }
}

void loop() {
  if (digitalRead(buttonPin) == LOW && !measuring) { //Pressed
    measuring = true;
    firstDis = getDistance();
    firstAngle = getAngle();
    startTime = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Measuring...");
    matrix.loadFrame(LEDMATRIX_HEART_BIG);

    delay(200);
  }

  if (digitalRead(buttonPin) == HIGH && measuring) { //Unpressed
    measuring = false;
    lastDis = getDistance();
    lastAngle = getAngle();
    endTime = millis();

    float measuringTime = (endTime - startTime) / 1000.0;
    float changeAngle = radians(lastAngle - firstAngle);
    float changeDis = sqrt( sq(lastDis*cos(changeAngle) - firstDis)+sq(lastDis*sin(changeAngle)) );
    float speed = changeDis/measuringTime;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    lcd.setCursor(0, 1);
    lcd.print(speed);
    lcd.print(" cm/s");
    matrix.loadFrame(LEDMATRIX_HEART_SMALL);

    delay(500);
  }
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

float getAngle() {
  return gyro.getRotationZ();
}
