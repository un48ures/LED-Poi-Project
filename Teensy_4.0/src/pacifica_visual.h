// #include "visual_functions.h"
// #include "teensy_4.0_config.h"
// #include "types.h"
#include "FastLED.h"
#include <RF24.h>

void pacifica_loop(CRGB *leds);

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff, CRGB *leds);

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps(CRGB *leds);

// Deepen the blues and greens
void pacifica_deepen_colors(CRGB *leds);