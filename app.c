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
#include "horizontalSegment.h"

#define GAME_SPAWN_TIME_MS 3001  /**< Time interval between spawning fruits (ms) */
#define GAME_TICK_TIME_MS 1000   /**< Base game logic tick interval (ms) */

volatile uint32_t msTicks = 0;          /**< System millisecond counter */
volatile uint32_t timerGameAction;      /**< Countdown timer for game logic ticks */
volatile uint32_t timerSpawn;           /**< Countdown timer for spawning fruits */

/**
 * @brief SysTick interrupt handler.
 *
 * Increments the millisecond counter and decrements game timers.
 */
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
  CMU_ClockEnable(cmuClock_GPIO, true);

  game_init();
  horizontalSegment_uartInit();

  timerGameAction = GAME_TICK_TIME_MS;
  timerSpawn = GAME_SPAWN_TIME_MS;

  /* Setup SysTick Timer for 1 millisecond interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE)/1000)) {
    while (1) ;
  }

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  if(timerGameAction == 0){
      game_action();
      timerGameAction = GAME_TICK_TIME_MS / game_getFallingSpeed();
  }
  if(timerSpawn == 0){
      game_spawnFruit();
      timerSpawn = GAME_SPAWN_TIME_MS;
  }
}
