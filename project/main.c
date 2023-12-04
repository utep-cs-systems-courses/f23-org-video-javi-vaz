#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "switches.h"
#include "led.h"
#include "buzzer.h"

short redrawScreen = 1;
u_int controlFontColor = COLOR_GREEN;

short clearCount = 0;
short drawPos[8] = {1, 32, 1, 64, 1, 96, 1, 128}, controlPos[8] = {2, 32, 2, 64, 2, 96, 2, 128};
short colVelocity = 1, colLimits[2] = {1, screenWidth};

void
draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 3, 3, color);
}


void
screen_update_ball()
{
  for (char axis = 0; axis < 8; axis ++)
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;/* nothing to do */
 redraw:
  //for(char axis = 0; axis< 8; axis+=2)
    //draw_ball(drawPos[axis], drawPos[axis+1], COLOR_BLUE); /* erase */
  for (char axis = 0; axis < 8; axis ++)
    drawPos[axis] = controlPos[axis];
  //for(char axis = 0; axis < 8;axis+=2)
    //draw_ball(drawPos[axis], drawPos[axis+1], COLOR_WHITE); /* draw */
}

int main(void)
{
  led_init();
  configureClocks();
  enableWDTInterrupts();
  lcd_init();
  switch_init();
  buzzer_init();
  or_sr(0x8);
  
  clearScreen(COLOR_BLUE);
  while (1) {/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      screen_update_ball();
      if(clearCount>=50){
	clearScreen(COLOR_BLUE);
      }
    }
    //P1OUT &= ~BIT6;/* led off */
    or_sr(0x10);/**< CPU OFF */
    //P1OUT |= BIT6;/* led on */
  }
}

int blinkFrequency = 1;
static int blinks = 0;
static int secondCount = 0;
int countState = 0;
short count = 0;

void
wdt_c_handler()//__interrupt_vec(WDT_VECTOR) WDT()/* 250 interrupts/sec */
{
  if (secondCount >= 25) {/* 10/sec */
    {/* move ball */
      redrawScreen = 1;
      for(char axis = 0; axis < 8; axis+=2){
        short oldCol = controlPos[axis];
        short newCol = oldCol + colVelocity;
        if (newCol <= colLimits[0] || newCol >= colLimits[1])
	  colVelocity = -colVelocity;
        else
	  controlPos[axis] = newCol;
      }
    }
    P1OUT ^= BIT6;
    secondCount = 0;
    clearCount++;
  }
  secondCount++;
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }
}
