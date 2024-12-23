#include <Arduino.h>

// กำหนดขา ADC สำหรับ Potentiometer 4 ตัว
int potPins[] = {36, 39, 12, 13};
// กำหนดขา GPIO สำหรับ LED 8 ดวง (แบ่งเป็น 4 กลุ่ม)
int ledPins[][2] = {{23, 19}, {18, 5}, {17, 16}, {4, 0}};
// PWM Channels สำหรับ LED แต่ละดวง
int pwmChannels[] = {0, 1, 2, 3, 4, 5, 6, 7};
int pwmFrequency = 5000;  // ความถี่ PWM 5kHz
int pwmResolution = 8;    // ความละเอียด PWM 8 บิต (0-255)
// ขา Digital Input สำหรับ Switch 6 ตัว
int switchPins[] = {15, 2, 34, 35, 32, 33}; 

bool chasingMode = false;  // โหมด Chasing LED
bool systemEnabled = true; // ระบบเปิด/ปิด LED

void setup() {
  // ตั้งค่า PWM สำหรับ LED
  for (int group = 0; group < 4; group++) {
    for (int led = 0; led < 2; led++) {
      int pin = ledPins[group][led];
      int channel = pwmChannels[group * 2 + led];
      ledcAttachPin(pin, channel);
      ledcSetup(channel, pwmFrequency, pwmResolution);
    }
  }

  // ตั้งค่า Switch เป็น Input
  for (int i = 0; i < 6; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);
  }
}

void loop() {
  static bool lastSwitch5State = HIGH;
  static bool lastSwitch6State = HIGH;

  // ตรวจสอบ Switch ที่ 5 สำหรับสลับโหมดระหว่าง Potentiometer และ Chasing LED
  bool currentSwitch5State = digitalRead(switchPins[4]);
  if (lastSwitch5State == HIGH && currentSwitch5State == LOW) {
    chasingMode = !chasingMode; // สลับโหมด
    delay(200); // Debounce
  }
  lastSwitch5State = currentSwitch5State;

  // ตรวจสอบ Switch ที่ 6 สำหรับเปิด/ปิดระบบทั้งหมด
  bool currentSwitch6State = digitalRead(switchPins[5]);
  if (lastSwitch6State == HIGH && currentSwitch6State == LOW) {
    systemEnabled = !systemEnabled; // สลับสถานะระบบ
    delay(200); // Debounce
  }
  lastSwitch6State = currentSwitch6State;

  // หากระบบถูกปิด ให้ปิดไฟ LED ทั้งหมดและหยุดการทำงาน
  if (!systemEnabled) {
    for (int i = 0; i < 8; i++) {
      int channel = pwmChannels[i];
      ledcWrite(channel, 0); // ปิดไฟ LED ทุกดวง
    }
    return; // ออกจาก loop
  }

  // ระบบทำงานปกติ
  if (chasingMode) {
    // โหมด Chasing LED
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        int channel = pwmChannels[j];
        ledcWrite(channel, (j == i) ? 255 : 0);
      }
      delay(200); // หน่วงเวลาเพื่อให้เกิดเอฟเฟกต์วิ่ง
    }
  } else {
    // โหมดหรี่ไฟ (ควบคุมด้วย Potentiometer)
    for (int group = 0; group < 4; group++) {
      // ตรวจสอบสถานะของ Switch
      if (digitalRead(switchPins[group]) == LOW) {
        // Switch ปิด -> ปิด LED ทั้งสองดวงในกลุ่ม
        for (int led = 0; led < 2; led++) {
          int channel = pwmChannels[group * 2 + led];
          ledcWrite(channel, 0);
        }
      } else {
        // Switch เปิด -> ใช้งาน Potentiometer
        int potValue = analogRead(potPins[group]);
        int dutyCycle = map(potValue, 0, 4095, 0, 255);
        for (int led = 0; led < 2; led++) {
          int channel = pwmChannels[group * 2 + led];
          ledcWrite(channel, dutyCycle);
        }
      }
    }
  }

  // หน่วงเวลาเล็กน้อย
  delay(10);
}







