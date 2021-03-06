#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer 0 and 1 */
void setupTimer(uint16_t period)
{
  /*
    TODO enable and set up the timer
    
    1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
    2. Write the period to register TIMER1_TOP
    3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
    4. Start the timer by writing 1 to TIMER1_CMD
    
    This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.
  */  


  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1; // enable clock for timer1
  *TIMER1_CTRL |= 0x05000000; // prescale set to 1:1024
  *TIMER1_TOP = period; // set period
  *TIMER1_IFC = 0x773; // clear all timer1 interrupts
  *TIMER1_IEN = 1; // enable interrupts

  /**CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER0; // enable clock for timer0
  *TIMER0_CTRL |= 0x05000000; // prescale set to 1:1024
  *TIMER0_TOP = period; // set period
  *TIMER0_IFC = 0x773; // clear all timer1 interrupts
  *TIMER0_IEN = 1; // enable interrupts */
}


void startTimer(uint16_t period)
{
  *TIMER1_TOP = period;
  *TIMER1_CMD = 1;
  /**TIMER0_TOP = period;
  *TIMER0_CMD = 1;*/

}

void stopTimer(void)
{
  *TIMER1_CMD = 2;
}

// setting low energy timer
void setupLETimer()
{
  *CMU_HFCORECLKEN0 |= 0x10; // Low Energy Peripheral Interface Clock Enable
  *CMU_LFACLKEN0 |= 0x4; // Low Energy Timer 0 Clock Enable
  *CMU_LFCLKSEL |= 0x2; // LFXO clock selected
  *CMU_OSCENCMD |= 0x100; //LFXO Enable
  *CMU_LFAPRESC0 |= (0b101 << 8); // magic prescale 1:64
  *LETIMER0_CMD = 0x2; // stop timer
  *LETIMER0_CTRL |= 0x200; // Compare value 0 is top value
  *LETIMER0_COMP0 = 1; // top value
  *LETIMER0_IFC = 0x1F; // clear interrupt flags
  *LETIMER0_IEN = 0x4; // enable underflow interrupt
}

void startLETimer()
{
  *LETIMER0_CMD = 0x1;
}

void stopLETimer()
{
  *LETIMER0_CMD = 0x2;
}
