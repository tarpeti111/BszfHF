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

#ifndef APP_H
#define APP_H

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

/**
 * @brief Initialize application and peripherals.
 *
 * - Enables GPIO clock
 * - Initializes game state and display
 * - Initializes UART input for player control
 * - Configures SysTick timer for 1 ms ticks
 */
void app_init(void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/

/**
 * @brief Periodic application processing function.
 *
 * This function should be called in the main loop.
 * - Updates game logic if timerGameAction has elapsed
 * - Spawns new fruits if timerSpawn has elapsed
 */
void app_process_action(void);

#endif  // APP_H
