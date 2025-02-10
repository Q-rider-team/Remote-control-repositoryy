////////////////////////Danovskiy's concept///////////////////
#include "FastLED.h"
#include <GyverOLED.h>
#include "GyverButton.h"

#define btn_r 32
#define btn_p 40
#define btn_d 38 
#define btn_n 36
#define btn_s 30
#define btn_m 34

#define RELAY_IN1 5    //реле актуатора
#define RELAY_IN2 4    //реле актуатора 
#define RELAY_IN3 3    //реле реверса (бывший брейк)
#define RELAY_IN4 2    //реле реверса (бывший брейк)

#define btn_sport_high 8//  //кнопка для режима спорт
#define btn_sport_low 9// //кнопка для режима спорт

#define btn_high_low 22

#define conc_middle 42//6
 
#define servis_high 6  //кнопки для определения нейтрали 
#define servis_middle 7  //кнопки для определения нейтрали
#define servis_low 8  //кнопки для определения нейтрали

#define LED_PIN 42     // пин для адресной ленты 


#define LED_NUM 8    // количество светодиодов


byte bright = 0;     //переменная определяющая яркость
CRGB leds[LED_NUM];

GyverOLED<SSH1106_128x64> oled;
GButton butt1(btn_p);
GButton butt4(btn_r);
GButton butt3(btn_n);
GButton butt2(btn_m);
GButton butt5(btn_d);
GButton butt6(btn_s);

GButton butt7(btn_high_low);
GButton butt8(btn_sport_low);

GButton butt10(conc_middle);

GButton butt11(servis_high);
GButton butt12(servis_middle);
GButton butt13(servis_low);


//когда выясним, что повышенная, а что пониженная, нужно сделать
//повышенную передачу на один клик(butt1.isClick()), 
//а пониженную на зажатие 2 сек (butt1.isHold())


void setup() {
  FastLED.addLeds< WS2812B, LED_PIN, GRB>(leds, LED_NUM);      

  Serial.begin(9600);
 
  oled.init();   //подключаем олед дисплей ssh1106
  Wire.setClock(800000L);  //скорость вывода, максимум 800000
  oled.clear();       // очистка
  oled.update();  // обновить. Только для режима с буфером! OLED_BUFFER
  oled.setContrast(15); //яркость  15 макс
  pinMode(btn_r, INPUT_PULLUP);   // режимы работы 
  pinMode(btn_p, INPUT_PULLUP);   // режимы работы
  pinMode(btn_d, INPUT_PULLUP);   // режимы работы
  pinMode(btn_n, INPUT_PULLUP);   // режимы работы
  pinMode(btn_s, INPUT_PULLUP);   // режимы работы
  pinMode(btn_m, INPUT_PULLUP);   // режимы работы
  pinMode(RELAY_IN1, OUTPUT);     // реле 1 
  pinMode(RELAY_IN2, OUTPUT);     // реле 2 
  pinMode(RELAY_IN3, OUTPUT);     //реле 3, стоп сигнал и брейк 

  digitalWrite(RELAY_IN1, LOW);    //изначальное положение актуатора всегда на повышенной
  digitalWrite(RELAY_IN2, LOW);   
  digitalWrite(RELAY_IN3, LOW);

  pinMode(btn_high_low, INPUT_PULLUP);   

  pinMode(btn_sport_high, INPUT_PULLUP);   
  pinMode(btn_sport_low, INPUT_PULLUP);   

  pinMode(conc_middle, INPUT_PULLUP);

  butt3.setTimeout(500);
  butt6.setTimeout(500);
  butt2.setTimeout(100);
  butt7.setTimeout(500);

  }
  
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;    
  bool high_click = 0;
  bool low_hold = 1;
//byte counter;  //для радуги

  void loop() 
  {    
  FastLED.addLeds< WS2812B, LED_PIN, GRB>(leds, LED_NUM);      
  
  if (bright == 0)  //плавное включние адресной ленты
    {          
    
      for (byte i = 0; i < 255; i++)
      {
        FastLED.setBrightness(bright);
        FastLED.showColor(CRGB::Lime);   /////////https://github.com/FastLED/FastLED/wiki/Pixel-reference
        delay(15);                           ///сайт с названиями цветов
        bright += 1;                           //Amethyst Aqua Aquamarine
        FastLED.show();                        //DarkGreen Teal Lime LimeGreen
        Serial.println(bright);                //LightSeaGreen Indigo DeepPink
       }                                       //Crimson 
     }
     
 // FastLED.setBrightness(255);
 // FastLED.showColor(CRGB::Lime);
           
  butt1.tick();
  butt2.tick();
  butt3.tick();
  butt4.tick();
  butt5.tick();
  butt6.tick();
  
  butt7.tick();  //кнопка high low
  butt8.tick();  //
  
  butt10.tick(); //conc_middle

  butt11.tick(); //кнопка на джойсктике HIGH
  butt12.tick();  //кнопка на джойсктике MIDL
  butt13.tick();  //кнопка на джойсктике LOW

////////////////////REVERSE////////////////////////    
  if (butt4.isHolded())
  {     
  FastLED.setBrightness(255);
  FastLED.showColor(CRGB::DarkRed);   
///////////////////////////////////
  oled.rect(x1, y1, x2+123, y2+63, OLED_CLEAR);
  oled.update();
/////////////////////////////////////
  oled.setScale(2);   // масштаб текста (1..4)
  oled.setCursor(10, 1);   //координаты Х и У
  oled.print("H");
  oled.setCursor(44, 1);
  oled.print("L");
  oled.setCursor(108, 1);
  oled.print("N"); 
  oled.update();
/////////////////////////////////////
    oled.setCursor(76, 1);
    oled.setScale(2);
    oled.print("R");
    oled.roundRect(x1+70, y1+2, x2+91, y2+26, OLED_STROKE);    
    oled.rect(0, 28, 127, 63, OLED_CLEAR);
    oled.update();     
    oled.setScale(3);
    oled.setCursorXY(2, 36);
    oled.print("REVERSE");
    oled.update();
///////////////////////////////////
    static bool flag_p = 1;
    if (butt4.isHold())
    {
      digitalWrite(RELAY_IN3, HIGH);      
      butt4.tick();
    }
    digitalWrite(RELAY_IN3, LOW);
    flag_p = !flag_p;
    butt4.tick();    
    butt6.tick();    
    butt3.tick(); 
    butt7.tick();
  }
//--------------------HIGH--------------------  
  if (butt6.isHolded())
    { 
    FastLED.setBrightness(255);
    FastLED.showColor(CRGB::Lime);  
    digitalWrite(RELAY_IN3, HIGH); 
//----------------вывод текста-------------------
    oled.rect(x1, y1, x2+123, y2+63, OLED_CLEAR);
    oled.update();
    oled.setScale(2);   
    oled.setCursor(44, 1);
    oled.print("L");
    oled.setCursor(76, 1);
    oled.print("R");
    oled.setCursor(108, 1);
    oled.print("N"); 
    oled.update();
    oled.setCursor(10, 1);
    oled.setScale(2);
    oled.print("H");
    oled.roundRect(x1+4, y1+2, x2+24, y2+26, OLED_STROKE);    
    oled.rect(0, 28, 127, 63, OLED_CLEAR);
    oled.update();     
    oled.setScale(3);
    oled.setCursorXY(32, 36);
    oled.print("HIGH");    // тут проверить H или L
    oled.update();
//------------------Реле---------------------------
    high_click++;
    digitalWrite(RELAY_IN1 , HIGH);
    digitalWrite(RELAY_IN2 , LOW);         
    Serial.println("sport_high"); 
    butt6.tick();  
    butt4.tick();    
    butt3.tick(); 
    butt7.tick();
    }   
//---------------------LOW--------------------------  
   if (butt3.isHolded())
    {
    FastLED.setBrightness(255);
    FastLED.showColor(CRGB::Yellow);        
    digitalWrite(RELAY_IN3, HIGH);    
//---------------------вывод текста-----------------
    oled.rect(x1, y1, x2+123, y2+63, OLED_CLEAR);
    oled.update();
    oled.setScale(2);   
    oled.setCursor(10, 1);   //координаты Х и У
    oled.print("H");
    oled.setCursor(76, 1);
    oled.print("R");
    oled.setCursor(108, 1);
    oled.print("N"); 
    oled.setCursor(44, 1);
    oled.setScale(2);
    oled.print("L");
    oled.roundRect(x1+38, y1+2, x2+58, y2+26, OLED_STROKE);
    oled.rect(0, 28, 127, 63, OLED_CLEAR);
    oled.update();     
    oled.setScale(3);
    oled.setCursorXY(0, 36);
    oled.print("  LOW   ");    
    oled.update();
//----------------------------------------
    low_hold++;
    digitalWrite(RELAY_IN1 , LOW);
    digitalWrite(RELAY_IN2 , HIGH);          
    butt3.tick(); 
    butt6.tick();  
    butt4.tick();
    butt7.tick();
    }
//--------------------NEUTRAL------------------------
  if (butt2.isHolded())
  { 
    FastLED.setBrightness(255);
    FastLED.showColor(CRGB::DeepPink);
    digitalWrite(RELAY_IN3, HIGH);
    oled.rect(x1, y1, x2+123, y2+63, OLED_CLEAR);
    oled.clear();
    oled.setScale(2);     
    oled.setCursor(10, 1);   //координаты Х и У
    oled.print("H");
    oled.setCursor(44, 1);
    oled.print("L");
    oled.setCursor(76, 1);
    oled.print("R");
    oled.setCursor(108, 1);
    oled.setScale(2);
    oled.print("N");
    oled.roundRect(x1+102, y1+2, x2+122, y2+26, OLED_STROKE);    
    oled.rect(0, 28, 127, 63, OLED_CLEAR);
    oled.update();     
    oled.setScale(3);
    oled.setCursorXY(2, 36);
    oled.print("разраб.");
    oled.update();
//-----------------------------------------
    butt3.tick();  
    butt6.tick();  
    butt4.tick();
    butt7.tick();
    butt2.tick();    
    /*digitalWrite(RELAY_IN1 , LOW);
    digitalWrite(RELAY_IN2 , HIGH);   
    delay(1500);*/
    digitalWrite(RELAY_IN1, LOW);
    digitalWrite(RELAY_IN2, LOW); 
    /*if (butt12.isClick())   
    {
        Serial.println("СЕРЕДИНКА");
        digitalWrite(RELAY_IN1, LOW);
        digitalWrite(RELAY_IN2, LOW);        
    }
    
    if (butt13.hasClicks())  //выбрать пин для кнопки
    {   
        Serial.println("ЗАДВИНУТ");
        digitalWrite(RELAY_IN1, HIGH);
        digitalWrite(RELAY_IN2, LOW);
    }
    if (butt11.hasClicks())  //выбрать пин для кнопки
    {
        Serial.println("ВЫДВИНУТ");
        digitalWrite(RELAY_IN2, HIGH);
        digitalWrite(RELAY_IN1, LOW);
    }*/
  }
/////////////////////Турбо режим///////////////////////    
 /*if (butt5.isHolded())
  {  
     for (int i2 = 0; i2 < LED_NUM; i2++ ) {         // от 0 до первой трети
    leds[i2] = CHSV(counter + i2 * 2, 255, 255);  // HSV. Увеличивать HUE (цвет)
    // умножение i уменьшает шаг радуги
  }
  counter++;        // counter меняется от 0 до 255 (тип данных byte)
  delay(5);  
     
    oled.setScale(2);
    oled.setCursorXY(0, 32);
    oled.print("EMP ТОП");
    oled.update(); 
    oled.update(); 
    butt3.tick();  
    butt6.tick();  
    butt4.tick();
    butt7.tick();
    butt2.tick();   
  }*/
}
