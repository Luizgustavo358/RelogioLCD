// incluindo bibliotecas
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<time.h>
#include <ESP8266WiFi.h>

// dados do wifi
const char* ssid = "Uai-fi dos Bragança"; // WiFi Nome, SSID
const char* password = "3649Aqwh!@";      // WiFi Senha

// configurando o time zone
const char* NTP_SERVER = "ch.pool.ntp.org";
const char* TZ_INFO    = "BRST+3BRDT+2,M10.3.0,M2.3.0";

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;

// configurando o display lcd com a backlight ligada
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/* ------------------------------------------------------------------------ SETUP */
void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5);
  delay(20);

  // inicializa o display 16x2
  lcd.begin(16, 2);

  lcd.setBacklight(HIGH);

  lcd.setCursor(0, 0);
  lcd.print("Conectando a");
  lcd.setCursor(0, 1);
  lcd.print(ssid);

  connectToWiFi();

  // configurando a hora
  configTime(0, 0, NTP_SERVER);
  setenv("TZ", TZ_INFO, 1);

  if(getNTPtime(10)) {
    // espera 10s para sincronizar
  } else {
    ESP.restart();
  }

  lcd.clear();
  showTime(&timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
}
/* ------------------------------------------------------------------------ SETUP */

void connectToWiFi() {
  // conectando ao WiFi
  WiFi.begin(ssid, password);

  int counter = 0;
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(200);

    if(counter > 100) {
      ESP.restart();
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Conectado!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
}

/* ------------------------------------------------------------------------ LOOP */
void loop() {
  getNTPtime(10);
  showTime(&timeinfo);
  delay(1000);
}
/* ------------------------------------------------------------------------ LOOP */

bool getNTPtime(int sec) {
  {
    uint32_t start = millis();

    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      delay(10);
    } while(((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));

    if(timeinfo.tm_year <= (2016 - 1900)) {
      return false;
    }
  }
  return true;
}

void showTime(tm *localTime) {
  /*// Escreve a Data
//  lcd.setCursor(0, 0);
//  lcd.print("Data:");
//  lcd.setCursor(5, 0);
//  lcd.print(" ");
//  lcd.setCursor(6, 0);
//  lcd.print(localTime->tm_mday);
//  lcd.setCursor(8, 0);
//  lcd.print("/");
//  lcd.setCursor(9, 0);
//  lcd.print(localTime->tm_mon + 1);
//  lcd.setCursor(11, 0);
//  lcd.print("/");
//  lcd.setCursor(12, 0);
//  lcd.print(localTime->tm_year - 100);
//  lcd.setCursor(14, 0);
//  lcd.print(" ");*/

  // variavel de hora e data
  char time_output[16];

  // Escreve a Data
  lcd.setCursor(0, 0);
  sprintf(time_output, "Data: %02d/%02d/%02d ", localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year - 100);
  lcd.print(time_output);
  
  // Escreve a Hora
  lcd.setCursor(0, 1);
  sprintf(time_output, "Hora: %02d:%02d:%02d", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
  lcd.print(time_output);
  
/*/  lcd.setCursor(0, 1);
//  lcd.print("Hora: ");
//  lcd.setCursor(6, 1);
//  lcd.print(localTime->tm_hour);
//  lcd.setCursor(8, 1);
//  lcd.print(":");
//  lcd.setCursor(9, 1);
//  lcd.print(localTime->tm_min);
//  lcd.setCursor(11, 1);
//  lcd.print(":");
//  lcd.setCursor(12, 1);
//  lcd.print(localTime->tm_sec);*/
}
