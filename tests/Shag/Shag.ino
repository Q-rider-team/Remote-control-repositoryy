#include <GyverStepper.h>
#include <GyverButton.h>

// Кнопки
GButton button1(6);  // Вперёд
GButton button2(7);  // Назад

// Шаговый двигатель (800 шагов/оборот * 8 микрошагов = 6400 импульсов на оборот)
GStepper<STEPPER2WIRE> stepper(6400, 3, 4, 5);  // STEP=3, DIR=4, ENABLE=5

void setup() {
  button1.setDebounce(50);
  button2.setDebounce(50);
  
  stepper.setRunMode(FOLLOW_POS);
  stepper.setSpeed(500);       // Скорость (шагов/сек)
  stepper.setAcceleration(800); // Ускорение
  stepper.enable();            // Включить драйвер
}

void loop() {
  button1.tick();
  button2.tick();
  
  // Нажатие кнопки 1 - вращение вперёд (1 оборот)
  if (button1.isPress()) {
    stepper.setTarget(stepper.getCurrent() + 6400); // +6400 микрошагов = 1 оборот
  }
  
  // Нажатие кнопки 2 - вращение назад (1 оборот)
  if (button2.isPress()) {
    stepper.setTarget(stepper.getCurrent() - 6400); // -6400 микрошагов = 1 оборот
  }
  
  stepper.tick();
}