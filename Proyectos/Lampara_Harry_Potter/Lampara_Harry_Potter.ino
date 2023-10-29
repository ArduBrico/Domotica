////   WWW.YOUTUBE.COM/@ARDUBRICO    ////

#include <FastLED.h>
#define DATA_PIN 2
#define NUM_LEDS 30

double n;
double ni;
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;

#define MIN_VARIATION 0
#define MAX_VARIATION 30
#define MIN_INTENSITY 0.1
#define MAX_INTENSITY 0.7
#define NOISE_SPEED_COLOR 5
#define NOISE_SPEED_INTENSITY 3

bool ledsOn = false; 
bool buttonPressed = false;
int buttonState = LOW;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

CRGBPalette16 colorPalettes[] = {

  CRGBPalette16(CRGB::Red, CRGB::Black, CRGB::Black),  // Rojo
  CRGBPalette16(CRGB::Blue, CRGB::Black, CRGB::Black),  // Azul
  CRGBPalette16(CRGB::Yellow, CRGB::Black, CRGB::Black),  // Amarillo
  CRGBPalette16(CRGB::Green, CRGB::Black, CRGB::Black)  // Verde
};

int currentColorIndex = -1; // Índice del color actual

const int buttonPin = 3; // Pin D3 para el botón

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
  // Apaga todos los LEDs en el inicio
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  
  pinMode(buttonPin, INPUT_PULLUP);

  // Establece el último estado del botón al estado actual
  lastButtonState = digitalRead(buttonPin);

  // Selección aleatoria de la paleta de colores al inicio
  randomSeed(analogRead(A0)); // Semilla aleatoria basada en una lectura analógica
  currentColorIndex = random(sizeof(colorPalettes) / sizeof(colorPalettes[0]));
  currentPalette = colorPalettes[currentColorIndex];
  Serial.println("Paleta de colores inicial seleccionada aleatoriamente");
}


void loop() {
  // Detecta pulsación del botón
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        buttonPressed = true;
        Serial.println("Botón pulsado");
      }
    }
  }

  lastButtonState = reading;

  if (buttonPressed) {
    if (!ledsOn) {
      // Si los LEDs están apagados, enciende aleatoriamente
      changeRandomColor();
      ledsOn = true;
      Serial.println("LEDs encendidos y cambio de color aleatorio");
    } else {
      // Si los LEDs están encendidos, apaga
      clearLEDs();
      ledsOn = false;
      Serial.println("LEDs apagados");
    }
    buttonPressed = false;
  }

  if (ledsOn) {
    renderLEDs(); // Renderiza solo si los LEDs están encendidos
  }
}

unsigned int lastTime = 0;
void renderLEDs() {
  unsigned int time = millis();
  lastTime = time;

  for (int i = 0; i < NUM_LEDS; i++) {
    n = inoise8(i*250 , (time+i)/NOISE_SPEED_COLOR);
    ni = inoise8(i*500 , (time+i)/NOISE_SPEED_INTENSITY);

    float v = QuadraticEaseInOut(n/255);
    float vi = QuadraticEaseInOut(ni/255);
    
    vi = (MAX_INTENSITY - MIN_INTENSITY) * v + MIN_INTENSITY;
    
    CRGB color = ColorFromPalette(currentPalette, int(vi * 255));
    leds[i] = color;
  }
  FastLED.show();
}

void clearLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void changeRandomColor() {
  int newColorIndex;
  do {
    newColorIndex = random(sizeof(colorPalettes) / sizeof(colorPalettes[0]));
  } while (newColorIndex == currentColorIndex); // Asegura que el nuevo color sea diferente al actual

  currentColorIndex = newColorIndex;
  currentPalette = colorPalettes[currentColorIndex];
  Serial.println("Cambio de color a paleta aleatoria");
}

float CubicEaseInOut(float p) {
  if (p < 0.5) {
    return 4 * p * p * p;
  } else {
    float f = ((2 * p) - 2);
    return 0.5 * f * f * f + 1;
  }
}

float QuadraticEaseInOut(float p) {
  if (p < 0.5) {
    return 2 * p * p;
  } else {
    return (-2 * p * p) + (4 * p) - 1;
  }
}
