/*
 * display.h
 *
 *  Created on: Nov 16, 2025
 *      Authors: tarpe JK42GI,
 *              pappistvan QNMGJE
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "fruit.h"
#include "stdint.h"

/**
 * @brief Initializes the LCD display and internal segment array.
 *
 * Clears all lower segments, initializes the SegmentLCD driver, sets initial number to 0,
 * and turns on the colon symbol.
 */
void display_init(void);

/**
 * @brief Toggles the player’s position on the lower LCD.
 *
 * @param player_pos The index of the lower LCD character where the player is located.
 * @param on true to turn on the player segment, false to turn it off.
 */
void display_togglePlayerPostition(uint8_t player_pos, bool on);

/**
 * @brief Toggles the display of fruits on the lower LCD.
 *
 * Updates the lower segments according to the fruit positions and states.
 *
 * @param fruits pointer to an Array of Fruit structures representing all fruits in the game.
 * @param on true to turn on the fruit segments, false to turn them off.
 */
void display_toggleFruits(Fruit* fruits, bool on);

/**
 * @brief Updates the score display on the upper LCD.
 *
 * The score is displayed as (caught * 100 + missed).
 *
 * @param missed Number of fruits missed by the player.
 * @param caught Number of fruits caught by the player.
 */
void display_updateScore(uint8_t missed, uint8_t caught);

/**
 * @brief Clears all lower segments on the LCD.
 *
 * Resets the internal segment array and updates the display.
 */
void display_clear(void);

#endif /* DISPLAY_H_ */
