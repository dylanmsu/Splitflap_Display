#include <ColorConverterLib.h>
#include <Adafruit_NeoPixel.h>

uint8_t red;
uint8_t green;
uint8_t blue;

int pin_led = 2;
int aantal_pixels = 30;

Adafruit_NeoPixel pixels(aantal_pixels, pin_led, NEO_GRB + NEO_KHZ800);

void setup() 
{
 pixels.clear();
 pixels.begin();
}

void loop() 
{
char hex[] = "#0000FF";
String hex_color(hex);
  ColorConverter::HexToRgb(hex_color, red, green, blue);
  for(int i=0; i<aantal_pixels; i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();
    delay(0);
  }
}
