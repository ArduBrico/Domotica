///// https://www.youtube.com/@ArduBrico /////

#include <ESP8266WiFi.h>   // Biblioteca necesaria para las conexiones Wifi de las placas con chip ESP8266
#include <PubSubClient.h>  // Biblioteca ecesaria para poder publicar y recibir mensajes MQTT
#include <ArduinoOTA.h>    // Biblioteca necesaria para poder programar por OTA

///// PARAMETROS CONFIGURABLES /////

const char* ssid = "SSID";            // Nombre de tu SSID
const char* password = "PASSWORD";    // Contraseña de tu SSID
const char* mqtt_server = "XXX.XXX.X.XX";  // I.P. de tu servidor MQTT
int mqttport = 1883;                       // Puerto para MQTT
const char* mqttusuario = "MQTT";          // Usuario MQTT en Home Assistant
const char* mqttpass = "PASS_MQTT";             // Contraseña para el usuario MQTT en Home Assistant
const char* OTA_password = "PASS_OTA";         // Contraseña OTA
#define CLIENT_ID "Luz_XXX"               // ID del dispositivo, debe ser único en tu sistema
#define MQTT_TOPIC1 "luces/1"              // Topic luz Nº1
#define MQTT_TOPIC2 "luces/2"              // Topic luz Nº2
#define MQTT_TOPIC3 "luces/3"              // Topic luz Nº3
#define MQTT_TOPIC4 "luces/4"              // Topic luz Nº4
int RECONECT = 20;                         // Segundos hasta nuevo intento de conexión
const int relay1 = 14;                     // Lo conectamos a D5
const int relay2 = 12;                     // Lo conectamos a D6
const int relay3 = 13;                     // Lo conectamos a D7
const int relay4 = 3;                      // Lo conectamos a RX
const int button1 = 5;                     // Boton1 al D1
const int button2 = 4;                     // Boton2 al D2
const int button3 = 0;                     // Boton3 al D3
const int button4 = 2;                     // Boton4 al D4

///// FIN PARAMETROS CONFIGURABLES /////

//// Configuracion luz 1 ////
int val1 = 0;                                       // Guardamos el estado del interruptor
int state1 = 1;                                     // Establecemos un estado dependiendo del valor del interruptor para encender o apagar la luz
int old_state1 = 1;                                 // Guardamos un estado anterior de state1
int old_val1 = 0;                                   // Guardamos el estado anterior del interruptor
unsigned long tiempoC1;                             // Lo utilizamos para medir el tiempo que el interruptor está cerrado
unsigned long tiempoA1;                             // Lo utilizamos para medir el tiempo que el interruptor está abierto
unsigned long tiempomax1 = 100;                     // Tiempo que le damos para evitar efecto rebote
bool flancoC1 = false;                              // Flanco para activar el temporizador del interruptor en estado cerrado
bool flancoA1 = false;                              // Flanco para activar el temporizador del interruptor en estado abierto
const char* topic_relay1 = MQTT_TOPIC1 "/comando";  // Topic específico para recibir los mensajes de MQTT

//// Configuracion luz 2 ////
int val2 = 0;
int state2 = 1;
int old_state2 = 1;
int old_val2 = 0;
unsigned long tiempoC2;
unsigned long tiempoA2;
unsigned long tiempomax2 = 100;
bool flancoC2 = false;
bool flancoA2 = false;
const char* topic_relay2 = MQTT_TOPIC2 "/comando";

//// Configuracion luz 3 ////
int val3 = 0;
int state3 = 1;
int old_state3 = 1;
int old_val3 = 0;
unsigned long tiempoC3;
unsigned long tiempoA3;
unsigned long tiempomax3 = 100;
bool flancoC3 = false;
bool flancoA3 = false;
const char* topic_relay3 = MQTT_TOPIC3 "/comando";

//// Configuracion luz 4 ////
int val4 = 0;
int state4 = 1;
int old_state4 = 1;
int old_val4 = 0;
unsigned long tiempoC4;
unsigned long tiempoA4;
unsigned long tiempomax4 = 100;
bool flancoC4 = false;
bool flancoA4 = false;
const char* topic_relay4 = MQTT_TOPIC4 "/comando";

bool flancowifi = true;   // Flanco para activar la funcion de conectar a Wifi
bool flancomqtt = false;  // Flanco para activar la funcion de conectar a MQTT
bool contador = false;    // Flanco para activar la funcion de contador para intentar nueva conexión a Wifi
int cont = 0;             // Variable para contar los ciclos

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  state1 = digitalRead(button1);  // Leemos el estado del interruptor y ponemos el estado en el mismo valor para que las luces estén apagadas al inicio
  old_state1 = state1;            // Estblecemos los dos estados con el mismo valor para que las luces estén apagadas al inicio
  state2 = digitalRead(button2);
  old_state2 = state2;
  state3 = digitalRead(button3);
  old_state3 = state3;
  state4 = digitalRead(button4);
  old_state4 = state4;

  Serial.begin(9600);  // Iniciamos Serial
  Serial.println("Iniciando");

  pinMode(relay1, OUTPUT);  // Establecemos los relés como salida
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay1, HIGH);  // Indicamos que los relés estén apagados de inicio, depende del modelo (si es necesario, invertir los valores)
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  pinMode(button1, INPUT_PULLUP);  // Establecemos los interruptores en Pull_up para utilizar la resistencia interna del dispositivo
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  client.setServer(mqtt_server, mqttport);  // Iniciamos setServer con los datos porpocionados
  client.setCallback(callback);             // Iniciamos Callback para recibir mensajes de MQTT

  // Cargamos la configuración para la conexión OTA
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
  ArduinoOTA.begin();  // Iniciamos OTA

  Serial.println("Finalizado");
}

void loop() {
  ArduinoOTA.handle();

  //// INTERRUPTOR 1 ////

  val1 = digitalRead(button1);  // Leemos el estado del interruptor

  if (val1 == 0 && old_val1 == 1) {  // Si cambia el estado del interrupor de abierto a cerrado:
    tiempoC1 = millis();             // Establecemos tiempoC1 con el valor de los milisegundos actuales
    flancoC1 = true;                 // Establecemos flancoC1 en true pata iniciar el contador antirrebote
  }

  if (val1 == 1 && old_val1 == 0) {  // Si cambia el estado del interrupor de cerrado a abierto:
    tiempoA1 = millis();             // Establecemos tiempoA1 con el valor de los milisegundos actuales
    flancoA1 = true;                 // Establecemos flancoA1 en true pata iniciar el contador antirrebote
  }

  old_val1 = val1;  // Volvemos a igualar los valores para poder leer los próximos cambios

  if (flancoC1 == true && val1 == 0)          // Si el contador antirrebote está activa y el interruptor sigue en estado cerrado:
    if (millis() - tiempoC1 >= tiempomax1) {  // Si ha pasado el tiempo estipulado para el antirrebote:
      state1 = 1 - state1;                    // Cambiamos el estado de state, con lo cual, si esta encendida la luz, se apagará y si está apagada, se encenderá
      flancoC1 = false;                       // Establecemos flancoC1 en false para desactivar el contador antirrebote
    }

  if (flancoA1 == true && val1 == 1)          // Si el contador antirrebote está activa y el interruptor sigue en estado abierto:
    if (millis() - tiempoA1 >= tiempomax1) {  // Si ha pasado el tiempo estipulado para el antirrebote:
      state1 = 1 - state1;                    // Cambiamos el estado de state, con lo cual, si esta encendida la luz, se apagará y si está apagada, se encenderá
      flancoA1 = false;                       // Establecemos flancoA1 en false para desactivar el contador antirrebote
    }

  if (state1 == 1 && old_state1 == 0) {                //Si state cambia de 0 a 1:
    digitalWrite(relay1, LOW);                         // Encendemos la luz
    Serial.println("Luz 1 encedida");                  // Mostramos por Serial el mensaje
    client.publish(MQTT_TOPIC1 "/estado", "1", true);  // Enviamos por MQTT el estado, en este caso "1" que equivale a luz encendida
  }

  if (state1 == 0 && old_state1 == 1) {                // Si estate cambia de 1 a 0:
    digitalWrite(relay1, HIGH);                        // Apagamos la luz
    Serial.println("Luz 1 apagada");                   // Mostramos por Serial el mensaje
    client.publish(MQTT_TOPIC1 "/estado", "0", true);  // Enviamos por MQTT el estado, en este caso "0" que equivale a luz apagada
  }

  old_state1 = state1;  // Igualamos los estados para poder leer los próximos cambios

  //// INTERRUPTOR 2 ////

  val2 = digitalRead(button2);

  if (val2 == 0 && old_val2 == 1) {
    tiempoC2 = millis();
    flancoC2 = true;
  }

  if (val2 == 1 && old_val2 == 0) {
    tiempoA2 = millis();
    flancoA2 = true;
  }

  old_val2 = val2;

  if (flancoC2 == true && val2 == 0)
    if (millis() - tiempoC2 >= tiempomax2) {
      state2 = 1 - state2;
      flancoC2 = false;
    }

  if (flancoA2 == true && val2 == 1)
    if (millis() - tiempoA2 >= tiempomax2) {
      state2 = 1 - state2;
      flancoA2 = false;
    }

  if (state2 == 1 && old_state2 == 0) {

    digitalWrite(relay2, LOW);
    Serial.println("Luz 2 encedida");
    client.publish(MQTT_TOPIC2 "/estado", "1", true);
  }

  if (state2 == 0 && old_state2 == 1) {
    digitalWrite(relay2, HIGH);
    Serial.println("Luz 2 apagada");
    client.publish(MQTT_TOPIC2 "/estado", "0", true);
  }

  old_state2 = state2;

  //// INTERRUPTOR 3 ////

  val3 = digitalRead(button3);

  if (val3 == 0 && old_val3 == 1) {
    tiempoC3 = millis();
    flancoC3 = true;
  }

  if (val3 == 1 && old_val3 == 0) {
    tiempoA3 = millis();
    flancoA3 = true;
  }

  old_val3 = val3;

  if (flancoC3 == true && val3 == 0)
    if (millis() - tiempoC3 >= tiempomax3) {
      state3 = 1 - state3;
      flancoC3 = false;
    }

  if (flancoA3 == true && val2 == 1)
    if (millis() - tiempoA3 >= tiempomax3) {
      state3 = 1 - state3;
      flancoA3 = false;
    }

  if (state3 == 1 && old_state3 == 0) {

    digitalWrite(relay3, LOW);
    Serial.println("Luz 3 encedida");
    client.publish(MQTT_TOPIC3 "/estado", "1", true);
  }

  if (state3 == 0 && old_state3 == 1) {
    digitalWrite(relay3, HIGH);
    Serial.println("Luz 3 apagada");
    client.publish(MQTT_TOPIC3 "/estado", "0", true);
  }

  old_state3 = state3;

  //// INTERRUPTOR 4 ////

  val4 = digitalRead(button4);

  if (val4 == 0 && old_val4 == 1) {
    tiempoC4 = millis();
    flancoC4 = true;
  }

  if (val4 == 1 && old_val4 == 0) {
    tiempoA4 = millis();
    flancoA4 = true;
  }

  old_val4 = val4;

  if (flancoC4 == true && val4 == 0)
    if (millis() - tiempoC4 >= tiempomax4) {
      state4 = 1 - state4;
      flancoC4 = false;
    }

  if (flancoA4 == true && val4 == 1)
    if (millis() - tiempoA4 >= tiempomax4) {
      state4 = 1 - state4;
      flancoA4 = false;
    }

  if (state4 == 1 && old_state4 == 0) {

    digitalWrite(relay4, LOW);
    Serial.println("Luz 4 encedida");
    client.publish(MQTT_TOPIC4 "/estado", "1", true);
  }

  if (state4 == 0 && old_state4 == 1) {
    digitalWrite(relay4, HIGH);
    Serial.println("Luz 4 apagada");
    client.publish(MQTT_TOPIC4 "/estado", "0", true);
  }

  old_state4 = state4;

  //// FIN CODIGO INTERRUPTORES ////

  //// CONECTAMOS A WIFI Y MQTT ////

  if ((WiFi.status() != WL_CONNECTED) && (flancowifi == true)) {  // Si no estamos conectados a Wifi y "flancowifi" está activado:
    setup_wifi();                                                 // Llamamos a la funcion "setup_wifi"
    reconnect();                                                  // Llamamos a la funcion "reconnect"
    client.setServer(mqtt_server, mqttport);                      // Iniciamos "setServer" con los datos porpocionados
    client.setCallback(callback);                                 // Iniciamos "Callback" para recibir mensajes de MQTT
  }
  if (!client.connected()) {  // Si seguimos sin estar conectados:
    contador = true;          // Activamos el "contador"
    cont++;                   // "cont" suma de 1 en 1 en cada ciclo
    delay(10);                // Esperamos 10 milisegundos
  }

  if ((contador == true) && (cont == 1000)) {  // Si el "contador" está activado y "cont" ha llegado a 1000 ciclos (10 segundos)
    flancomqtt = true;                         // Activamos "flancomqtt"
    flancowifi = true;                         // Activamos "flancowifi"
    setup_wifi();                              // Iniciamos la conexión a Wifi
    reconnect();                               // Iniciamos la función "reconnect"
  }
  client.loop();  // Esta función revisa en cada ciclo si hay que enviar o leer mensaje por MQTT
}

void setup_wifi() {  // Configuración para conectar a Wifi
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");  //  Mostramos por serial el Wifi que nos queremos conectar
  Serial.println(ssid);           // Muestra el "SSID" que configuramos
  WiFi.mode(WIFI_STA);            // Configuración de tipo de conexión
  WiFi.begin(ssid, password);     // Configuración de nombre de "SSID" y su password

  while ((WiFi.status() != WL_CONNECTED) && (flancowifi == true)) {  // Si no estamos conectados a Wifi y "flancowifi" está activado:
    for (int i = 0; i <= 5; i++) {                                   // Iniciamos una cuenta de 5 ciclos
      Serial.println(i);                                             // Imprimimos el conteo por serial
      delay(1000);                                                   // Espermos 1 segundo en cada ciclo
      if (i == 5) {                                                  // Si pasados los 5 segundos (5 ciclos) no hemos conectado:
        flancowifi = false;                                          // Desactivamos "flancowifi" para no volver a reconectar hasta el tiempo estipulado
        contador = true;                                             // Iniciamos el "contador"
        break;                                                       // Salimos de la funcion "for"
      }
    }
  }
  if (WiFi.status() != WL_CONNECTED) {                                    // Si no hemos conectado tras el intento:
    Serial.print("No se ha podido conectar al Wifi, nuevo intento en ");  // Esperamos para conectar al Wifi durante los segundos configurados.
    Serial.print(RECONECT);                                               // Mostramos los segundos configurados
    Serial.println(" segundos...");                                       //
    flancowifi = false;                                                   // Desactivamos "flancowifi" para no volver a conectar hasta pasados los segundos estipulados en RECONECT

  } else {                             // Si no ocurre lo anterior:
    Serial.println("");                //
    Serial.println("WiFi conectado");  // Confirmamos que hemos conectado a Wifi
    Serial.println("Dirección IP: ");  // Y mostramos la IP otorgada por el router
    Serial.println(WiFi.localIP());    // IP otorgada por el router
    cont = 0;                          // Establecemos en "cont" a cero
    flancowifi = false;                // Establecemos "flancowifi" a false porque ya estamos conectados
    contador = false;                  // Establecemos "contador" a false porque ya estamos conectados
  }
}

void callback(char* topic, byte* payload, unsigned int length) {  // Función para poder recibir los mensajes por MQTT
  Serial.print("Mensaje recivido [");                             // Mostramos por serial cuando recibimos un mensaje
  Serial.print(topic);                                            // Mostramos el topic que recibimos
  Serial.print("] ");                                             //
  for (int i = 0; i < length; i++) {                              //
    Serial.print((char)payload[i]);                               // Muestra el mensaje
  }
  Serial.println();

  if (strcmp(topic, topic_relay1) == 0) {  // Si recibimos un mensaje del topic configurado en "topic_relay1":
    if ((char)payload[0] == '1') {         // Si el mensaje es "1":
      state1 = 1;                          // Establecemos "state1" en "1" (encendemos la luz)
    } else {                               // Si el mensaje recibido no es "1":
      state1 = 0;                          // Establecemos "state1" en "0" (apagamos la luz)
    }
  }

  if (strcmp(topic, topic_relay2) == 0) {
    if ((char)payload[0] == '1') {
      state2 = 1;

    } else {
      state2 = 0;
    }
  }

  if (strcmp(topic, topic_relay3) == 0) {
    if ((char)payload[0] == '1') {
      state3 = 1;

    } else {
      state3 = 0;
    }
  }

  if (strcmp(topic, topic_relay4) == 0) {
    if ((char)payload[0] == '1') {
      state4 = 1;

    } else {
      state4 = 0;
    }
  }
}

void reconnect() {  // Función para reconectar

  while ((!client.connected()) && (flancomqtt == true)) {    // Si no estamos conectados a MQTT y "flancowifi" lo tenemos activado:
    Serial.println("Conectando con MQTT");                   // Mostramos por serial que estamos intentando conectar
    if (client.connect(CLIENT_ID, mqttusuario, mqttpass)) {  // Si conectamos a Wifi, establecemos la configuración para conectarnos
      Serial.println("conectado");                           // Mostramos que hemos conectado por serial
      String state1_ = String(state1);                       // Pasamos el valor de los "state" a string para poder enviarlos por MQTT
      String state2_ = String(state2);
      String state3_ = String(state3);
      String state4_ = String(state4);
      client.publish(MQTT_TOPIC1 "/estado", state1_.c_str(), true);  // Publicamos los valores de los "state" en su topic correspondiente por MQTT para indicar a Home Assistant si esta encendida o apagada la luz conrrespondiente
      client.publish(MQTT_TOPIC2 "/estado", state2_.c_str(), true);
      client.publish(MQTT_TOPIC3 "/estado", state3_.c_str(), true);
      client.publish(MQTT_TOPIC4 "/estado", state4_.c_str(), true);
      client.subscribe(MQTT_TOPIC1 "/comando");  // Nos suscribimos a los topic para recibir las ordenes de apagar o encender
      client.subscribe(MQTT_TOPIC2 "/comando");
      client.subscribe(MQTT_TOPIC3 "/comando");
      client.subscribe(MQTT_TOPIC4 "/comando");
    } else {                                                        // Si no hemos podido conectar:
      Serial.print("No se ha podido conectar, nuevo intento en ");  // Intenamos conectar al servidos MQTT durante los segundos configurados.
      Serial.print(RECONECT);                                       // Mostramos los segundos configurados en "RECONECT"
      Serial.println(" segundos...");
      cont = 0;            // Establecemos "cont" a cero
      flancowifi = false;  // Establecemos "flancowifi" en false para no volver a internar la reconexión hasta que pasen los segundos configurados
      flancomqtt = false;  // Establecemos "flancomqtt" en false para no volver a internar la reconexión hasta que pasen los segundos configurados
      contador = true;     // Establecemos "contador" en true para iniciar el contador de los segundos establecidos he intentar la reconexión pasado ese tiempo
    }
  }
}
