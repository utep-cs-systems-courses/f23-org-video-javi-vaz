#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "buzzer.h"

char switch_state_down, switch_state_changed; /* effectively boolean */

static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */
  return p2val;
}

void
switch_init()/* setup switch */
{
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */
  switch_update_interrupt_sense();
}
int switches = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
  if (switches & SW1) {
    redrawScreen = 1;
    buzzer_set_period(478); /* C */
    drawCircle(drawPos[0],drawPos[1], 5, COLOR_WHITE);
    drawCircle(drawPos[1],drawPos[0],10,COLOR_PINK);
    clearCount = 0;
  }else if(switches & SW2){
    redrawScreen = 1;
    buzzer_set_period(425); /* D */
    drawCircle(drawPos[2],drawPos[3], 30, COLOR_RED);
    drawCircle(drawPos[3],drawPos[2],7,COLOR_WHITE);
    clearCount = 0;
  }else if (switches & SW3) {
    redrawScreen = 1;
    buzzer_set_period(379); /* E */
    drawCircle(drawPos[4],drawPos[5], 4, COLOR_WHITE);
    drawCircle(drawPos[5],drawPos[4],16,COLOR_GREEN);
    clearCount = 0;
  }else if(switches & SW4){
    redrawScreen = 1;
    buzzer_set_period(338); /* F */
    drawCircle(drawPos[6],drawPos[7], 15, COLOR_YELLOW);
    drawCircle(drawPos[7],drawPos[6],10,COLOR_WHITE);
    clearCount = 0;
  } else {
    redrawScreen = 0;
    buzzer_set_period(0);
  }
}
