#include "display.h"



void display(){
  oled.setPowerSave(0);

  oled.firstPage();

  do {
    
    displayMargins();
    displayClock();
    displaySeconds();
    displayTimeZone();
    displayDate();
    displayBatteryLevel();
    displayWiFiActive();
    displayName();

  } while (oled.nextPage());
}

void displayMargins(){
  oled.drawHLine(0, 9, oled.getDisplayWidth());
  oled.drawHLine(0, 49, oled.getDisplayWidth());
}

void displayClock(){
  oled.setFont(u8g2_font_logisoso30_tr);
  oled.drawStr(0, 44, clockChar);
}

void displaySeconds(){
  oled.setFont(u8g2_font_logisoso16_tr);
  oled.drawStr(105, 44, secondChar);
}

void displayTimeZone(){
  oled.setFont(u8g2_font_7x14B_tr);
  oled.drawStr(105, 24, timezoneChar);
}

void displayDate(){
  oled.setFont(u8g2_font_7x14B_tr);
  oled.drawStr(0, 63, "Miercoles");

  oled.drawStr(90, 63, "11/11");
}

void displayBatteryLevel(){
  oled.drawXBM(115, 0, 13, 7, battery_level_icon);

  if(batteryLevel > 80){
    oled.drawBox(118, 2, 2, 3);
  }
  if(batteryLevel > 40){
    oled.drawBox(121, 2, 2, 3);
  }
  if(batteryLevel > 20){
    oled.drawBox(124, 2, 2, 3);
  }
}

void displayWiFiActive(){
  oled.drawXBM(101, 0, 11, 7, WiFi_indicator_icon);
}

void displayName(){
  oled.drawXBM(1, 0, 32, 8, uClock_icon);
}

