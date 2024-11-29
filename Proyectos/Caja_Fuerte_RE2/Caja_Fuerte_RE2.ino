#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Servo.h>


#define LED_PIN A0         // Pin de la tira de LEDs
#define NUM_LEDS 8         // Número de LEDs en la tira
#define BUTTONS_COUNT 8    // Número de botones
#define SEQUENCE_LENGTH 8  // Longitud de la secuencia
Servo myservo;             //creamos un objeto servo

SoftwareSerial mySerial(11, 12);  // RX, TX

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
int buttonsPins[BUTTONS_COUNT] = { 2, 3, 4, 5, 6, 7, 8, 9 };
int sequence[SEQUENCE_LENGTH] = { 2, 3, 4, 5, 6, 7, 8, 9 };
bool ledsState[NUM_LEDS] = { false };  // Estado de los LEDs (apagado/inicialmente)
unsigned long previousMillis = 0;      // Variable para almacenar el tiempo anterior
const long interval = 50;              // Intervalo de espera en milisegundos
int buttonPressed;
int currentStep = 0;                      // Paso actual en la secuencia
int correctButton;                        // Botón correcto establecido por el usuario
bool sequenceCompleted = false;           // Variable para indicar si la secuencia se ha completado
unsigned long previousBlinkMillis = 0;    // Variable para controlar el parpadeo
const unsigned long blinkInterval = 500;  // Intervalo de parpadeo en milisegundos
bool blinkState = false;                  // Estado de parpadeo
int blinkCount = 0;                       // Contador de parpadeo
bool interruptorEstado = false;           // Estado del interruptor
int randomNumber;
const char* orden = "A7:0000";
int sonido = 0;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);  // Inicializa el puerto serial
  strip.begin();
  strip.show();  // Inicializa todos los LEDs apagados
  for (int i = 0; i < BUTTONS_COUNT; i++) {
    pinMode(buttonsPins[i], INPUT_PULLUP);
  }

  // Configurar el reproductor de MP3 (JR6001)
  mySerial.println("AF:30");  // Configurar el volumen a 30
  delay(100);

  // Lee el estado del interruptor
  pinMode(A2, INPUT_PULLUP);  // Añadido interruptor en A2
  interruptorEstado = digitalRead(A2);
  randomSeed(analogRead(A5));

  // Genera una nueva secuencia aleatoria si el interruptor está en ON
  if (interruptorEstado == LOW) {

    //Escribimos el numero aleatorio por el puerto serie
    Serial.print("El numero aleatorio es = ");
    Serial.println(randomNumber);
    SecAl();
  }

  myservo.attach(10);
  myservo.write(45);
  delay(500);
  myservo.detach();
}

void shuffleArray(int arr[], int size) {
  for (int i = size - 1; i > 0; i--) {
    int j = random(i + 1);  // Genera un índice aleatorio entre 0 y i
    // Intercambia los elementos en las posiciones i y j
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void SecAl() {
  // Arreglo con los números del 2 al 9
  int numeros[] = { 2, 3, 4, 5, 6, 7, 8, 9 };

  // Baraja aleatoriamente los números en el arreglo
  shuffleArray(numeros, 8);

  // Asigna los números barajados al arreglo buttonsPins
  for (int i = 0; i < BUTTONS_COUNT; i++) {
    buttonsPins[i] = numeros[i];
  }

  // Mostrar la secuencia actualizada por el puerto serial
  Serial.print("Nuevo orden de los botones: ");
  for (int i = 0; i < BUTTONS_COUNT; i++) {
    Serial.print(buttonsPins[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  checkButtons();
  checkSequenceComplete();

  // Control de parpadeo de LEDs
  unsigned long currentMillis = millis();
  if (blinkCount < 3) {  // Controla el parpadeo solo si no se han completado tres ciclos
    if (sequenceCompleted && (currentMillis - previousBlinkMillis >= blinkInterval)) {
      previousBlinkMillis = currentMillis;
      blinkState = !blinkState;  // Cambia el estado de parpadeo
      // Enciende o apaga los LEDs alternativamente
      for (int i = 0; i < NUM_LEDS; i++) {
        if (i % 2 == 0) {  // LEDs pares
          strip.setPixelColor(i, blinkState ? strip.Color(0, 255, 0) : strip.Color(0, 0, 0));
        } else {  // LEDs impares
          strip.setPixelColor(i, blinkState ? strip.Color(0, 0, 0) : strip.Color(0, 255, 0));
        }
      }
      strip.show();

      // Incrementa el contador de parpadeo si se completa un ciclo completo

      if (!blinkState) {
        blinkCount++;
      }
    }
  } else {  // Después de completar tres ciclos, apaga todos los LEDs
    delay(1000);
    mySerial.println("A7:00010");  // Enviar comando para reproducir el archivo predeterminado
    delay(300);
    myservo.attach(10);
    myservo.write(0);
    delay(500);
    myservo.write(0);
    delay(500);
    myservo.write(45);
    delay(500);
    myservo.detach();
    Serial.println("Reiniciando");
    delay(500);
    asm volatile("jmp 0x00");
    clearStrip();
    strip.show();
  }
}

void modificarSecuencia(int primerBoton) {
  int offset = 0;
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    sequence[i] = primerBoton + offset;
    if (sequence[i] > 9) {
      sequence[i] -= 8;  // Ajuste para que la secuencia se mantenga en el rango de botones
    }
    offset++;
  }
  correctButton = sequence[currentStep];  // Actualiza el botón correcto a seguir
  // Mostrar la secuencia actualizada por el puerto serial
  Serial.print("Secuencia actualizada: ");
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    Serial.print(sequence[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void checkButtons() {
  unsigned long currentMillis = millis();  // Obtener el tiempo actual

  for (int i = 0; i < BUTTONS_COUNT; i++) {
    if (digitalRead(buttonsPins[i]) == LOW) {  // Botón presionado
      buttonPressed = i + 2;                   // Ajuste para el número de botón presionado

      // Si la secuencia se ha completado, ignorar nuevas pulsaciones de botones
      if (sequenceCompleted) {
        return;
      }

      // Verifica si el LED correspondiente ya está encendido
      int ledIndex = buttonPressed - 2;  // Índice del LED correspondiente al botón
      if (ledIndex >= 0 && ledIndex < NUM_LEDS && ledsState[ledIndex]) {
        // El LED ya está encendido, no hacer nada
        return;
      }

      if (currentStep == 0) {  // Si es el primer botón de la secuencia
        modificarSecuencia(buttonPressed);
        Serial.print("Secuencia inicializada para el botón ");
        Serial.println(buttonPressed);
      }

      if (buttonPressed == correctButton) {      // Botón correcto en la secuencia
        int lastCorrectLed = buttonPressed - 2;  // Índice del LED correspondiente al botón
        if (lastCorrectLed >= 0 && lastCorrectLed < NUM_LEDS) {
          int sonido = currentStep + 1;
          mySerial.print(orden);  // Enviar el comando para el archivo específico
          mySerial.println(sonido);
          Serial.print("Sonido = ");
          Serial.println(sonido);
          strip.setPixelColor(lastCorrectLed, strip.Color(0, 255, 0));  // Enciende el LED correspondiente en verde
          ledsState[lastCorrectLed] = true;                             // Actualiza el estado del LED
          strip.show();
          Serial.print("LED ");
          Serial.print(lastCorrectLed);
          Serial.println(" encendido en verde.");
          currentStep++;  // Avanza en la secuencia
          if (currentStep < SEQUENCE_LENGTH) {
            correctButton = sequence[currentStep];  // Actualiza el próximo botón correcto
            Serial.print("Siguiente botón: ");
            Serial.println(correctButton);
          } else {
            // Se completó la secuencia, reiniciar para la próxima ronda
            sequenceCompleted = true;
            mySerial.println("A7:00001");  // Enviar comando para reproducir el archivo predeterminado
            delay(100);
            sonido = 9;
            mySerial.print(orden);  // Enviar el comando para el archivo específico
            mySerial.println(sonido);
            Serial.println("Sonido = ");
            Serial.println(sonido);
            Serial.println("Secuencia completada.");
          }
        }
      } else {  // Botón incorrecto
        errorFlash();
        clearStrip();
        strip.setPixelColor(buttonPressed - 2, strip.Color(0, 255, 0));  // Enciende el LED incorrecto en verde
        strip.show();
        int sonido = buttonPressed - 1;
        mySerial.print(orden);  // Enviar el comando para el archivo específico
        mySerial.println(sonido);
        Serial.print("Sonido = ");
        Serial.println(sonido);
        Serial.print("LED ");
        Serial.print(buttonPressed - 2);
        Serial.println(" encendido en verde.");
        ledsState[buttonPressed - 2] = true;  // Actualiza el estado del LED incorrecto

        // Reinicia la secuencia y el botón correcto
        currentStep = 1;
        modificarSecuencia(buttonPressed);
        correctButton = sequence[currentStep];
        Serial.println("Reiniciando secuencia.");
      }

      delay(10);                                     // Debouncing
      while (digitalRead(buttonsPins[i]) == LOW) {}  // Espera a que se suelte el botón
    }
  }
}

void checkSequenceComplete() {
  if (checkAllLedsOn()) {  // Verificar si todos los LEDs están encendidos

    if (!sequenceCompleted) {          // Si la secuencia se completó por primera vez
      sequenceCompleted = true;        // Marcar la secuencia como completada
      previousBlinkMillis = millis();  // Inicia el parpadeo
      blinkCount = 0;                  // Reinicia el contador de parpadeo
    }
  }
}

bool checkAllLedsOn() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (!ledsState[i]) {
      return false;  // Al menos un LED está apagado
    }
  }
  return true;  // Todos los LEDs están encendidos
}

void errorFlash() {
  for (int flashCount = 0; flashCount < 3; flashCount++) {  // Parpadear tres veces en rojo
    for (int i = 0; i < NUM_LEDS; i++) {
      if (ledsState[i]) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));  // Enciende en rojo el LED encendido en verde
      }
    }
    strip.show();
    delay(100);    // Tiempo para mostrar el parpadeo en rojo
    clearStrip();  // Apaga todos los LEDs y reinicia el estado
    delay(10);     // Tiempo antes del siguiente parpadeo
  }
}


void clearStrip() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Apaga todos los LEDs
    ledsState[i] = false;                          // Reinicia el estado del LED
  }
  strip.show();
  sequenceCompleted = false;  // Reinicia la variable de secuencia completada
}