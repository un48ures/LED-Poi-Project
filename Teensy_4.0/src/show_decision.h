
/// @brief Make decision which array to show in relation to the reveived message_global
/// @param message_global shows the function which picture/array should be shown
void show_decision(int message_global)
{
  switch (message_global)
  {
  case 0:
    LED_fill_black();
    FastLED.show();
    break;
  case 1:
    LED_fill_white();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 2:
    LED_fill_red();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 3:
    LED_fill_green();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 4:
    LED_fill_blue();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 5:
    LED_fill_purple();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 6:
    LED_fill_yellow();
    FastLED.setBrightness(message_brightness);
    FastLED.show();
    break;
  case 7:
    // FADE PULSE - BLUE
    hue = 150; // BLUE
    LED_Pulsing(hue, message_brightness);
    delay(50);
    break;
  case 8:
    // FADE PULSE - GREEN
    hue = 100; // GREEN
    LED_Pulsing(hue, message_brightness);
    delay(50);
    break;
  case 9:
    // FADE PULSE - RED
    hue = 255; // GREEN
    LED_Pulsing(hue, message_brightness);
    delay(50);
    break;

  //    case 9:
  //      FastLED.setBrightness(5);
  //      if (filldownDone = 0) {
  //        fill_solid(leds, NUM_LEDS, CRGB::Black);
  //      }
  //      LED_filldown();
  //      break;
  case 10:
    FastLED.setBrightness(message_brightness);
    rainbow();
    break;
  case 11:
    LED_strobo();
    break;
  case 12:
    PoiSonic(timePicIsShown, array1);
    break;
  case 13:
    PoiSonic(timePicIsShown, array2);
    break;
  case 14:
    PoiSonic(timePicIsShown, array3);
    break;
  case 15:
    PoiSonic(timePicIsShown, array4);
    break;
  case 16:
    PoiSonic(timePicIsShown, array5);
    break;
  case 17:
    PoiSonic(timePicIsShown, array6);
    break;
  case 18:
    PoiSonic(timePicIsShown, array7);
    break;
  case 19:
    PoiSonic(timePicIsShown, array8);
    break;
  case 20:
    PoiSonic(timePicIsShown, array9);
    break;
  case 21:
    PoiSonic(timePicIsShown, array10);
    break;
  case 22:
    PoiSonic(timePicIsShown, array11);
    break;
  case 23:
    PoiSonic(timePicIsShown, array12);
    break;
  case 24:
    PoiSonic(timePicIsShown, array13);
    break;
  case 25:
    PoiSonic(timePicIsShown, array14);
    break;
  case 26:
    PoiSonic(timePicIsShown, array15);
    break;
  case 27:
    PoiSonic(timePicIsShown, array16);
    break;
  case 28:
    PoiSonic(timePicIsShown, array17);
    break;
  case 29:
    PoiSonic(timePicIsShown, array18);
    break;
  case 30:
    PoiSonic(timePicIsShown, array19);
    break;
  case 31:
    PoiSonic(timePicIsShown, array20);
    break;
  case 32:
    PoiSonic(timePicIsShown, array21);
    break;

  default:
    LED_blink_red();
    break;
  }
}