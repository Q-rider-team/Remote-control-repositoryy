	#include <Wire.h>	#include <math.h>		const int MPU_ADDR = 0x68; // I2C-адрес MPU-6050		
  void setup() {	    
    Serial.begin(115200);
	  Wire.begin();
	
	  // Выводим MPU-6050 из режима сна
	  Wire.beginTransmission(MPU_ADDR);
	  Wire.write(0x6B); // Регистр управления питанием
	  Wire.write(0);    // Устанавливаем 0 (активный режим)
	  Wire.endTransmission();
	  Serial.println("MPU-6050 инициализирован.");
	}
	
	void loop() {
	    int16_t accelX, accelY, accelZ;
	    
	    // Читаем данные акселерометра
	    Wire.beginTransmission(MPU_ADDR);
	    Wire.write(0x3B); // Начальный регистр акселерометра
	    Wire.endTransmission(false);
	    Wire.requestFrom(MPU_ADDR, 6, true); // Читаем 6 байт
	
	    accelX = (Wire.read() << 8) | Wire.read();
	    accelY = (Wire.read() << 8) | Wire.read();
	    accelZ = (Wire.read() << 8) | Wire.read();
	
	    // Преобразуем в g (ускорение в единицах g)
	    float Ax = accelX / 16384.0;
	    float Ay = accelY / 16384.0;
	    float Az = accelZ / 16384.0;
	
	    // Вычисляем углы наклона (Roll, Pitch) в градусах
	    float roll  = atan2(Ay, Az) * 180.0 / PI;
	    float pitch = atan2(-Ax, sqrt(Ay * Ay + Az * Az)) * 180.0 / PI;
	
	    // Вывод данных
	    Serial.print("Roll (X): "); Serial.print(roll);
	    Serial.print(" | Pitch (Y): "); Serial.println(pitch);
	
	    delay(50);
	}
	
	//x - наклон (Влево/Вправо)
