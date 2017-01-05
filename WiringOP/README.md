
# WiringOP

__WiringOP__ es una biblioteca que permite manejar los pins de expansión de tu OrangePi.

Está basada en _[WiringPi de @drogon](http://wiringpi.com/)_ y usa los [módulos Linux creados por la comunidad
sunxi](https://linux-sunxi.org/Main_Page) para acceder al API de manejo del GPIO (_General Purpose Input/Output_) y
otras extensiones (I2C, SPI, PWM, ...) relacionadas con los pins de expansión.

## Instalación

    git clone https://github.com/zhaolei/WiringOP.git -b h3

    cd WiringOP
    sudo ./build

## El comando 'gpio' 

    caligari@orangepizero:~/WiringOP$ gpio readall
     +-----+-----+----------+------+---+-Orange Pi+---+---+------+---------+-----+--+
     | BCM | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | BCM |
     +-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
     |     |     |     3.3v |      |   |  1 || 2  |   |      | 5v       |     |     |
     |  12 |   8 |    SDA.0 |  OUT | 0 |  3 || 4  |   |      | 5V       |     |     |
     |  11 |   9 |    SCL.0 |  OUT | 0 |  5 || 6  |   |      | 0v       |     |     |
     |   6 |   7 |   GPIO.7 |  OUT | 0 |  7 || 8  | 0 | OUT  | TxD3     | 15  | 13  |
     |     |     |       0v |      |   |  9 || 10 | 1 | OUT  | RxD3     | 16  | 14  |
     |   1 |   0 |     RxD2 |  OUT | 0 | 11 || 12 | 0 | OUT  | GPIO.1   | 1   | 110 |
     |   0 |   2 |     TxD2 |  OUT | 0 | 13 || 14 |   |      | 0v       |     |     |
     |   3 |   3 |     CTS2 |  OUT | 0 | 15 || 16 | 0 | OUT  | GPIO.4   | 4   | 68  |
     |     |     |     3.3v |      |   | 17 || 18 | 0 | OUT  | GPIO.5   | 5   | 71  |
     |  64 |  12 |     MOSI |  OUT | 1 | 19 || 20 |   |      | 0v       |     |     |
     |  65 |  13 |     MISO |  OUT | 1 | 21 || 22 | 1 | OUT  | RTS2     | 6   | 2   |
     |  66 |  14 |     SCLK |  OUT | 1 | 23 || 24 | 0 | OUT  | CE0      | 10  | 67  |
     |     |     |       0v |      |   | 25 || 26 | 1 | OUT  | GPIO.11  | 11  | 21  |
     |  19 |  30 |    SDA.1 | ALT4 | 0 | 27 || 28 | 0 | ALT4 | SCL.1    | 31  | 18  |
     |   7 |  21 |  GPIO.21 |  OUT | 1 | 29 || 30 |   |      | 0v       |     |     |
     |   8 |  22 |  GPIO.22 |  OUT | 1 | 31 || 32 | 0 | ALT3 | RTS1     | 26  | 200 |
     |   9 |  23 |  GPIO.23 |  OUT | 1 | 33 || 34 |   |      | 0v       |     |     |
     |  10 |  24 |  GPIO.24 |  OUT | 1 | 35 || 36 | 0 | ALT3 | CTS1     | 27  | 201 |
     |  20 |  25 |  GPIO.25 |  OUT | 1 | 37 || 38 | 0 | ALT5 | TxD1     | 28  | 198 |
     |     |     |       0v |      |   | 39 || 40 | 0 | ALT5 | RxD1     | 29  | 199 |
     +-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
     | BCM | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | BCM |
     +-----+-----+----------+------+---+-Orange Pi+---+------+----------+-----+-----+
    

## Biblioteca WiringPi 

    /* blink.c - Blink with WiringOP. 
     * 
     * Compile with: gcc -Wall -o blink blink.c -lwiringPi
     */
    
    #include <wiringPi.h>

    #include <stdlib.h>
    #include <errno.h>
    #include <stdio.h>
    
    int main(int argc, char* argv[]) 
    {
      /* Default pin is 7 (number in the 13x2 pin expansion) */
      int pin_led = 7;
      
      /* Or pass pin via command parameter */
      if (argc > 1)
      {
        char* end;
        int param = strtol(argv[1], &end, 10);
        if (*end)
        {
          printf("Parameter '%s' is not a integer value!\n", argv[1]);
          return -1;
        }
        pin_led = param;
      }
    
      /* Initialize the library */
      wiringPiSetup();
    
      /* Initialize the pin */
      pinMode(pin_led, OUTPUT) ;
    
      /* Ctrl-C to interrupt */
      while (1) 
      {
        /* On */
        digitalWrite(pin_led, HIGH); 
        delay(500);
    
        /* Off */
        digitalWrite(pin_led, LOW); 
        delay(500);
      }
    
      return 0;
    }
    
    
    
    
