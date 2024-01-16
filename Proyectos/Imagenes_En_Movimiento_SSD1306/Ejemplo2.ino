//// https://www.youtube.com/@ArduBrico ////

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // Definimos el ancho de nuetra pantalla OLED
#define SCREEN_HEIGHT 64  // Definimos el alto de nuestra pantalla OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  // Configuramos los parametros para la pantalla

const int botonAbrir = 12;  // Conectamos el pulsador de abrir a D6
const int botonArmar = 14;  // Conectamos el pulsador de armar a D5

int valAbrir = 0;  // Variable para guardar el estado del pulsador "abrir"
int valArmar = 0;  // Variable para guardar el estado del pulsador "armar"

// A continuación configuramos las imagenes que vamos a mostrar en la pantalla

// Imagen del logo
static const unsigned char PROGMEM logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7f, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xc1, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x80, 0x38, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x81, 0x80, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x80, 0x0c, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x80, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x80, 0x06, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x80, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x87, 0x82, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0xc0, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x86, 0xc2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x08, 0x43, 0xce, 0x1f, 0xfc, 0x78, 0x7c, 0x84, 0xc2, 0x7b, 0xdf, 0x03, 0xf8, 0x3f, 0x80,
  0x03, 0x18, 0x43, 0xfe, 0x3f, 0xfc, 0x78, 0x7c, 0x87, 0x86, 0x7f, 0xdf, 0x0f, 0xf8, 0xff, 0xc0,
  0x02, 0x18, 0x63, 0xfe, 0x7f, 0xfc, 0x78, 0x7c, 0x80, 0x04, 0x7f, 0xdf, 0x1f, 0xf9, 0xff, 0xe0,
  0x02, 0x1c, 0x23, 0xfe, 0x7e, 0xfc, 0x78, 0x7c, 0x80, 0x1c, 0x7f, 0xdf, 0x1f, 0x93, 0xf3, 0xe0,
  0x06, 0x3c, 0x23, 0xf0, 0xf8, 0x7c, 0x78, 0x7c, 0x80, 0x04, 0x7e, 0x1f, 0x3e, 0x03, 0xe1, 0xf0,
  0x04, 0x1c, 0x23, 0xe0, 0xf8, 0x7c, 0x78, 0x7c, 0x80, 0x02, 0x7c, 0x1f, 0x3e, 0x03, 0xe1, 0xf0,
  0x04, 0x00, 0x33, 0xe0, 0xf8, 0x7c, 0x78, 0x7c, 0x87, 0xc2, 0x7c, 0x1f, 0x3e, 0x03, 0xe1, 0xf0,
  0x04, 0x00, 0x13, 0xe0, 0xf8, 0x7c, 0x78, 0x7c, 0x84, 0x43, 0x7c, 0x1f, 0x3e, 0x03, 0xe1, 0xf0,
  0x0c, 0x00, 0x13, 0xe0, 0xf8, 0x7c, 0x7c, 0x7c, 0x84, 0x43, 0x7c, 0x1f, 0x3e, 0x03, 0xe1, 0xf0,
  0x08, 0x7e, 0x1b, 0xe0, 0xf8, 0x7c, 0x7c, 0x7c, 0x87, 0xc3, 0x7c, 0x1f, 0x3f, 0x03, 0xe1, 0xf0,
  0x08, 0x62, 0x0b, 0xe0, 0x7f, 0xfc, 0x7f, 0xfc, 0x83, 0x02, 0x7c, 0x1f, 0x1f, 0xfb, 0xff, 0xe0,
  0x18, 0x43, 0x0b, 0xe0, 0x7f, 0xfc, 0x7f, 0xfc, 0x80, 0x06, 0x7c, 0x1f, 0x1f, 0xf9, 0xff, 0xe0,
  0x10, 0x43, 0x0f, 0xe0, 0x3f, 0xfc, 0x3f, 0xfc, 0x80, 0x0c, 0x7c, 0x1f, 0x0f, 0xf8, 0xff, 0xc0,
  0x18, 0xc1, 0x0f, 0xe0, 0x1f, 0x3c, 0x1f, 0x3c, 0x80, 0x78, 0x78, 0x1f, 0x03, 0xf8, 0x3f, 0x00,
  0x1f, 0x80, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// A continuación las imagenes utilizadas para la animación de la puerta

const unsigned char Puerta1[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1,
  0xfb, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0xec, 0xfc, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0c, 0x1f, 0xbb, 0x3f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7e, 0xe0, 0xcf,
  0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xfb, 0x80, 0x33, 0xf0, 0x60, 0x00, 0x00, 0x00, 0x03,
  0x07, 0xee, 0x00, 0x0c, 0xfc, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xbf, 0xff, 0xff, 0x3f, 0x06,
  0x00, 0x00, 0x00, 0x30, 0x7e, 0x02, 0x00, 0x00, 0x0f, 0xc1, 0x80, 0x00, 0x00, 0xc1, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0x60, 0x00, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x18, 0x00,
  0x1c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x06, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0x80, 0x20, 0x7f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x3e, 0x00,
  0x1f, 0x00, 0x1f, 0x01, 0x41, 0x80, 0x20, 0x50, 0xff, 0x80, 0x7f, 0xc0, 0x7f, 0xc1, 0x41, 0x80,
  0x20, 0x51, 0xc1, 0xc0, 0xc0, 0xe0, 0xe0, 0x61, 0x41, 0x80, 0x20, 0x53, 0x00, 0x61, 0x80, 0x31,
  0x80, 0x31, 0x41, 0x80, 0x20, 0x53, 0x00, 0x61, 0x00, 0x31, 0x80, 0x11, 0x41, 0x80, 0x20, 0x56,
  0x00, 0x33, 0x00, 0x11, 0x00, 0x19, 0x41, 0x80, 0x20, 0x57, 0xff, 0xf3, 0xff, 0xfb, 0xff, 0xf9,
  0x41, 0x80, 0x20, 0x53, 0xff, 0xe3, 0xff, 0xf1, 0xff, 0xf9, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfd, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x40, 0x40, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x10, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x41, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00
};
const unsigned char Puerta2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1,
  0xfb, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0xec, 0xfc, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0c, 0x1f, 0xbb, 0x3f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7e, 0xe0, 0xcf,
  0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xfb, 0x80, 0x33, 0xf0, 0x60, 0x00, 0x00, 0x00, 0x03,
  0x07, 0xee, 0x00, 0x0c, 0xfc, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xbf, 0xff, 0xff, 0x3f, 0x06,
  0x00, 0x00, 0x00, 0x30, 0x7e, 0x02, 0x00, 0x00, 0x0f, 0xc1, 0x80, 0x00, 0x00, 0xc1, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0x60, 0x00, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x18, 0x00,
  0x1c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x06, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0x80, 0x20, 0x7f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfd, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x40, 0x40, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x40, 0x40, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x10, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00
};
const unsigned char Puerta3[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1,
  0xfb, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0xec, 0xfc, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0c, 0x1f, 0xbb, 0x3f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7e, 0xe0, 0xcf,
  0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xfb, 0x80, 0x33, 0xf0, 0x60, 0x00, 0x00, 0x00, 0x03,
  0x07, 0xee, 0x00, 0x0c, 0xfc, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xbf, 0xff, 0xff, 0x3f, 0x06,
  0x00, 0x00, 0x00, 0x30, 0x7e, 0x02, 0x00, 0x00, 0x0f, 0xc1, 0x80, 0x00, 0x00, 0xc1, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0x60, 0x00, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x18, 0x00,
  0x1c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x06, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0x80, 0x20, 0x7f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfd, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x40, 0x40, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x40, 0x40, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x10, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00
};
const unsigned char Puerta4[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1,
  0xfb, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0xec, 0xfc, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0c, 0x1f, 0xbb, 0x3f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7e, 0xe0, 0xcf,
  0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xfb, 0x80, 0x33, 0xf0, 0x60, 0x00, 0x00, 0x00, 0x03,
  0x07, 0xee, 0x00, 0x0c, 0xfc, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xbf, 0xff, 0xff, 0x3f, 0x06,
  0x00, 0x00, 0x00, 0x30, 0x7e, 0x02, 0x00, 0x00, 0x0f, 0xc1, 0x80, 0x00, 0x00, 0xc1, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0x60, 0x00, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x18, 0x00,
  0x1c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x06, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0x80, 0x20, 0x7f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0xc0,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x40, 0x40, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x5f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfd, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x10, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00
};
const unsigned char Puerta5[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0x0e, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7f, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1,
  0xfb, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0xec, 0xfc, 0x38, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0c, 0x1f, 0xbb, 0x3f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x7e, 0xe0, 0xcf,
  0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xfb, 0x80, 0x33, 0xf0, 0x60, 0x00, 0x00, 0x00, 0x03,
  0x07, 0xee, 0x00, 0x0c, 0xfc, 0x18, 0x00, 0x00, 0x00, 0x0c, 0x1f, 0xbf, 0xff, 0xff, 0x3f, 0x06,
  0x00, 0x00, 0x00, 0x30, 0x7e, 0x02, 0x00, 0x00, 0x0f, 0xc1, 0x80, 0x00, 0x00, 0xc1, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf0, 0x60, 0x00, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x18, 0x00,
  0x1c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x06, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81, 0x80, 0x20, 0x7f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x80, 0x20, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80,
  0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x41, 0x80, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x41, 0x80, 0x10, 0x50, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x41, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00
};
// Imagen para mostrar alarma desconectada
const unsigned char AlarmaOff[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0,
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe3, 0xff, 0x80, 0x00,
  0x00, 0x03, 0xff, 0x80, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x7f,
  0xf0, 0x00, 0x07, 0xff, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x01, 0xff, 0x00, 0x00, 0x7f, 0x00, 0x00,
  0x00, 0x7f, 0x00, 0x00, 0x7c, 0x00, 0x3e, 0x00, 0x1f, 0x00, 0x00, 0x78, 0x00, 0x7f, 0x80, 0x0f,
  0x00, 0x00, 0x78, 0x00, 0xff, 0xc0, 0x0f, 0x00, 0x00, 0x78, 0x01, 0xc1, 0xc0, 0x0f, 0x00, 0x00,
  0x78, 0x01, 0xc0, 0xe0, 0x0f, 0x00, 0x00, 0x78, 0x01, 0x80, 0xe0, 0x0f, 0x00, 0x00, 0x78, 0x01,
  0x80, 0x00, 0x0f, 0x00, 0x00, 0x78, 0x01, 0x80, 0x00, 0x0f, 0x00, 0x00, 0x78, 0x01, 0x80, 0x00,
  0x0f, 0x00, 0x00, 0x78, 0x01, 0x80, 0x00, 0x0f, 0x00, 0x00, 0x78, 0x01, 0x80, 0x00, 0x0f, 0x00,
  0x00, 0x78, 0x07, 0xff, 0xf0, 0x0f, 0x00, 0x00, 0x78, 0x0f, 0xff, 0xf8, 0x0f, 0x00, 0x00, 0x7c,
  0x0f, 0xff, 0xf8, 0x0f, 0x00, 0x00, 0x7c, 0x0f, 0xff, 0xf8, 0x1f, 0x00, 0x00, 0x7c, 0x0f, 0xff,
  0xf8, 0x1f, 0x00, 0x00, 0x3c, 0x0f, 0xff, 0xf8, 0x1f, 0x00, 0x00, 0x3c, 0x0f, 0xff, 0xf8, 0x1e,
  0x00, 0x00, 0x3e, 0x0f, 0xff, 0xf8, 0x3e, 0x00, 0x00, 0x3e, 0x0f, 0xff, 0xf8, 0x3e, 0x00, 0x00,
  0x1e, 0x0f, 0xff, 0xf8, 0x3c, 0x00, 0x00, 0x1f, 0x0f, 0xff, 0xf8, 0x7c, 0x00, 0x00, 0x0f, 0x8f,
  0xff, 0xf8, 0x7c, 0x00, 0x00, 0x0f, 0x87, 0xff, 0xf0, 0xf8, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x01,
  0xf0, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x03, 0xe0, 0x00,
  0x00, 0x01, 0xf0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00,
  0xfe, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc1,
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
// Imagen para combinar con alarma modo "Armada ausente" para crear la animación
const unsigned char AlarmaArmando[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};
void setup() {
  Serial.begin(9600);                                // Iniciamos el Serial
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Si no es correcta la configuración de la pantalla nos muestra un error
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.clearDisplay();                      // Limpiamos el buffer de la pantalla
  display.drawBitmap(0, 0, logo, 128, 64, 1);  // Dibujamos el logo en la pantalla
  display.display();                           // Hacemos que se muestre en la pantalla
  delay(3000);                                 // Durante 3 segundos
  display.clearDisplay();                      // Después la limpiamos
  display.display();                           // Hacemos que se muestre en la pantalla

  pinMode(botonArmar, INPUT_PULLUP);  // Definimos los pulsadores como Pull_Up, no necesitamos resistencias externas
  pinMode(botonAbrir, INPUT_PULLUP);
}

void loop() {
  // PULSADOR ABRIR //

  valAbrir = digitalRead(botonAbrir);  // Leemos el valor del pulsador

  if (valAbrir == LOW) {
    for (int i = 0; i < 5; i++) {                                                        // Creamos un "for" para aumentar el valor de "i" mientras el botón está pulsado
      const unsigned char* puerta[5] = { Puerta1, Puerta2, Puerta3, Puerta4, Puerta5 };  // Creamos un "array" con los nombres de las imagenes de la puerta
      display.fillRect(0, 0, 82, 64, 0);                                                 // Dibujamos un cuadrado negro para borrar la imagen anterior, si usasemos la opcion de "clearDisplay" nos borraría toda la pantalla
      display.drawBitmap(0, 0, puerta[i], 75, 64, 1);                                    // Mostramos la imagen llamándola desde el "array" anteriormente configurado,donde la variable de "s" va a ir cambiando entre "0" y "1"
      display.display();                                                                 // Mostramos la imagen en pantalla
      delay(500);

      if (valAbrir == HIGH) {  // Si el botón no está pulsado...

        break;  // Salimos del bucle "for"
      }
    }
  }

  // PULSADOR ARMAR //

  valArmar = digitalRead(botonArmar);  // Leemos el valor del pulsador

  if (valArmar == LOW) {
    for (int b = 0; b < 2; b++) {                                     // Creamos un "for" para aumentar el valor de "b" mientras el botón está pulsado
      const unsigned char* alarma[2] = { AlarmaOff, AlarmaArmando };  // Creamos un "array" con los nombres de las imagenes de la alarma
      display.fillRect(82, 9, 64, 64, 0);                             // Dibujamos un cuadrado negro para borrar la imagen anterior, si usasemos la opcion de "clearDisplay" nos borraría toda la pantalla
      display.drawBitmap(82, 9, alarma[b], 50, 50, 1);                // Mostramos la imagen llamándola desde el "array" anteriormente configurado,donde la variable de "s" va a ir cambiando entre "0" y "1"
      display.display();                                              // Mostramos la imagen en pantalla
      delay(500);

      if (valArmar == HIGH) {  // Si el botón no está pulsado...

        break;  // Salimos del bucle "for"
      }
    }
  }
}
