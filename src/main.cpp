#include <Servo.h>
#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x08
#define SERVO_1 D3
#define SERVO_2 D6

void doubleBeep();
void openBox();
void closeBox();
bool checkCode();
void resetCode();
int readNumpad();

Servo s1;
Servo s2;


// SERVO:
// 180 for open status
// 20 for close status
bool openStateBox = false;
const int CODE_LENGTH = 4;
int code[CODE_LENGTH] = {0, 1, 2, 3}; // Example code, change as needed
int enteredCode[CODE_LENGTH] = {0};
int inputIndex = 0;

void setup() {
  pinMode(D0, OUTPUT);
  Wire.begin(4,5);
  Serial.begin(115200);
}

void loop() {
  int key = readNumpad();

  if (key != 0xFF) {
    Serial.println(key);
    enteredCode[inputIndex++] = key;
    
    if (inputIndex == CODE_LENGTH) {
      if (checkCode()) {
        Serial.println("Code correct. Opening box.");
        openBox();
      } else {
        Serial.println("Incorrect code. Try again.");
      }
      resetCode();
    }
  }

  delay(100);

}

void doubleBeep() {
  digitalWrite(D0, HIGH);
  delay(50);
  digitalWrite(D0, LOW);
  delay(50);
  digitalWrite(D0, HIGH);
  delay(50);
  digitalWrite(D0, LOW);
}

int readNumpad() {
  Wire.requestFrom(SLAVE_ADDRESS, 1);

  if (Wire.available()) {
    int value = Wire.read();
    return value;
  }
  return 0xFF;
}

void openBox() {
  doubleBeep();

  s1.attach(SERVO_1);
  s2.attach(SERVO_2);

  while (s1.attached() == false || s2.attached() == false) {}
  
  int value = 180;
  int reverse = 180 - value;

  s1.write(value);
  s2.write(reverse);
  delay(1000);
  s1.detach();
  s2.detach();
  openStateBox = true;
}

void closeBox() {
  doubleBeep();

  s1.attach(SERVO_1);
  s2.attach(SERVO_2);

  while (s1.attached() == false || s2.attached() == false) {}
  
  int value = 10;
  int reverse = 180 - value;

  s1.write(value);
  s2.write(reverse);
  delay(1000);
  s1.detach();
  s2.detach();
  openStateBox = false;
}

bool checkCode() {
  for (int i = 0; i < CODE_LENGTH; i++) {
    if (enteredCode[i] != code[i]) {
      return false;
    }
  }
  return true;
}

void resetCode() {
  inputIndex = 0;
  for (int i = 0; i < CODE_LENGTH; i++) {
    enteredCode[i] = 0;
  }
}