#include "display.h"

U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

#define BUTTON 2

// ~ VARIABLES ~

unsigned long now;
unsigned long lastPulse;
unsigned long lastClock;

bool displayOn = 1;

int hour = 19;
int minute = 37;
int second = 10;

char secondChar[3];
char clockChar[9];

float batteryLevel = 90;

int displayTimeout = 5000;


// ~ Inicialización ~

void setup() {
  oled.begin();

  pinMode(BUTTON, INPUT);

}


// ~ Main Loop ~

void loop() {

  now = millis();
  
  if(now - lastClock >= 1000){
    runClock();
    batteryLevel -= 5;
    lastClock = now;
  }

  if (digitalRead(BUTTON)) {
    lastPulse = now;
    displayOn = true;
    oled.setPowerSave(0);
  }

  // Si el display está encendido, mostrar y controlar apagado por timeout
  if (displayOn) {
    display();

    if (now - lastPulse >= displayTimeout) {
      oled.setPowerSave(1);
      displayOn = false;
    }
  }  
}

void runClock(){
  second++;
  if(second == 60){
    second = 0;
    minute++;
  }
  if(minute == 60){
    minute = 0;
    hour++;
  }
  if(hour == 24){
    hour = 0;
  }

  sprintf(clockChar, "%02d:%02d", hour, minute);
  sprintf(secondChar, "%02d", second);
}
