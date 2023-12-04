#include <msp430.h>
#include "led.h"

void led_init() {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;/* leds initially off */
}
