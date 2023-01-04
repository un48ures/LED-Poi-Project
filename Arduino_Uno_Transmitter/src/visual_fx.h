// Lauflicht 1-6 links nach rechts
    if (program == 12)
    {
      for (int i = 1; i < 7; i++)
      {
        send_data(i + 19, 1);
        delay(25);
        send_data(i + 19, 0);
      }
    }

    // Lauflicht 1-6 hin und her
    if (program == 13)
    {
      for (int i = 1; i < 7; i++)
      {
        send_data(i + CH_P1 - 1, 1);
        delay(25);
        send_data(i + CH_P1 - 1, 0);
      }
      for (int i = 6; i > 0; i--)
      {
        send_data(i + CH_P1 - 1, 1);
        delay(25);
        send_data(i + CH_P1 - 1, 0);
      }