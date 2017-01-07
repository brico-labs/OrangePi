
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

