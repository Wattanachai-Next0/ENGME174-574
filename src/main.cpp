#include <Arduino.h>

#include <Arduino.h>

// กำหนด GPIO สำหรับ LED
const int ledPins[] = {23, 19, 18, 5, 17, 16, 4, 0};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// กำหนดขา Potentiometer
const int potPin = 36; // ADC1_CH0

void setup()
{
  // ตั้งค่า LED เป็นโหมด OUTPUT
  for (int i = 0; i < numLeds; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // เริ่มต้นปิด LED ทุกดวง
  }

  // เริ่มต้น Serial Monitor เพื่อ Debug
  Serial.begin(115200);
}

void loop()
{
  // อ่านค่าแรงดันจาก Potentiometer
  int analogValue = analogRead(potPin);
  float voltage = (analogValue / 4095.0) * 3.3; // คำนวณแรงดัน (ESP32 ADC 12-bit)

  Serial.print("Voltage: ");
  Serial.println(voltage);

  // ควบคุม LED ตามแรงดัน
  if (voltage >= 0 && voltage < 1.0)
  {
    // แรงดัน 0-0.9V: ปิด LED ทุกดวง
    for (int i = 0; i < numLeds; i++)
    {
      digitalWrite(ledPins[i], LOW);
    }
  }
  else if (voltage >= 1.0 && voltage < 2.0)
  {
    // แรงดัน 1.0-1.9V: เปิด LED 1-3
    for (int i = 0; i < numLeds; i++)
    {
      digitalWrite(ledPins[i], i < 3 ? HIGH : LOW);
    }
  }
  else if (voltage >= 2.0 && voltage < 3.0)
  {
    // แรงดัน 2.0-2.9V: เปิด LED 1-5
    for (int i = 0; i < numLeds; i++)
    {
      digitalWrite(ledPins[i], i < 5 ? HIGH : LOW);
    }
  }
  else if (voltage >= 3.0)
  {
    // แรงดัน > 3.0V: เปิด LED ทุกดวง
    for (int i = 0; i < numLeds; i++)
    {
      digitalWrite(ledPins[i], HIGH);
    }
  }

  delay(100); // หน่วงเวลา 100ms เพื่อลดการอ่านค่า ADC บ่อยเกินไป
}
