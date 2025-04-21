#include <AccelStepper.h>
#include <Encoder.h>

// Настройки пинов
const int potPin = A0;          // Потенциометр
const int encoderPinA = 10;      // Энкодер канал A
const int encoderPinB = 11;      // Энкодер канал B
const int stepPin = 3;          // Шаговый драйвер STEP
const int dirPin = 9;           // Шаговый драйвер DIR

// Инициализация библиотек
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin); // Драйвер типа STEP/DIR
Encoder myEncoder(encoderPinA, encoderPinB);                 // Энкодер

// Параметры шагового двигателя
const float maxSpeed = 1000.0;      // Максимальная скорость (шаги/сек)
const float acceleration = 500.0;   // Ускорение (шаги/сек^2)

void setup() {
  // Настройка скорости и ускорения шагового двигателя
  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(acceleration);

  // Установка начальной позиции шагового двигателя
  long initialPosition = myEncoder.read();
  stepper.setCurrentPosition(initialPosition);

  Serial.begin(9600); // Для отладки
}

void loop() {
  // Чтение значения с потенциометра
  int potValue = analogRead(potPin);
  long targetPosition = map(potValue, 0, 1023, -20000, 20000); // Преобразование в диапазон позиций

  // Чтение текущей позиции с энкодера
  long currentPosition = myEncoder.read();

  // Отладочная информация
  Serial.print("Target: ");
  Serial.print(targetPosition);
  Serial.print(" Current: ");
  Serial.println(currentPosition);

  // Установка новой целевой позиции для шагового двигателя
  stepper.moveTo(targetPosition);

  // Выполнение движения
  stepper.run();
}