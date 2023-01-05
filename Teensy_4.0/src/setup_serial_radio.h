void setup_serial_radio()
{
    Serial.begin(115200);
    Serial.println("Initializing...");
    SPI.setSCK(RF24_SCK);
    SPI.begin();
    delay(200);

    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(10)>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);

    radio.begin();
    radio.openReadingPipe(0, pipe_address);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_1MBPS);
    // radio.setRetries(0,15);
    radio.setChannel(CHANNEL);
    // radio.enableDynamicPayloads() ;
    // radio.setAutoAck( true ) ;
    // radio.powerUp() ;
    radio.startListening();

    // pinMode(ledPin, OUTPUT);
    // digitalWrite(ledPin, HIGH);
    Serial.println("Initializing done");
}