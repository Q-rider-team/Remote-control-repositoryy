#include <Wire.h>
#include <math.h>
#include <Servo.h>

// Конфигурация
const int MPU_ADDR = 0x68;     // Адрес MPU-6050
const int SERVO_PIN = 11;      // Пин сервопривода
const float DANGEROUS_ANGLE = 70.0; // Порог опасного наклона

// Настройки сервопривода
const int SAFE_POSITION = 180; // Положение "безопасно"
const int ALERT_POSITION = 90; // Положение "опасность"
const int SERVO_SPEED_DELAY = 1; // Задержка между шагами (меньше = быстрее)

Servo protectionServo;          // Объект сервопривода
int currentServoPosition = SAFE_POSITION; // Текущая позиция
bool lastAlertState = false;    // Предыдущее состояние

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Инициализация сервопривода
  protectionServo.attach(SERVO_PIN);
  protectionServo.write(SAFE_POSITION);
  
  // Настройка MPU-6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0);    // Выход из sleep-режима
  Wire.endTransmission(true);
  
  Serial.println("=== СИСТЕМА ЗАЩИТЫ КВАДРОЦИКЛА ===");
  Serial.println("Инициализация завершена");
  Serial.print("Порог срабатывания: ");
  Serial.print(DANGEROUS_ANGLE);
  Serial.println("°");
}

void loop() {
  // Чтение данных с акселерометра
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Начиная с регистра 0x3B
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); // Запрашиваем 6 байт
  
  // Получаем значения осей
  int16_t accelX = Wire.read() << 8 | Wire.read();
  int16_t accelY = Wire.read() << 8 | Wire.read();
  int16_t accelZ = Wire.read() << 8 | Wire.read();
  
  // Конвертируем в g (для ±2g диапазона)
  const float LSB_PER_G = 16384.0;
  float Ax = accelX / LSB_PER_G;
  float Ay = accelY / LSB_PER_G;
  float Az = accelZ / LSB_PER_G;
  
  // Вычисляем углы наклона
  float roll = atan2(Ay, Az) * 180.0 / PI;
  float pitch = atan2(-Ax, sqrt(Ay*Ay + Az*Az)) * 180.0 / PI;
  
  // Определяем опасное состояние
  bool isDangerNow = (abs(roll) >= DANGEROUS_ANGLE) || 
                    (abs(pitch) >= DANGEROUS_ANGLE);
  
  // Выводим текущие углы
  Serial.print("Roll: ");
  Serial.print(roll, 1);
  Serial.print("° | Pitch: ");
  Serial.print(pitch, 1);
  Serial.print("° | Состояние: ");
  
  // Оповещение об изменении состояния
  if (isDangerNow != lastAlertState) {
    if (isDangerNow) {
      Serial.println("ОПАСНО! Активация защиты!");
    } else {
      Serial.println("Норма. Защита отключена");
    }
    lastAlertState = isDangerNow;
  } else {
    Serial.println(isDangerNow ? "ОПАСНО (активно)" : "Норма");
  }
  
  // Управление сервоприводом (быстрое перемещение)
  int targetPos = isDangerNow ? ALERT_POSITION : SAFE_POSITION;
  
  if (currentServoPosition != targetPos) {
    // Мгновенный переход (максимальная скорость)
    protectionServo.write(targetPos);
    currentServoPosition = targetPos;
    
    // Либо для плавного, но быстрого перемещения:
    // int step = (targetPos > currentServoPosition) ? 5 : -5;
    // protectionServo.write(currentServoPosition + step);
    // currentServoPosition += step;
    // delay(SERVO_SPEED_DELAY);
  }
  
  delay(50); // Общая задержка цикла
}