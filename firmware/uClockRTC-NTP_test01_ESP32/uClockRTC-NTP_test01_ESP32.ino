#include "Wire.h"
#include "WiFi.h"
#include "time.h"
#include "RTClib.h"

const int btnPin = 4;
unsigned long lastBtnChange = 0;  // variable global
bool showLocal = true;
int timezone = -3;

const long NTP_SYNC_PERIOD = 60000;

bool syncing = false;
int startSecond;

const char* ssid = "Telecentro-7a70";
const char* password = "tele1234";

const char* ntpServer = "pool.ntp.org";

RTC_DS3231 rtc;

unsigned long lastSync = 0;

void setup() {
  Serial.begin(115200);

  pinMode(btnPin, INPUT_PULLUP);

  Serial.print("Conectando WiFi...");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){}

  Serial.print("\nWiFi conectado: ");
  Serial.println(WiFi.localIP());

  if (!rtc.begin()) {
    Serial.println("ERROR: No se puedo encontrar RTC");
    while (1);
  }
  
  configTime(0, 0, ntpServer);
  Serial.printf("Esperando sincronizacion NTP(%s)...\n", ntpServer);

  time_t now;
  do {
    delay(100);
    time(&now);
  } while (now < 100000); // Espera hasta que el tiempo sea válido

  Serial.println("Tiempo sincronizado.");

  syncRTC(0);
}

void loop() {
  static bool lastBtnState = HIGH;
  bool btnState = digitalRead(btnPin);
  unsigned long now = millis();

  if (btnState == LOW && lastBtnState == HIGH && now - lastBtnChange > 200) {
    showLocal = !showLocal;
    Serial.printf("Modo hora cambiado a: %s\n", showLocal ? "LOCAL" : "UTC");
    lastBtnChange = now;
  }
  lastBtnState = btnState;

  syncRTC(NTP_SYNC_PERIOD);
  printRTC();
}

void syncRTC(long syncPeriod) {
  

  if (!syncing && millis() - lastSync >= syncPeriod) {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    startSecond = timeinfo.tm_sec;
    syncing = true;
    Serial.println("Sincronizando RTC...");
  }

  if (syncing) {
    struct tm timeinfo;
    struct timeval tv;
    gettimeofday(&tv, nullptr); // obtenés también microsegundos

    getLocalTime(&timeinfo);
    if (timeinfo.tm_sec != startSecond) {
      // Esperá hasta que los microsegundos se acerquen a 0
      while (tv.tv_usec > 100000) { // Esperá hasta que esté dentro del primer 0.1s del segundo
        delayMicroseconds(100); 
        gettimeofday(&tv, nullptr);
      }

      time_t t;
      time(&t);
      rtc.adjust(DateTime(t)); // Ajustás al segundo siguiente
      Serial.println("RTC sincronizado.");
      lastSync = millis();
      syncing = false;
    }
  }
}

void printRTC(){
  static int lastSecond = -1;
  DateTime now = rtc.now();

  if (now.second() != lastSecond) {
    struct timeval tv;
    gettimeofday(&tv, nullptr); // tiempo actual del sistema, en segundos y microsegundos

    // Convertimos RTC a timestamp en ms
    uint64_t rtc_ms = now.unixtime() * 1000ULL;
    uint64_t sys_ms = (uint64_t)tv.tv_sec * 1000ULL + tv.tv_usec / 1000;
    int64_t offset_ms = (int64_t)(rtc_ms - sys_ms); // puede ser negativo

    int displayHour = now.hour();
    if (showLocal) {
      displayHour = (displayHour + 24 + timezone) % 24;
    }

    Serial.printf("%02d/%02d/%04d %02d:%02d:%02d | Offset: %lld ms | Temp: %.2f °C\n", now.day(), now.month(), now.year(), displayHour, now.minute(), now.second(), offset_ms, rtc.getTemperature());

    lastSecond = now.second();
  }
}