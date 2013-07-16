#include <Adafruit_NeoPixel.h>

#define PIN 13

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(240, PIN, NEO_GRB + NEO_KHZ800);
uint8_t t = 0;
uint32_t z = strip.Color(0, 0, 0);

void setup() {
  //Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //init(5, Wheel(t));
  random_init(3, 5, Wheel(t));
  //runs(2, 1, Wheel(t));
}

void loop() {
  uint32_t c = Wheel(t+= 5);
  Serial.print("=== "); 
  Serial.print(c); 
  Serial.println(" ===");
  delay(200);
  //printStrand();
  live(c);
}

void printStrand() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    Serial.println(strip.getPixelColor(i));
  }
}

void live(uint32_t c) {
  boolean change = false;
  uint32_t prev, curr, next, acc = 0;
  prev = 0;
  curr = strip.getPixelColor(0);

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i < strip.numPixels() - 1) {
      next = strip.getPixelColor(i+1);    
    } 
    else {
      next = 0;
    }

    /*
    Serial.println("---");
     Serial.println(prev);
     Serial.println(curr);
     Serial.println(next);
     Serial.println("---");
     */

    // http://en.wikipedia.org/wiki/Rule_90
    boolean alive = !!prev ^ !!next;
    change = !!curr ^ alive;
    if (alive)
      strip.setPixelColor(i, c);
    else
      strip.setPixelColor(i, z);

    prev = curr;
    curr = next;

    acc |= curr;
  }
  strip.show();
}

void evens(uint32_t color) {
  init(2, color, 0, 1);
}

void odds(uint32_t color) {
  init(2, color, 1, 1);
}

void runs(uint8_t on, uint8_t off, uint32_t color) {
  init(off+on, color, 0, on);
}

void init(uint8_t k, uint32_t color, uint8_t first, uint8_t run) {
  for(uint16_t i=first; i < strip.numPixels(); i+= k) {
    for(uint16_t j=0; j < run; ++j) {
      strip.setPixelColor(i+j, color);
    }
  }
  strip.show();
}

void random_init(uint8_t k, uint8_t n, uint32_t color) {
  for(uint16_t i=1; i < strip.numPixels(); i++) {
    if (random(n) < k) strip.setPixelColor(i, color);
  }
  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition g - b - back to g.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 128) {
    return strip.Color(0, 255 - WheelPos * 2, WheelPos * 2);
  } 
  else {
    WheelPos -= 128;
    return strip.Color(0, WheelPos * 2, 255 - WheelPos * 2);
  }
}








