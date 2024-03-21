// Coded by Gabe Magwood
#include <Adafruit_NeoPixel.h>

#define OUTER_LED_PIN 6
#define INNER_LED_PIN 3

int innerMode = 1; 
int outerMode = 2;

const int speed = 10; //ms delay
const int brightness = 200;
const int LED_COUNT = 144 * 3;
unsigned long timer;
int cycles = 0;

Adafruit_NeoPixel innerStrip(LED_COUNT, 3, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel outerStrip(LED_COUNT, 6, NEO_RGBW + NEO_KHZ800);

/*
 1 = Pulsing random color
 2 = Solid white
 3 = Running lights
*/

void setup() {
  Serial.begin(9600);
  innerStrip.begin();           
  outerStrip.begin();
  outerStrip.show();  
  innerStrip.show();         
  innerStrip.setBrightness(brightness);
  outerStrip.setBrightness(brightness);
  Serial.println("Done");
}

void loop() {
  timer = millis();

  switch (innerMode) {
    case 1: {
      int red = random(0, 128);
      int green = random(0, 128);
      int blue = random(0, 128);
      for(int i = 0; i < 5; i++) {
        for(int i = 0; i <= brightness; i+=5) {
          innerStrip.fill(innerStrip.Color(red, green, blue));
          innerStrip.setBrightness(i);
          innerStrip.show();
          delay(speed);
        }

        for(int i = brightness; i >= 0; i-=5) {
          innerStrip.fill(innerStrip.Color(red, green, blue));
          innerStrip.setBrightness(i);
          innerStrip.show();
          delay(speed);
        }
      }
      break;
    }

    case 2: {
      innerStrip.setBrightness(brightness);
      innerStrip.fill(innerStrip.Color(0, 0, 0, innerStrip.gamma8(255)));
      innerStrip.show();
      delay(5000);
      break;
    }

    case 3: {
      innerStrip.setBrightness(brightness);
      byte r = 0x00;
      byte g = 0x00;
      byte b = 0xff;
      int pos = 0; 
      for(int i = 0; i < LED_COUNT*2; i++) {
        pos++;
        for(int i = 0; i < LED_COUNT; i++) {
          setPixelInner(i,((sin(i+pos) * 127 + 128)/255)*r,
                  ((sin(i+pos) * 127 + 128)/255)*g,
                  ((sin(i+pos) * 127 + 128)/255)*b);
        }

        innerStrip.show();
        delay(50);
      }
      break;
    
    }
  }

  switch (outerMode) {
    case 1: {
      int red = random(0, 128);
      int green = random(0, 128);
      int blue = random(0, 128);
      for(int i = 0; i < 5; i++) {
        for(int i = 0; i <= brightness; i+=5) {
          outerStrip.fill(outerStrip.Color(red, green, blue));
          outerStrip.setBrightness(i);
          outerStrip.show();
          delay(speed);
        }

        for(int i = brightness; i >= 0; i-=5) {
          outerStrip.fill(outerStrip.Color(red, green, blue));
          outerStrip.setBrightness(i);
          outerStrip.show();
          delay(speed);
        }
      }
      break;
    }

    case 2: {
      outerStrip.setBrightness(brightness);
      outerStrip.fill(outerStrip.Color(0, 0, 0, outerStrip.gamma8(255)));
      outerStrip.show();
      delay(5000);
      break;
    }

    case 3: {
      outerStrip.setBrightness(brightness);
      byte r = 0x00;
      byte g = 0x00;
      byte b = 0xff;
      int pos = 0; 
      for(int i = 0; i < LED_COUNT*2; i++) {
        pos++;
        for(int i = 0; i < LED_COUNT; i++) {
          setPixelOuter(i,((sin(i+pos) * 127 + 128)/255)*r,
                  ((sin(i+pos) * 127 + 128)/255)*g,
                  ((sin(i+pos) * 127 + 128)/255)*b);
        }

        outerStrip.show();
        delay(50);
      }
      break;
    
    }
  }
  cycles++;

  if (cycles % 5 == 0) {
    innerMode = random(1, 4);
    outerMode = random(1, 4);
    Serial.println(innerMode, outerMode);
  }
}

void setPixelInner(int Pixel, byte red, byte green, byte blue) {
  innerStrip.setPixelColor(Pixel, innerStrip.Color(red, green, blue));
}

void setPixelOuter(int Pixel, byte red, byte green, byte blue) {
  outerStrip.setPixelColor(Pixel, outerStrip.Color(red, green, blue));
}