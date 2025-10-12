#include "em_device.h"
#include "stdbool.h"
#include "em_cmu.h"
#include "em_usart.h"
#include "em_gpio.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include <em_timer.h>

#define G 0x047d
#define A 0x0477
#define M 0x02b6
#define E 0x0079
#define O 0x003f
#define V 0x2230
#define R 0x0c73

volatile uint8_t animationFrame;

// TIMER0 interrupt handler
void TIMER0_IRQHandler(void) {
  TIMER_IntClear(TIMER0, TIMER_IF_OF); // Clear overflow interrupt flag
  animationFrame++;
  animationFrame %= 11;
}

// Scroll Animation
void animate(SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments, char *frames){
  lowerCharSegments[0].raw = frames[(animationFrame + 0) % 11];
  lowerCharSegments[1].raw = frames[(animationFrame + 1) % 11];
  lowerCharSegments[2].raw = frames[(animationFrame + 2) % 11];
  lowerCharSegments[3].raw = frames[(animationFrame + 3) % 11];
  lowerCharSegments[4].raw = frames[(animationFrame + 4) % 11];
  lowerCharSegments[5].raw = frames[(animationFrame + 5) % 11];
  lowerCharSegments[6].raw = frames[(animationFrame + 6) % 11];
  // only increment asd every 0.5 seconds or so, dont use a delay, use a peripheral timer
}

void USART_TxString(USART_TypeDef *usart, char *data, uint8_t lenght){
  for(int i = 0; i <= lenght; i++){
      USART_Tx(usart,data[i]);
  }
}

int main(void)
{
  CMU_ClockEnable(cmuClock_GPIO,true);
  CMU_ClockEnable(cmuClock_UART0,true);
  CMU_ClockEnable(cmuClock_TIMER0, true);

  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
  USART_InitAsync(UART0, &init);
  UART0->ROUTE = USART_ROUTE_LOCATION_LOC1 | USART_ROUTE_RXPEN | USART_ROUTE_TXPEN;

  GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull,1);
  GPIO_PinModeSet(gpioPortE, 1, gpioModeInput,0);
  GPIO_PinModeSet(gpioPortF,7,gpioModePushPull,1);

  SegmentLCD_Init(false);
  SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

  // ---------------- TIMER0 SETUP ----------------
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  timerInit.prescale = timerPrescale1024;
  TIMER_Init(TIMER0, &timerInit);
  TIMER_TopSet(TIMER0, 6836);
  TIMER_IntEnable(TIMER0, TIMER_IF_OF);
  NVIC_EnableIRQ(TIMER0_IRQn);
  TIMER_Enable(TIMER0, true);
  // ------------------------------------------------
  uint8_t cursorPosition = 0;
  bool b_gameover = false;

  while(1){
      // CLEAR LOWER SEGMENTS
      for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_LOWER_CHARS; p++) {
          for (uint8_t s = 0; s < 15; s++) {
              lowerCharSegments[p].raw = 1 << s;
          }
      }

      if(UART0->STATUS & USART_STATUS_RXDATAV){

        char recieved = USART_Rx(UART0);
        //debug input -> output
        USART_Tx(UART0,recieved);

        if(!b_gameover){
            if(recieved == 'd' && cursorPosition < 6){
                cursorPosition++;
            }
            else if(recieved == 'a' && cursorPosition > 0){
                cursorPosition--;
            }
            else if(recieved == '+'){
                //speed up
            }
            else if(recieved == '-'){
                //slow down
            }
        }
      if(recieved == 'r'){
          b_gameover = !b_gameover;
          for(int i  = 0; i < 50; i++){
              USART_Tx(UART0, '\n');
          }
          USART_Tx(UART0, '\r');
      }
    }
    if(!b_gameover){
        lowerCharSegments[cursorPosition].d = 1;
    }
    if(b_gameover){

    }
    //SegmentLCD_Number(cursorPosition);
    SegmentLCD_LowerSegments(lowerCharSegments);
 }
}
