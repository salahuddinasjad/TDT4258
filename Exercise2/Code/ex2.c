#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "efm32gg.h"
#include "interrupt_handlers.h"
#include "gpio.h"
#include "timer.h"
#include "dac.h"
#include "sounds.h"
#include "dma.h"


uint8_t play_sound = 0;
extern uint8_t finished;

/* Declaration of peripheral setup functions */
void setupNVIC();

/* Your code will start executing here */
int main(void) 
{  
  uint8_t i = 0;
  /* Call the peripheral setup functions */
  setupGPIO();
  setupDAC();
  setupLETimer();
  //setupTimer(SAMPLE_PERIOD);
  //setupDMA(SAMPLE_PERIOD);

  /* Enable interrupt handling */
  setupNVIC();
  
  /* TODO for higher energy efficiency, sleep while waiting for interrupts
     instead of infinite loop for busy-waiting
  */

  *SCR = 4; // EM2 sleep mode

 
  while(1)
  { 
    __asm("wfi");
    if(play_sound != 0){
        *SCR = 0;
		for (i = 0; i<10; i++){	
        	play(i,play_sound);
        // waiting for finishing last sound
        	while (finished == 0){
              __asm("wfi");
        	}
        }  
        *SCR = 4;
      	play_sound = 0; // clear flag		
	}

  }
  return 0;
}


void setupNVIC()
{
  /* TODO use the NVIC ISERx registers to enable handling of interrupt(s)
     remember two things are necessary for interrupt handling:
      - the peripheral must generate an interrupt signal
      - the NVIC must be configured to make the CPU handle the signal
     You will need TIMER1, GPIO odd and GPIO even interrupt handling for this
     assignment.
  */
  *ISER0 = 0x4100000802; //  // allow NVIC interrupts for EVEN and ODD GPIO and TIMER1 and TIMER0 and DMA. (page 12 in datasheet)
  *ISER0 |= (1 << 26); // enable LETIMER0
}


/* if other interrupt handlers are needed, use the following names: 
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/
