# LED Poi Project w/ Arduino Uno and Teensy 3.6/4.0
## General
This project is using remote controlled LED Light sticks (so called Pois) to create
a music synchronized, visually appealing light show using the persistence of vision effect.

Therefore one controller station (PC) plays music and sends at distinct time stamps commands via serial to an Arduino Uno Transmitter.
The Arduino sends these commands then further to the LED Pois which then load the yet hard-coded visuals and controls the LEDs.

The original idea came from mortonkopf who used APA102 pixel strips with a Teensy to light paint. He built a tube poi which, when rotated quickly, 
made the viewer see real colored images because of the persistence of vision (pov).

So I wanted to take it a step further and make a music synchronized controller which controls multiple Pois (6 at this point).
Therefore nRF24L01 2.4Ghz Transceiver moduls were used.

## Teensy 3.6 - Wiring
![image](https://github.com/user-attachments/assets/d5cb2b31-7629-4b29-bba9-347b3d013a23)

## Teensy 4.0 - Wiring
![image](https://github.com/user-attachments/assets/423ae56b-6234-4ae1-b0d4-ef39c1e420d2)
