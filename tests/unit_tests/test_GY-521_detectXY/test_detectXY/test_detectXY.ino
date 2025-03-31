#include <Wire.h>
#include <math.h>

const int MPU_ADDR = 0x68; // I2C-адрес MPU-6050

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Инициализация MPU-6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Регистр управления питанием
  Wire.write(0);    // Выход из спящего режима
  Wire.endTransmission(true); // Добавлен параметр true для отправки stop condition
  
  // Настройка чувствительности акселерометра (по желанию)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C); // Регистр конфигурации акселерометра
  Wire.write(0x00); // Диапазон ±2g (16384 LSB/g)
  Wire.endTransmission(true);
  
  Serial.println("MPU-6050 инициализирован");
}

void loop() {
  int16_t accelX, accelY, accelZ;
  
  // Чтение данных акселерометра
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Начальный регистр акселерометра
  if (Wire.endTransmission(false) != 0) { // Проверка ошибок
    Serial.println("Ошибка связи с MPU-6050");
    delay(1000);
    return;
  }
  
  // Запрос 6 байт данных
  if (Wire.requestFrom(MPU_ADDR, 6, true) != 6) {
    Serial.println("Не удалось получить данные");
    delay(1000);
    return;
  }
  
  // Чтение данных
  accelX = Wire.read() << 8 | Wire.read();
  accelY = Wire.read() << 8 | Wire.read();
  accelZ = Wire.read() << 8 | Wire.read();
  
  // Преобразование в g (ускорение в единицах g)
  const float LSB_PER_G = 16384.0; // Для диапазона ±2g
  float Ax = accelX / LSB_PER_G;
  float Ay = accelY / LSB_PER_G;
  float Az = accelZ / LSB_PER_G;
  
  // Вычисление углов наклона (Roll, Pitch) в градусах
  float roll = atan2(Ay, Az) * 180.0 / PI;
  float pitch = atan2(-Ax, sqrt(Ay * Ay + Az * Az)) * 180.0 / PI;
  
  // Вывод данных
  Serial.print("Roll (X): "); Serial.print(roll, 1); // 1 знак после запятой
  Serial.print("° | Pitch (Y): "); Serial.print(pitch, 1);
  Serial.println("°");
  
  delay(100); // Увеличенная задержка для стабильности
}