/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "display.h"
#include "game.h"
#include "em_usart.h"
#include "em_cmu.h"
#include "uart.h"

volatile uint32_t msTicks = 0;

volatile uint32_t timerGameAction;
volatile uint32_t timerSpawn;

void SysTick_Handler(void){
    msTicks++;

    if (timerGameAction > 0) timerGameAction--;
    if (timerSpawn > 0)      timerSpawn--;
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

void app_init(void)
{
  // Enable clock for GPIO
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;

  game_init();

  msTicks = 0;
  timerGameAction = 1000;
  timerSpawn = 1500;

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE)/1000)) {
    while (1) ;
  }

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  char ch;
  ch = USART_RxNonblocking(UART0);
  if (ch != 0) {
      game_handleInput(ch);
  }
  if(timerGameAction == 0){
      game_action();
      timerGameAction = 1000 / game_getFallingSpeed();
  }
  if(timerSpawn == 0){
      game_spawnFruit();
      timerSpawn = 3333;
  }
}
