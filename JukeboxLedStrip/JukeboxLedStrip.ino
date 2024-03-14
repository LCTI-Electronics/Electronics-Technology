// Coded by Gabe Magwood
#include <Adafruit_NeoPixel.h>
#define LED_PIN     6
#define LED_COUNT  144

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

/*
 1 = Pulsing random color
 2 = Solid white
 3 = Running lights
*/
int mode = 1; 
const int speed = 10; //ms delay
const int brightness = 200;
unsigned long timer;
int cycles = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();           
  strip.show();           
  strip.setBrightness(brightness);
}

void loop() {
  timer = millis();

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
    Serial.println(mode);
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

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
  }

  delay(500); // Pause 1/2 second
}
