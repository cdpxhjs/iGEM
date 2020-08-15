#include <FastLED.h>

// Number of LED pixels that will be used
#define NUM_LEDS 256*2
// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6

// Pass the number of LEDs to the leds array
CRGB leds[NUM_LEDS];

// Define the brightness
// The greater the brightness the more voltage required to power the leds
#define BRIGHTNESS 7
void setup() { 
       FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
       FastLED.setBrightness(BRIGHTNESS);
}

// Cycle between light on and off
void loop() { 
  // Turn the LED on, then pause
  for(int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();
  // 14 hours white light on (in milliseconds)
  delay(50400000);

  // Now turn the LED off, then pause
  for(int j  = 0; j <= NUM_LEDS; j++) {
    leds[j] = CRGB::Black;
  }
  FastLED.show();
  // 10 hours lights off (in milliseconds)
  delay(36000000);
}
