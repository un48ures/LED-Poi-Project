
/// @brief Function to go through the picture arrays and show each slice via FastLED
/// @param time Time a picture is shown (repeated while time is not over)
/// @param array Array (picture) to be shown via FastLED
void PoiSonic_old(unsigned long time, const unsigned int array[])
{
    unsigned long currentTime = millis();

    while (millis() < currentTime + (time))
    {
        int f = numberOfSlices;
        int z; // a counter
        int j = NUM_LEDS;
        for (int x = 0; x < f; x++)
        {
            for (z = NUM_LEDS; z > 0; z--)
            {
                leds[z - 1] = array[x + ((j - z) * f)];
            }
            FastLED.setBrightness(message_brightness);
            FastLED.show();
            // Serial.println("FASTLED show...");
            delayMicroseconds(500); // may need to increase / decrease depending on spin rate original value = 40
        }
        delayMicroseconds(1000); // Abstand zwischen Bilder - may need to increase / decrease depending on spin rate
    }
}

void showPicOnce(const unsigned int array[])
{
    for (int x = 0; x < numberOfSlices; x++)
        {
            for (int z = NUM_LEDS; z > 0; z--)
            {
                leds[z - 1] = array[x + ((NUM_LEDS - z) * numberOfSlices)];
            }
            FastLED.setBrightness(message_brightness);
            FastLED.show();
            // Serial.println("FASTLED show...");
            // if (radio.available()) return;
            delayMicroseconds(500); // Pause between slices - may need to increase / decrease depending on spin rate original value = 40
        }
}

void PoiSonic(unsigned long time, const unsigned int array[])
{
    unsigned long currentTime = millis();

    while ((millis() < currentTime + (time)) && !radio.available())
    {
        showPicOnce(array);
        delayMicroseconds(1000); // Abstand zwischen Bilder - may need to increase / decrease depending on spin rate
    }
    if ((millis() < currentTime + (time)) && radio.available())
    {
        Serial.println("Radio available but time not over");
    }
}

// Move a single white dot up and down at startup
void StartDemo()
{
    for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1)
    {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::White;
        // Show the leds (only one of which is set to white, from above)
        FastLED.show();
        // Wait a little bit
        delay(7);
        // Turn our current led back to black for the next loop around
        leds[whiteLed] = CRGB::Black;
    }

    for (int whiteLed = NUM_LEDS; whiteLed > 0; whiteLed--)
    {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::White;
        // Show the leds (only one of which is set to white, from above)
        FastLED.show();
        // Wait a little bit
        delay(7);
        // Turn our current led back to black for the next loop around
        leds[whiteLed] = CRGB::Black;
    }
}

// Standby 1st LED Blink RED
void LED_blink_red()
{
    unsigned long currentMillis = millis(); // for LED blink

    if (currentMillis - previousMillis >= interval_1)
    {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        blink_once = 1; // start one blink
    }

    if (blink_once)
    {
        // if the LED is off turn it on and vice-versa:
        if (LEDstate == 0)
        {
            // Let the first LED blink red
            leds[0] = CRGB::Red;
            FastLED.setBrightness(DEFAULT_BRIGHTNESS);
            FastLED.show();
            time_on = millis();
            LEDstate = 1;
        }
        if (LEDstate == 1)
        {
            if (millis() - time_on > LED_red_on_time)
            {
                leds[0] = CRGB::Black;
                FastLED.show();
                LEDstate = 0;
                blink_once = 0;
            }
        }
    }
}

// FILL COLORS------------------------------------------------------------------------------------------------
void LED_fill_black()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
}

void LED_fill_white()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::White;
    }
}

void LED_fill_red()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Red;
    }
}

void LED_fill_green()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Green;
    }
}

void LED_fill_blue()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Blue;
    }
}

void LED_fill_purple()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Purple;
    }
}

void LED_fill_yellow()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Yellow;
    }
}

//-------------------------------------------------------------------------------------------------------------
// FADE PULSING
//-------------------------------------------------------------------------------------------------------------
void LED_Pulsing(int hue, int message_brightness)
{
    // aufsteigend
    if (looping == 0)
    {
        for (int j = 0; j < message_brightness; j++)
        {
            //    if (radio.available()) {last_one = 1;}
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i].setHSV(hue, 255, 255);
                FastLED.setBrightness(j);
                FastLED.show();
            }
        }
        looping = 1; // jetzt absteigend
    }

    // absteigend
    if (looping == 1)
    {
        for (int j = message_brightness; j > 0; j--)
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i].setHSV(hue, 255, 255);
                FastLED.setBrightness(j);
                FastLED.show();
            }
        }
        looping = 0;
        LED_fill_black();
        FastLED.show();
    }
}

//-------------------------------------------------------------------------------------------------------------
// FILL STROBO
//-------------------------------------------------------------------------------------------------------------
void LED_strobo()
{
    LED_fill_white();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    delay(50);
    LED_fill_black();
    FastLED.setBrightness(1);
    FastLED.show();
    delay(50);
}

//-------------------------------------------------------------------------------------------------------------
// FILL FROM BOTTOM
//-------------------------------------------------------------------------------------------------------------
void LED_fillup()
{
    if (fillupDone == 0)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::Red;
            FastLED.show();
            FastLED.delay(25);
        }
        fillupDone = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------
// FILL FROM TOP
//-------------------------------------------------------------------------------------------------------------
void LED_filldown()
{
    if (filldownDone == 0)
    {
        for (int i = NUM_LEDS - 1; i > 0; i--)
        {
            leds[i] = CRGB::Red;
            FastLED.show();
            FastLED.delay(25);
        }
        filldownDone = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------
// RAINBOW
//-------------------------------------------------------------------------------------------------------------
void addGlitter(fract8 chanceOfGlitter)
{
    if (random8() < chanceOfGlitter)
    {
        leds[random16(NUM_LEDS)] += CRGB::White;
    }
}

void rainbow()
{
    // FastLED's built-in rainbow generator
    fill_rainbow(leds, NUM_LEDS, gHue, 7);
    addGlitter(80);
    FastLED.show();
    FastLED.delay(50);
    EVERY_N_MILLISECONDS(10)
    {
        gHue++;
    }
}

void fadetoblack()
{
    fadeToBlackBy(leds, NUM_LEDS, 50);
    FastLED.show();
    FastLED.delay(15);
}
