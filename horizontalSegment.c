/*
 * horizonalSegment.c
 *
 *  Created on: Nov 12, 2025
 *      Authors: tarpe JK42GI,
 *              pappistvan QNMGJE
 */


#include "em_usart.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include <string.h>
#include "horizontalSegment.h"
#include <stdio.h>
#include "game.h"
#include "display.h"

typedef void (*HorizontalMove)(void);
volatile char input;


void horizontalSegment_uartInit(){
  // Set PF7 to high
  GPIO_PinModeSet(gpioPortF, 7, gpioModePushPull, 1);
  // Configure UART0
  // (Now use the "emlib" functions whenever possible.)
  // Enable clock for UART0
  CMU_ClockEnable(cmuClock_UART0, true);
  // Initialize UART0 (115200 Baud, 8N1 frame format)
  // To initialize the UART0, we need a structure to hold
  // configuration data. It is a good practice to initialize it with
  // default values, then set individual parameters only where needed.
  USART_InitAsync_TypeDef UART0_init = USART_INITASYNC_DEFAULT;
  USART_InitAsync(UART0, &UART0_init);
  // USART0: see in efm32ggf1024.h
  // Set TX (PE0) and RX (PE1) pins as push-pull output and input resp.
  // DOUT for TX is 1, as it is the idle state for UART communication
  GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);
  // DOUT for RX is 0, as DOUT can enable a glitch filter for inputs,
  // and we are fine without such a filter
  GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);
  // Use PE0 as TX and PE1 as RX (Location 1, see datasheet (not refman))
  // Enable both RX and TX for routing
  UART0->ROUTE |= UART_ROUTE_LOCATION_LOC1;
  // Select "Location 1" as the routing configuration
  UART0->ROUTE |= UART_ROUTE_TXPEN | UART_ROUTE_RXPEN;

  USART_IntClear(UART0, _USART_IF_MASK);
  USART_IntEnable(UART0, USART_IEN_RXDATAV);
  NVIC_ClearPendingIRQ(UART0_RX_IRQn);
  NVIC_EnableIRQ(UART0_RX_IRQn);
}



/**
    * @brief interrupt handler of UART's rx buferr
 */
void UART0_RX_IRQHandler(void) {




  input=USART_RxDataGet(UART0);



     if(input=='s') {
              game_start();



            }
     if(!game_getStarted()) {processCommandInitial();
     if('+'==input|| '-'==input) {
     printf("speed is: %d /s for falling\r\n", game_getFallingSpeed());
     }
     }

     if(game_getStarted()) {
         processCommand();


     }

}


void processCommand(void);



/**
 * @brief struct for callback func and its description that is associated in @see void processCommand()
 */
typedef struct {
  char description; //direction of movement
  HorizontalMove func; //callback func to implemented movement
  } CommandMove;


#define CMD_COUNT 2  //given chars from specification

  void go_left(void);
  void go_right(void) ;
  void decreaseSpeed(void);
  void increaseSpeed(void);


  /**
   * @brief instance of @see CommandMove type w discription of player position (that is input) ' s command and corresponding func
   * @details cases when game has started
   */
 CommandMove commandTable[] = {

     {'b',go_left },
     {'j', go_right}


 } ;


 /**
   * @brief instance of @see CommandMove type w discription of speed (that is input) command and corresponding func
   * @details case : when game is not started yet
   */
 CommandMove commandTableInitial[] = {

     {'-', decreaseSpeed},
     {'+', increaseSpeed}

 };

 /** @brief iterating over instanse of Commandmove (case:game is started) and calling callback func for its corresponding discripton (input)
    * @param[in] void
    * @return void
    */
 void processCommand() {
   for(uint8_t i=0; i<CMD_COUNT; i++){
       if(input==commandTable[i].description)
         commandTable[i].func();  //invoking corresponding func due to discription for example go left)
   }
 }



 /** @brief iterating over instanse of Commandmove (case:game is not started) and calling callback func for its corresponding discripton (input)
    * @param[in] void
    * @return void
    */
 void processCommandInitial(void) {
   for(uint8_t i=0; i<CMD_COUNT; i++){
       if(input==commandTableInitial[i].description)
         commandTableInitial[i].func();  //invoking corresponding func due to discription for example go left)
   }
 }

 //impelemntenting commandTable funcs
//********************************************************************************************************************************************






 /**
  * @brief increasing player's position for right direction @see void game_increasePlayerPosition(void)
  *@details toogling horizontal segment: -incremented pos is on by@see void display_togglePlayerPostition(uint8_t player_pos, bool on)
  *                                       -current pos is off
  * @param[in] void
  * @return void
  */
   void go_right(void) {
   display_togglePlayerPostition(game_getPlayerPosition(), false);
   game_increasePlayerPosition();
   display_togglePlayerPostition(game_getPlayerPosition(), true);
 }



   /**
     * @brief decreasing  player's position for right direction @see void game_decreasePlayerPosition()(void)
     *@details toogling horizontal segment: -decremented pos is on by@see void display_togglePlayerPostition(uint8_t player_pos, bool on)
     *                                       -current pos is off
     * @param[in] void
     * @return void
     */
 void go_left(void) {
   display_togglePlayerPostition(game_getPlayerPosition(), false);
   game_decreasePlayerPosition();
   display_togglePlayerPostition(game_getPlayerPosition(), true);
 }



 /**
      * @brief absraction over @see game_increaseSpeed()
      * @param[in] void
      * @return void
      */
 void increaseSpeed(void) {
     game_increaseSpeed();
 }


 /**
       * @brief absraction over @see game_decreaseSpeed();
       * @param[in] void
       * @return void
       */
 void decreaseSpeed(void){
     game_decreaseSpeed();
 }


/**
 * @brief overwriting build_in write for formatting string
 *  it s polling
*/
int _write(int file, char *ptr, int len)
 {
     (void)file; //ingoring file discriptor for notwarning


     for (int i = 0; i < len; i++)
        {
         while (!(UART0->STATUS & USART_STATUS_TXBL));
                 UART0->TXDATA = ptr[i];
        }



     return len; //not relevant
 }

void horizontalSegment_uartFlush() {
    while (!(UART0->STATUS & USART_STATUS_TXC));
}

void horizontalSegment_TxScore() {
    printf(GAME_END_STRING "Score: %d\r\n", game_getScore());
}
