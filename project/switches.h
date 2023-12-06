#ifndef switches_included
#define switches_included

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

extern short drawPos[8];
extern short colPos[8];
extern short clearCount;
extern short redrawScreen;

void switch_init();
void switch_interrupt_handler();
static char switch_update_interrupt_sense();

#endif // included
