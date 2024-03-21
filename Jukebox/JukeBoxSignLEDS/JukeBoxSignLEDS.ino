#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define LED_COUNT  432

/*
 1 = Pulsing random color
 2 = Solid white
 3 = Running lights
*/
int mode = 1; 
const int speed = 10; //ms delay
const int brightness = 200;
int cycles = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);


void setup() {
  strip.begin();           
  strip.show();           
  strip.setBrightness(brightness);
}

void loop() {

  
  switch (mode) {
    case 1: {
      int red = random(0, 128);
      int green = random(0, 128);
      int blue = random(0, 128);
      for(int i = 0; i < 5; i++) {
        for(int i = 0; i <= brightness; i+=5) {
          strip.fill(strip.Color(red, green, blue));
          strip.setBrightness(i);
          strip.show();
          delay(speed);
        }

        for(int i = brightness; i >= 0; i-=5) {
          strip.fill(strip.Color(red, green, blue));
          strip.setBrightness(i);
          strip.show();
          delay(speed);
        }
      }
      break;
    }

    case 2: {
      strip.setBrightness(brightness);
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(255)));
      strip.show();
      delay(5000);
      break;
    }

    case 3: {
      strip.setBrightness(brightness);
      byte r = 0x00;
      byte g = 0x00;
      byte b = 0xff;
      int pos = 0; 
      for(int i = 0; i < LED_COUNT*2; i++) {
        pos++;
        for(int i = 0; i < LED_COUNT; i++) {
          setPixel(i,((sin(i+pos) * 127 + 128)/255)*r,
                  ((sin(i+pos) * 127 + 128)/255)*g,
                  ((sin(i+pos) * 127 + 128)/255)*b);
        }

        strip.show();
        delay(50);
      }
      break;
    
    }
    cycles++;
  }

  if (cycles % 5 == 0) {
    mode = random(1, 4);
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}