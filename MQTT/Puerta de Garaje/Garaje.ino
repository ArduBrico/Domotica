/*   ###### VISITA WWW.YOUTUBE.COM/@ARDUBRICO ######

Configuración que debes poner en el archivo mqtt.yaml

  cover:
    - name: "Garaje"
      unique_id: "Puerta_Garaje"
      device_class: garage
      command_topic: "garaje/puerta/comando"
      state_topic: "garaje/puerta/estado"
      qos: 0
      retain: true
      payload_open: "1"
      payload_close: "2"
      payload_stop: "3"
      state_open: "1"
      state_opening: "2"
      state_closed: "3"
      state_closing: "4"
      state_stopped: "5"
      optimistic: true
 */     

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

const char* ssid  = "SSID"; // Nombre de nuestra red Wifi
const char* password = "CONTRASEÑA"; // Contraseña de nuestra ede Wifi
const char* mqtt_server = "XXX.XXX.X.XX"; // I.P. de nuestro servidor MQTT (Home Assistant)
int mqttport = 1883;
const char* mqttusuario = "MQTT"; // Nombre de usuario MQTT en Home Assistant
const char* mqttpass = "XXX"; // Contraseña para el usuario de MQTT
#define CLIENT_ID   "Puerta_Garaje"  // Nombre que le queramos dar al dispositivo                 
#define MQTT_TOPIC  "garaje"         // Topic raíz
#define LUZ   "garaje/luces/comando" // Topic para encender la luz del garaje si la tenemos domotizada

WiFiClient espClient;
PubSubClient client(espClient);
      
const int relay1 = 13; //Lo conectamos a D7
const int botonAbierta = 5; // Sensor puerta garaje a D1 PUERTA ABIERTA
const int botonCerrada = 4; // Sensor puerta garaje a D2 PUERTA CERRADA
int AbrirPuerta = 0;
int CerrarPuerta = 0;
const char* estadoParada = "0";
const char* Parar = "0";
const char* estadoPuerta = "Sin estado";
int estadoAbierta = 0; 
int old_estadoAbierta = 1; 
int estadoCerrada = 0; 
int old_estadoCerrada = 1; 
int valAbierta = 0; 
int old_valAbierta = 1;
int valCerrada = 0; 
int old_valCerrada = 1; 
unsigned long tiempopuertaabierta;
unsigned long tiempoAbierta;
unsigned long tiempoCerrada;
bool flancopuertaabierta = false;
bool flancoAbierta = false;
bool flancoCerrada = false;
unsigned long tiempopuertacerrada;
bool flancopuertacerrada = false;
bool flancoEstadoCerrada = false;
bool flancoEstadoCerrando = false;
bool flancoEstadoAbierta = false;
bool flancoEstadoAbriendo = false;
unsigned long tiempo1;
unsigned long tiempomax1 = 150;
const char* topic_comando = MQTT_TOPIC"/puerta/comando"; //comando

void setup(){
  Serial.begin(9600);
  Serial.println("setup begin");

 
  digitalWrite(relay1, HIGH);     
  pinMode(relay1, OUTPUT);
  delay(10);

  pinMode(botonCerrada, INPUT_PULLUP);
  pinMode(botonAbierta, INPUT_PULLUP);

  valAbierta= digitalRead(botonAbierta);
  valCerrada= digitalRead(botonCerrada);
 


  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // OTA setup
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
  ArduinoOTA.begin();

  Serial.println("setup end");
 }


void loop(){
   ArduinoOTA.handle();

      //PUERTA ABIERTA//
    valAbierta= digitalRead(botonAbierta);
      
  if ((valAbierta == LOW) && (old_valAbierta == 1)){ // flanco de 1 a 0
    tiempopuertaabierta = millis();  // inicias la cuenta      
    flancopuertaabierta = true; 
     
   }
  else if ((valAbierta == HIGH) && (old_valAbierta == 0 )){ // flanco de 1 a 0
    tiempopuertacerrada = millis();  // inicias la cuenta      
    flancopuertacerrada = true; 
   
   }


 old_valAbierta = valAbierta;        // guardo ultimo cambio

 if (flancopuertaabierta &&(valAbierta == LOW))     // durante el tiempo que val1 esta en LOW
    if (millis()-tiempopuertaabierta >= tiempomax1){ // se supero el segundo?
      Serial.println("Abierta ON"); 
       estadoAbierta = 1;
       old_estadoAbierta = 0;
       flancopuertaabierta = false;
       flancoEstadoAbierta = true;
    }

    if (flancoEstadoAbierta && (estadoAbierta == 1) && (old_estadoAbierta == 0) && (estadoCerrada == 0) && (old_estadoCerrada == 0)){ 
      
        estadoPuerta = "1";
        client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
        Serial.println(estadoPuerta);
        flancoEstadoAbierta = false;
 
  }   

  old_estadoAbierta = estadoAbierta;  
  
  if (flancopuertacerrada &&(valAbierta == HIGH))     // durante el tiempo que val1 esta en LOW
    if (millis()-tiempopuertacerrada >= tiempomax1){ // se supero el segundo?
      Serial.println("Abierta OFF"); 
        estadoAbierta = 0;
        old_estadoAbierta = 1;
       flancopuertacerrada = false;
       flancoEstadoCerrando = true;

    }
        if (flancoEstadoCerrando && (estadoAbierta == 0) && (old_estadoAbierta == 1) && (estadoCerrada == 0) && (old_estadoCerrada == 0)){ 
          
          estadoPuerta = "4";
          client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
          Serial.println(estadoPuerta);
          flancoEstadoCerrando = false;
 
  }   

    old_estadoAbierta = estadoAbierta;  
  
     //PUERTA CERRADA

     valCerrada= digitalRead(botonCerrada);
      
 if ((valCerrada == LOW) && (old_valCerrada == 1)){ // flanco de 1 a 0
    tiempoAbierta = millis();  // inicias la cuenta      
    flancoAbierta = true;      // 
 }
 else if ((valCerrada == HIGH) && (old_valCerrada == 0 )){ // flanco de 1 a 0
    tiempoCerrada = millis();  // inicias la cuenta      
    flancoCerrada = true;      // 
 }

 old_valCerrada = valCerrada;        // guardo ultimo cambio

 if (flancoAbierta &&(valCerrada == LOW))     // durante el tiempo que val1 esta en LOW
    if (millis()-tiempoAbierta >= tiempomax1){ // se supero el segundo?
      Serial.println("Cerrada ON"); 
       estadoCerrada = 1;
       old_estadoCerrada = 0;
       flancoAbierta = false;
       flancoEstadoCerrada = true;
       
    }
        if (flancoEstadoCerrada && (estadoAbierta == 0) && (old_estadoAbierta == 0) && (estadoCerrada == 1) && (old_estadoCerrada == 0)){ 
          
          estadoPuerta = "3";
          client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
          Serial.println(estadoPuerta);
          flancoEstadoCerrada = false;
  }  
      old_estadoCerrada = estadoCerrada;  
      
 if (flancoCerrada &&(valCerrada == HIGH))     // durante el tiempo que val1 esta en LOW
    if (millis()-tiempoCerrada >= tiempomax1){ // se supero el segundo?
      Serial.println("Cerrada OFF"); 
       estadoCerrada = 0;
       old_estadoCerrada = 1;
       flancoCerrada = false;
       flancoEstadoAbriendo = true;

    }

     if (flancoEstadoAbriendo && (estadoAbierta == 0) && (old_estadoAbierta == 0) && (estadoCerrada == 0) && (old_estadoCerrada == 1)){ 
      
           estadoPuerta = "2";
           client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
           client.publish(LUZ, "1", true);
           Serial.println(estadoPuerta);
           flancoEstadoAbriendo = false;

 
 }  
      old_estadoCerrada = estadoCerrada;  

  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
    reconnect();
    client.setServer(mqtt_server, mqttport);
    client.setCallback(callback);
  } 
      
  // reconnect when MQTT is not connected
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 }

void abrir(){
  if ((AbrirPuerta == 1) && (estadoPuerta == "3")){
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10);   
     AbrirPuerta = 0;
     estadoParada = "0";
     Serial.println("abriendo desde cerrada");
     Serial.println(estadoParada);
     }
 else if ((AbrirPuerta == 1) && (estadoParada == "paraBajada")){
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10);   
     AbrirPuerta = 0;
     estadoParada = "0";
     estadoPuerta = "2";
     Serial.println("abriendo desde parada");
     Serial.println(estadoParada);
     client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
     }

 else if ((AbrirPuerta == 1) && (estadoParada == "paraSubida") && (estadoPuerta == "5")){
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH); 
     delay(500);
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH); 
     delay(10); 
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH); 
     delay(10); 
     AbrirPuerta = 0;
     estadoParada = "0";
     estadoPuerta = "2";
     Serial.println("abriendo desde parada subiendo");
     Serial.println(estadoParada);
     client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
     }

 else if ((AbrirPuerta == 1) && (estadoPuerta == "4") && (estadoAbierta == 0)){
         digitalWrite(relay1, LOW);
         delay(500);
         digitalWrite(relay1, HIGH); 
         delay(500);
         digitalWrite(relay1, LOW);
         delay(500);
         digitalWrite(relay1, HIGH); 
         delay(10);  
         AbrirPuerta = 0; 
         estadoPuerta = "2";
         estadoParada = "0";
         Serial.println("abriendo desde bajando");
         Serial.println(estadoParada);
         client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
    } else {
      //estadoParada = "0";
      delay(10); 
      
    }
    
  }

void cerrar(){

  if ((CerrarPuerta == 1) && (estadoPuerta == "1")){
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10);   
     CerrarPuerta = 0;
     Parar = "0";
     Serial.println("cerrando desde abierta");
     Serial.println(estadoParada);
     }

  else if ((CerrarPuerta == 1) && (estadoParada == "paraSubida")){
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10);   
     CerrarPuerta = 0;
     Parar = "0";
     estadoPuerta = "4";
     Serial.println("cerrando desde parada");
     Serial.println(estadoParada);
     client.publish(MQTT_TOPIC"/puertan/estado", estadoPuerta, true);
     }
  
  else if ((CerrarPuerta == 1) && (estadoParada == "paraBajada") && (estadoPuerta == "5")){
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10);
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10); 
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10);    
     CerrarPuerta = 0;
     Parar = "0";
     estadoPuerta = "4";
     Serial.println("cerrando desde parada bajada");
     Serial.println(estadoParada);
     client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
     }

  else if ((CerrarPuerta == 1) && (estadoPuerta == "2") && (estadoCerrada == 0)){
         digitalWrite(relay1, LOW);
         delay(500);
         digitalWrite(relay1, HIGH); 
         delay(500);
         digitalWrite(relay1, LOW);
         delay(500);
         digitalWrite(relay1, HIGH); 
         delay(10);  
         CerrarPuerta = 0; 
         estadoPuerta = "4";
         Parar = "0";
         Serial.println("cerrando desde bajando");
         Serial.println(estadoParada);
         client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
    } else {
      estadoParada == "0";
      delay(10); 
      
    }
    
   }
void parar(){

   if ((Parar == "1") && (estadoPuerta == "2") && (estadoParada == "0")){
     digitalWrite(relay1, LOW);
     delay(500);
     digitalWrite(relay1, HIGH);  
     delay(10);   
     estadoParada = "paraSubida";
     estadoPuerta = "5";
     Serial.println(estadoParada);
     client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
     }

      else if ((Parar == "1") && (estadoPuerta == "4") && (estadoParada == "0")){
         digitalWrite(relay1, LOW);
         delay(500);
         digitalWrite(relay1, HIGH); 
         delay(10);  
         estadoParada = "paraBajada"; 
         estadoPuerta = "5";
         Serial.println(estadoParada);
         client.publish(MQTT_TOPIC"/puerta/estado", estadoPuerta, true);
    } else {
       //estadoParada = "0";
      delay(10); 
    }
    
 }
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  WiFi.mode(WIFI_STA); //ESP in connect to network mode
  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) {
    // wait 50 * 10 ms = 500 ms to give ESP some time to connect (need to give delay else you get soft WDT reset)
      for (int i=0; i <= 50; i++){
        delay(10);
        if (i == 50) { 
          Serial.println(".");
        } else {
          Serial.print(".");
        }     
      }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 }

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //switch on relay1 LED if topic is correct and 1 is received
  if (strcmp(topic, topic_comando)==0){      
    if ((char)payload[0] == '1') {     
      AbrirPuerta = 1;
      abrir();      
          
    } else if ((char)payload[0] == '2'){
     CerrarPuerta = 1;
     cerrar();     
    }

    else if ((char)payload[0] == '3'){
     Parar = "1";
     parar();
       }
  else {
    delay(10);
  }
  }
 }

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CLIENT_ID, mqttusuario, mqttpass)) {
      Serial.println("connected");
      client.subscribe(MQTT_TOPIC"/puerta/comando");    //subscribe relay to MQTT server, you can set your name here
          } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait (500 * 10ms = 5000ms) 5 seconds before retrying, but read button press meanwhile
      for (int i=0; i <= 500; i++){
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
