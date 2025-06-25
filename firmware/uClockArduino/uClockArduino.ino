#include "display.h"
#include "ClickButton.h"

U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

const int buttonPin = 2;

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

char timezoneChar[4];

float batteryLevel = 90;

int displayTimeout = 5000;


// ~ Inicialización ~

ClickButton button(buttonPin, LOW, CLICKBTN_PULLUP);

void setup() {
  Serial.begin(115200);

  oled.begin();

  button.longClickTime = 300;
  button.debounceTime = 10;
}


// ~ Main Loop ~

void loop() {

  now = millis();
  button.Update();

  if(now - lastClock >= 1000){
    runClock();
    batteryLevel -= 2;
    lastClock = now;
  }
  if(button.clicks == 1 && displayOn == false) {
    displayOn = true;
    lastPulse = now;
  }

  if(displayOn){
    display();
  }

  if (now - lastPulse >= displayTimeout && displayOn == true) {
    oled.setPowerSave(1);
    displayOn = false;
  }






  /*if(now - lastClock >= 1000){
    runClock();
    batteryLevel -= 5;
    lastClock = now;
  }

  if (button.clicks == 0) {
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
  }*/
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




