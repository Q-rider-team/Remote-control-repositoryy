#include <GyverButton.h>

// Создаем объект кнопки
GButton button1(6); // Пин, к которому подключена кнопка 1 (левая)
GButton button2(7);
 
void setup() {
  Serial.begin(9600);
  button1.setDebounce(100);        // Устанавливаем антидребезг в 1с
  button1.setTimeout(2000);        // Устанавливаем таймаут удержания в 2с (чтообы вывело held)
  button1.setClickTimeout(500);   // Устанавливаем таймаут между кликами в 300 мс

  button2.setDebounce(100);        // Устанавливаем антидребезг в 50 мс
  button2.setTimeout(2000);        // Устанавливаем таймаут удержания в 2с (чтообы вывело held)
  button2.setClickTimeout(300);
}

void loop() {
  button1.tick(); // Обязательная функция отработки. Должна постоянно опрашиваться
  button2.tick(); // Обязательная функция отработки. Должна постоянно опрашиваться

  if (button1.isHolded()){
    Serial.println("Button is holded");
  }
  else if (button1.isClick()){
    Serial.println("Button is clicked");
  }

  if (button2.isHolded()){
    Serial.println("Button is holded");
  }
  else if (button2.isClick()){
    Serial.println("Button is clicked");
  }
}

// Косяк иногда странно выводит clicker и holded пока что не сильно понятно в чем причина
