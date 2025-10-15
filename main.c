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

volatile uint8_t gameTicks = 0;
volatile uint8_t animationFrame = 0;
volatile bool b_gameTick = false;

// TIMER0 interrupt handler
void TIMER0_IRQHandler(void) {
  TIMER_IntClear(TIMER0, TIMER_IF_OF); // Clear overflow interrupt flag
  gameTicks++;
  gameTicks %= 256;
  b_gameTick = true;
}
void TIMER1_IRQHandler(void) {
  TIMER_IntClear(TIMER1, TIMER_IF_OF); // Clear overflow interrupt flag
    animationFrame++;
    animationFrame %= 11;
}

// Scroll Animation
void animate(SegmentLCD_LowerCharSegments_TypeDef *lowerCharSegments, uint16_t *frames, uint8_t maxFrames){
  for(int i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
      lowerCharSegments[i].raw = frames[(animationFrame + i) % maxFrames];
  }
}

void USART_TxString(USART_TypeDef *usart, char *data, uint8_t lenght){
  for(int i = 0; i <= lenght; i++){
      USART_Tx(usart,data[i]);
  }
}

// TODO: MAKE THIS
uint8_t getrand(uint8_t begin, uint8_t end){
  return 0;
}

int main(void)
{
  uint16_t points = 0;
  uint16_t gameOverFrames[] = {G,A,M,E,0,O,V,E,R,0,0};
  uint8_t cursorPosition = 0;
  bool b_gameover = false;
  uint8_t timeScale = 10;
  int bananas[SEGMENT_LCD_NUM_OF_LOWER_CHARS];
  for(uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
      bananas[i] = -1;
  }

  CMU_ClockEnable(cmuClock_GPIO,true);
  CMU_ClockEnable(cmuClock_UART0,true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_TIMER1, true);

  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
  USART_InitAsync(UART0, &init);
  UART0->ROUTE = USART_ROUTE_LOCATION_LOC1 | USART_ROUTE_RXPEN | USART_ROUTE_TXPEN;

  GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull,1);
  GPIO_PinModeSet(gpioPortE, 1, gpioModeInput,0);
  GPIO_PinModeSet(gpioPortF,7,gpioModePushPull,1);

  SegmentLCD_Init(false);
  SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

  // GAME TIMER
  // ---------------- TIMER0 SETUP ----------------
  TIMER_Init_TypeDef timer0Init = TIMER_INIT_DEFAULT;
  timer0Init.prescale = timerPrescale1024;
  TIMER_Init(TIMER0, &timer0Init);
  TIMER_TopSet(TIMER0, 13672);
  TIMER_IntEnable(TIMER0, TIMER_IF_OF);
  NVIC_EnableIRQ(TIMER0_IRQn);
  //TIMER_Enable(TIMER0, true);
  // ------------------------------------------------

  // ANIMATION TIMER
  // ---------------- TIMER1 SETUP ----------------
  TIMER_Init_TypeDef timer1Init = TIMER_INIT_DEFAULT;
  timer1Init.prescale = timerPrescale1024;
  TIMER_Init(TIMER1, &timer1Init);
  TIMER_TopSet(TIMER1, 13672);
  TIMER_IntEnable(TIMER1, TIMER_IF_OF);
  NVIC_EnableIRQ(TIMER1_IRQn);
  TIMER_Enable(TIMER1, true);
  // ------------------------------------------------
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
        uint8_t lastTimeScale = timeScale;
        switch (recieved){
          case 'd':
            if(!b_gameover)
              cursorPosition++;
            break;
          case 'a':
            if(!b_gameover)
              cursorPosition += 6;
            break;
          case '+':
            timeScale++;
            break;
          case '-':
            timeScale--;
            break;
          case 'r':
            b_gameover = !b_gameover;
            for(int i  = 0; i < 50; i++){
                USART_Tx(UART0, '\n');
            }
            for(int j = 0; j < SEGMENT_LCD_NUM_OF_LOWER_CHARS; j++){
                bananas[j] = -1;
            }
            USART_Tx(UART0, '\r');
            break;
          default:
            break;
        }
        cursorPosition %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
        if(timeScale != lastTimeScale){
            if(timeScale < 1){
                timeScale = 1;
            }
            TIMER_TopSet(TIMER0, 1367*timeScale);

            if(TIMER_CounterGet(TIMER0) >= 1367*timeScale){
              TIMER_CounterSet(TIMER0, 0);
            }
        }
    }
    if(!b_gameover){
        lowerCharSegments[cursorPosition].d = 1;

        for(int i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
          switch (bananas[i]){
            case 0:
              lowerCharSegments[i].a = 1;
              break;
            case 1:
              lowerCharSegments[i].j = 1;
              break;
            case 2:
              lowerCharSegments[i].p = 1;
              break;
            default:
              break;
          }
        }

        if(b_gameTick){
            b_gameTick = false;
            for(int i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
                if(bananas[i] >= 0 && bananas[i] < 3){
                    bananas[i]++;
                }
                if(bananas[i] == 3){
                  if(cursorPosition == i){
                      bananas[i] = -1;
                      points++;
                  }
                  else{
                      b_gameover = true;
                      for(int j = 0; j < SEGMENT_LCD_NUM_OF_LOWER_CHARS; j++){
                          bananas[j] = -1;
                      }
                  }
                }
            }
            if(gameTicks % 4 == 0){
                uint8_t newBananaLocation = getrand(0,6);
                while(bananas[newBananaLocation] != -1){
                  newBananaLocation++;
                  newBananaLocation %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
                }
                bananas[newBananaLocation] = 0;
            }
        }
    }
    if(b_gameover){
        animate(lowerCharSegments, gameOverFrames, 11);
    }
    SegmentLCD_Number(points);
    SegmentLCD_LowerSegments(lowerCharSegments);
 }
}
