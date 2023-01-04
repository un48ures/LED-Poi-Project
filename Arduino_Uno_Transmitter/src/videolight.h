void video_light_mode(RF24* radio)
{
  // Read poti analog input
  brightness = map(analogRead(poti), 0, 1023, 0, 50);

  // Read button input
  button_state = digitalRead(TASTER);

  // Increment program counter
  if (button_state_old == LOW and button_state == HIGH)
  {
    program++;
    button_state_old = HIGH;
  }
  if (button_state_old == HIGH and button_state == LOW)
  {
    button_state_old = LOW;
  }

  // If change then send
  if (brightness_old != brightness || program_old != program || program > 11)
  {
    brightness_old = brightness;
    program_old = program;

    if (program > 11)
    {
      program = 0;
    }

    Serial.print("Program counter: ");
    Serial.println(program);

    // data_array[1] = program;
    // data_array[3] = brightness;

    switch (program)
    {
    case 0:
      for (uint8_t i = 0; i < sizeof(CHs); i++)
      {
        data_array[1] = 0;
        send_data(CHs[i], data_array, (uint8_t) sizeof(data_array), pipe_address, radio);
        data_array[1] = 99;
        send_data(CHs[i], data_array, (uint8_t) sizeof(data_array), pipe_address, radio);
      }
      break;

    default:
      for (uint8_t i = 0; i < sizeof(CHs); i++)
      {
        data_array[1] = program;
        data_array[3] = brightness;
        send_data(CHs[i], data_array, (uint8_t) sizeof(data_array), pipe_address, radio);
      }
      break;
    }
  }
}