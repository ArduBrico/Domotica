///// https://www.youtube.com/@ArduBrico /////

#include <ESP8266WiFi.h>  // Incluye la librería para manejar la conexión WiFi en el ESP8266
#include <time.h>         // Incluye la librería para manejar el tiempo
#include <ArduinoOTA.h>   // Incluye la librería para actualizar el firmware a través de OTA
#include "TM1637.h"       // Incluye la librería para manejar el display TM1637
#include "debug.h"        // Incluye el archivo de encabezado para depuración (debug)

const char* ssid = "SSID";          // Nombre de tu SSID (nombre de tu red WiFi)
const char* password = "PASSWORD";  // Contraseña de tu SSID (contraseña de tu red WiFi)
#define CLIENT_ID "Reloj_Batman"    // Nombre único para el dispositivo (identificador del cliente)

#define NTP_SERVER "pool.ntp.org"        // Servidor NTP para sincronización de tiempo https://www.ntppool.org/es/
#define TZ "CET-1CEST,M3.5.0,M10.5.0/3"  // Zona horaria para ajustar el reloj (Time Zone) https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define CLK 5                            // Pin CLK del display conectado al pin D1 del ESP8266
#define DIO 4                            // Pin DIO del display conectado al pin D2 del ESP8266
TM1637 tm1637(CLK, DIO);                 // Objeto para manejar el display TM1637

time_t now;  // Variable para almacenar el tiempo actual
tm tm;       // Estructura de tiempo

void showTime() {
  time(&now);              // Obtiene el tiempo actual
  localtime_r(&now, &tm);  // Convierte el tiempo actual en una estructura tm

  static char buf[10] = { '\0' };
  static char prev[10] = { '\0' };
  static long last_read = 0;
  static bool flash = true;

  sprintf(buf, "%02d%02d", tm.tm_hour, tm.tm_min);

  if (strcmp(buf, prev) != 0) {

    tm1637.display(0, buf[0] - '0');
    tm1637.display(1, buf[1] - '0');
    tm1637.display(2, buf[2] - '0');
    tm1637.display(3, buf[3] - '0');

    strcpy(prev, buf);
  }

  long now = millis();

  if ((last_read == 0) || (abs(now - last_read) > 500)) {
    flash = !flash;

    tm1637.point(flash);
    memset(prev, '\0', sizeof(prev));
    last_read = now;
  }
}

void setup_wifi() {  // Configura la conexión WiFi
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");  // Mensaje de conexión
  Serial.println(ssid);           // Muestra el nombre de la red WiFi a la que se está conectando
  WiFi.mode(WIFI_STA);            // Configura el modo de conexión WiFi
  WiFi.begin(ssid, password);     // Intenta conectarse a la red WiFi

  while (WiFi.status() != WL_CONNECTED) {  // Espera hasta que se conecte
    for (int i = 0; i <= 50; i++) {
      delay(10);
      if (i == 50) {
        Serial.println(".");
      } else {
        Serial.print(".");
      }
    }
  }
  Serial.println("");
  Serial.println("WiFi conectado");  // Confirma la conexión WiFi
  Serial.println("Dirección IP: ");  // Muestra la dirección IP asignada por el router
  Serial.println(WiFi.localIP());    // Muestra la dirección IP asignada por el router
}

void setup() {
  Serial.begin(115200);  // Inicia la comunicación serial a 115200 baudios
  Serial.println("\nNTP TZ DST - bare minimum");

  tm1637.init();               // Inicializa el display TM1637
  tm1637.point(true);          // Activa el punto decimal en el display
  tm1637.set(BRIGHT_DARKEST);  // Configura el brillo del display

  configTime(TZ, NTP_SERVER);  // Configura la hora y la sincronización con el servidor NTP

  setup_wifi();  // Inicia la conexión WiFi

  // Configuración para Actualización de Firmware Over-The-Air (OTA)
  ArduinoOTA.setHostname(CLIENT_ID);
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();  // Inicia el servicio OTA
}

void loop() {
  ArduinoOTA.handle();  // Maneja las actualizaciones OTA

  showTime();   // Muestra la hora en el display
  delay(1000);  // Espera 1 segundo
}
