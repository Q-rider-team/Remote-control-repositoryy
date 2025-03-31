#include <Wire.h>  

void setup() {    
    Serial.begin(115200);    
    Wire.begin();    
    Serial.println("Поиск I2C-устройств...");  
}  

void loop() {
    byte error, address;  
    int devices = 0;  

    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);  
        error = Wire.endTransmission();  

        if (error == 0) {
            Serial.print("Найдено устройство на 0x");  
            Serial.println(address, HEX);  
            devices++;  
        }
    }

    if (devices == 0) Serial.println("Устройства не найдены!");  

    delay(5000);  
}