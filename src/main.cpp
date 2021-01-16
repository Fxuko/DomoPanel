
//  *****************************
//  DOMOPANEL V.1.0
//  Control domótico con un ESP32
//  *****************************
//

#include <Arduino.h>
#include <U8g2lib.h> // Libreria pantalla OLED I2C
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h" // Libreria sensor temperatura y humedad
#include "logoXBM.h"
#include "RTC.h" // Librería del reloj DS3231


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// configuraciones
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Adafruit_AM2320 am2320 = Adafruit_AM2320();


//Variables
const int sensorPin = 23; // Pin del para el detector de proximidad infrarrojo
const int lednaranja=26; // Pin del led naranja
static DS3231 RTC;
int horas,minutos;

//Dibuja el Logo
void draw(void) {
u8g2.drawXBMP(0,0,DPLogo_width,DPLogo_height,DomoP_logo);
}
// Función para la interrupcion al activarse el sensor infrarrojo
void IRAM_ATTR ifr() {
  digitalWrite(lednaranja,HIGH);
}
//SETUP ***************************
void setup() {
Serial.begin(115200);
RTC.begin();

attachInterrupt(sensorPin, ifr, RISING); // interrupcion para activar el sensor infrarrojo
//pinMode(sensorPin , INPUT); // definir pin 23 como entrada para infrarrojo
pinMode(lednaranja,OUTPUT);


u8g2.begin(); // Inicializando pantalla OLED
am2320.begin(); //Inicializando sensor temperatura/humedad
u8g2.firstPage();
 do {
     draw();
    } while(u8g2.nextPage());
    delay(3500);
}
// LOOP ***************************
void loop(void) {

// variables para guardar valores
float temperatura;
float humedad;
//int value=0;  // varuabke para capturar el pin del sensor infrarrojo
horas=RTC.getHours();
minutos=RTC.getMinutes();


/*
//value=digitalRead(sensorPin);

if (value==LOW){
  digitalWrite(lednaranja,HIGH);
}
else{
  digitalWrite(lednaranja,LOW);
}
*/
// capturas del sensor
temperatura=am2320.readTemperature();
humedad=am2320.readHumidity();
// Mostrar en pantalla OLED
u8g2.firstPage();
do {
u8g2.setFont(u8g2_font_fub30_tn);
//Temperatura
  u8g2.setCursor(2,35);
  u8g2.print(temperatura,1);
  u8g2.setFont(u8g2_font_inb16_mf);
  u8g2.drawGlyph(88, 35, 0x00b0); // degree
  u8g2.drawStr(105, 35, "C");
//Humedad
  u8g2.setCursor(75,60);
  u8g2.print(humedad,0);
  u8g2.drawStr(105, 60, "%");
  u8g2.setFont(u8g2_font_open_iconic_thing_2x_t); // 16 pix height
  u8g2.drawGlyph(50, 60, 0x0048); // drop
} while ( u8g2.nextPage() );
 delay(2000);
 
// Mostrar en la pantalla la hora
u8g2.firstPage();
  do {
       u8g2.setFont(u8g2_font_logisoso42_tn);
       u8g2.setCursor(0,55);
       u8g2.print(horas,1);
       u8g2.drawStr(53,53,":");
       u8g2.setCursor(70,55);
       u8g2.print(minutos,1);
  } while ( u8g2.nextPage() );
  delay(2000);
}