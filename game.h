/*
 * game.h
 *
 *  Created on: Nov 15, 2025
 *      Authors: tarpe JK42GI,
 *              pappistvan QNMGJE
 */

#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include <stdint.h>

#define GAME_END_STRING "<END>" /**< String for higher level application to tell that the game has ended*/

/**
 * @brief Initializes the game state.
 *
 * Sets the player position to 0, falling speed to initial value,
 * resets score counters, and initializes the display and random generator.
 */
void game_init(void);

/**
 * @brief Starts the game.
 *
 * Initializes fruits, sets the game to started, resets score counters,
 * and displays the player on the LCD.
 */
void game_start(void);

/**
 * @brief Spawns a new fruit at a random lower LCD position.
 *
 * Only spawns a fruit if the game is currently started.
 */
void game_spawnFruit(void);

/**
 * @brief Updates the game state for a single tick.
 *
 * Advances fruit states, handles caught/missed fruits,
 * and updates the LCD display and score.
 */
void game_action(void);

/**
 * @brief Checks if the game is currently running.
 *
 * @return true if the game is started, false otherwise.
 */
bool game_getStarted(void);

/**
 * @brief Returns the current player position on the lower LCD.
 *
 * @return Player position index.
 */
uint8_t game_getPlayerPosition(void);

/**
 * @brief Moves the player one position to the right.
 *
 * Wraps around to the first position if at the end.
 */
void game_increasePlayerPosition(void);

/**
 * @brief Moves the player one position to the left.
 *
 * Wraps around to the last position if at the start.
 */
void game_decreasePlayerPosition(void);

/**
 * @brief Returns the current falling speed of fruits.
 *
 * @return Falling speed value.
 */
uint8_t game_getFallingSpeed(void);

/**
 * @brief Increases the falling speed by one unit.
 *
 * Caps at the defined maximum speed.
 */
void game_increaseFallingSpeed(void);

/**
 * @brief Decreases the falling speed by one unit.
 *
 * Caps at the defined minimum speed.
 */
void game_decreaseFallingSpeed(void);

#endif /* GAME_H_ */
