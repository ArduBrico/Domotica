///// https://www.youtube.com/@ArduBrico /////

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

///// PARAMETROS CONFIGURABLES /////

const char* ssid = "SSID";                 // Nombre de tu SSID
const char* password = "PASSWORD";         // Contraseña de tu SSID
const char* mqtt_server = "XXX.XXX.X.XX";  // I.P. de tu servidor MQTT
int mqttport = 1883;                       // Puerto para MQTT
const char* mqttusuario = "MQTT";          // Usuario MQTT en Home Assistant
const char* mqttpass = "PASS_MQTT";        // Contraseña para el usuario MQTT en Home Assistant
const char* OTA_password = "PASS_OTA";     // Contraseña OTA

String User1 = "Juan";   // Nombre usuario 1, debe coincidir con la huella grabada en el sketch "enroll"
String User2 = "Yoli";   // Nombre usuario 2, debe coincidir con la huella grabada en el sketch "enroll"
String User3 = "Jorge";  // Nombre usuario 3, debe coincidir con la huella grabada en el sketch "enroll"
String User4 = "Juani";  // Nombre usuario 4, debe coincidir con la huella grabada en el sketch "enroll"
// Añadir tantos usuarios como hayamos grabado en el sketch "enroll"

String actionIn = "Entra";                        // Nombre que queremos dar a la acción de "entrar"
String actionOut = "Sale";                        // Nombre que queremos dar a la acción de "salir"
String User[4] = { User1, User2, User3, User4 };  // "array" con los usuarios, tantos como hayamos configurado

///// FIN PARAMETROS CONFIGURABLES /////

#define CLIENT_ID "Lector de huellas"  // ID del dispositivo, debe ser único en tu sistema
#define MQTT_TOPIC "lector_huellas"    // Topic específico para enviar los mensajes de MQTT al sensor

WiFiClient espClient;
PubSubClient client(espClient);

SoftwareSerial mySerial(D5, D6);  // Configuración de los pines del lector de huellas

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const int BUZZER_PIN = D2;  // Configuración del pin del "buzzer"
int estado_alarma = 0;      // Guardamos el estado de la alarma
int huella_ok = 0;          // Guardamos el estado de la huella

String usuario;    // Lo utilizaremos para enviar el usuario correspondiente al topic
String accion;     // Lo utilizaremos para enviar la acción correspondiente al topic
String resultado;  // LO utilizamos para enviar la frase resultante para la automatización

void setup() {
  Serial.begin(9600);  // Iniciamos el Serial
  while (!Serial)
    ;
  delay(100);

  pinMode(BUZZER_PIN, OUTPUT);  // Definimos el pin del buzzer como salida

  // Iniciamos el Wifi
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Iniciamos OTA
  ArduinoOTA.setHostname(CLIENT_ID);
  ArduinoOTA.setPassword(OTA_password);
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
  ArduinoOTA.begin();

  Serial.println("setup end");

  Serial.println("\n\nAdafruit finger detect test");

  // Iniciamos la configuración del sensor
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  } else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
}

void loop() {

  ArduinoOTA.handle();  // Inicia la función OTA
  getFingerprintID();   // Inicia la función del sensor
  estado();             // Inicia la función "estado"
  delay(50);

  //// CONECTAMOS A WIFI Y MQTT ////

  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
    reconnect();
    client.setServer(mqtt_server, mqttport);
    client.setCallback(callback);
  }

  // Reconectamos cuando MQTT no está conectado
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
uint8_t getFingerprintID() {  // Función que se encarga de leer las huellas
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      error();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // Si obtenemos una huella:
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    correcto();                           // Si la huella es reconocida, hacemos sonar el pitido de confirmación
    huella_ok = 1;                        // Establecemos el estado en "1"
    Serial.println(finger.fingerID);      // Mostramos por serial el numero de huella
    usuario = User[finger.fingerID - 1];  // Lo comparamos con el "array", restamos (-1) porque "array" comienza en "0"
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");  // Si la huella no se reconoce, hacemos sonar el pitido de "error"
    error();                                 // Si os resulta molesto, comentar esta linea
    huella_ok = 0;                           // Establecemos el estado en "0"
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return finger.fingerID;
}
void estado() {

  //// #### ARMAR O DEASARMAR ALARMA###

  if (huella_ok == 1 && estado_alarma == 0) {             // Si la huella es reconocida y la alarma está en estado "disarmed"
    accion = actionOut;                                   // Establecemos la "accion" en "salida"
    resultado = accion + " " + usuario;                   // Establecemos el resultado con la accion junto al usuario reconocido
    client.publish("alarmo/command", "arm_away", true);   // Conectamos la alarma en modo "Ausente"
    client.publish(MQTT_TOPIC, resultado.c_str(), true);  // Enviamos al topic el resultado (accion + usuario) Ejem. "Juan Sale"
    Serial.println("Alarma armada Ausente");              // Mostramos por serial el mensaje
    Serial.println(resultado);                            // Mostramos por serial el resultado (Utilizado para la atumatización)
    accion = "";                                          // Establecemos "accion" en blanco
    resultado = "";                                       // Establecemos "resultado" en blanco
    huella_ok = 0;                                        // Establecemos "huella_ok" en 0

  } else if (huella_ok == 1 && estado_alarma == 1) {      // Si la huella es reconocida y la alarma NO está en estado "disarmed"
    accion = actionIn;                                    // Establecemos la "accion" en "entrada"
    resultado = accion + " " + usuario;                   // Establecemos el resultado con la accion junto al usuario reconocido
    client.publish("alarmo/command", "disarm", true);     // Desconectamos la alarma
    client.publish(MQTT_TOPIC, resultado.c_str(), true);  // Enviamos al topic el resultado (accio n+ usuario) Ejem. "Juan Entra"
    Serial.println(resultado);                            // Mostramos por serial el resultado (Utilizado para la atumatización)
    Serial.println("Alarma desarmada");                   // Mostramos por serial el mensaje
    accion = "";                                          // Establecemos "accion" en blanco
    resultado = "";                                       // Establecemos "resultado" en blanco
    huella_ok = 0;                                        // Establecemos "huella_ok" en 0

  } else {          // Si no ocurre nada de lo anterior:
    huella_ok = 0;  // Establecemos "huella_ok" en 0
  }
}
void error() {  // Hacemo sonar el pitido de ERROR

  tone(BUZZER_PIN, 1300);
  delay(20);
  tone(BUZZER_PIN, 1000);
  delay(200);
  noTone(BUZZER_PIN);
}
void correcto() {  // Hacemo sonar el pitido de CONFIRMACIÓN

  tone(BUZZER_PIN, 1000);
  delay(20);
  tone(BUZZER_PIN, 1300);
  delay(200);
  noTone(BUZZER_PIN);
}
void setup_wifi() {  // Configuración para conectar a Wifi
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");  // Mostramos por serial el Wifi que nos queremos conectar
  Serial.println(ssid);           // Muestra el "SSID" que configuramos
  WiFi.mode(WIFI_STA);            // Configuración de tipo de conexión
  WiFi.begin(ssid, password);     // Configuración de nombre de "SSID" y su password

  while (WiFi.status() != WL_CONNECTED) {  // Si no está conectado a Wifi
    // Esperamos hasta que conecte
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
  Serial.println("WiFi conectado");  // Confirmamos que hemos conectado a Wifi
  Serial.println("Dirección IP: ");  // Y mostramos la IP otorgada por el router
  Serial.println(WiFi.localIP());    // IP otorgada por el router
}
void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");  // Si no estamos conectados a MQTT

    if (client.connect(CLIENT_ID, mqttusuario, mqttpass)) {  // Si conectamos a Wifi, establecemos la configuración para conectarnos
      Serial.println("connected");                           // Mostramos que hemos conectado por serial
      client.publish(MQTT_TOPIC, "Esperando", true);         // Publicamos para informar que esperamos una huella
      client.subscribe("alarmo/state");                      // Nos suscribimos al tipoc del estado de la alarma
    } else {                                                 // Si no hemos podido conectar:
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  // Intenta la reconexión en 5 sergundos

      for (int i = 0; i <= 500; i++) {
        delay(10);
        if (i == 50) {
          Serial.println(".");
        } else {
          Serial.print(".");
        }
      }
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length) {  // Función para poder recibir los mensajes por MQTT
  Serial.print("Mensaje recibido [");                             // Mostramos por serial cuando recibimos un mensaje
  Serial.print(topic);                                            // Mostramos el topic que recibimos
  Serial.print("] ");                                             //
  for (int i = 0; i < length; i++) {                              //
    Serial.print((char)payload[i]);                               // Muestra el mensaje por serial
  }
  Serial.println();

  payload[length] = '\0';
  String message = (char*)payload;  // Convertimos en mensaje a "String"

  if (strcmp(topic, "alarmo/state") == 0) {  // Si recibimos un mensaje del topic del estado de la alarma
    if (message == "disarmed") {             // Si el mensaje es "disarmed" (desarmada)
      estado_alarma = 0;                     // Establecemos el estado en "0"
    } else {                                 // Si no
      estado_alarma = 1;                     // Establecemos el estado en "1"
    }
  }
}