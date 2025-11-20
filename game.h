/*
 * game.h
 *
 *  Created on: Nov 15, 2025
 *      Author: tarpe
 */

#ifndef GAME_H_
#define GAME_H_

void game_init(void);
void game_start(void);
void game_spawnFruit(void);
void game_action(void);

bool game_getStarted(void);

uint8_t game_getPlayerPosition(void);
void game_increasePlayerPosition(void);
void game_decreasePlayerPosition(void);

uint8_t game_getFallingSpeed(void);
void game_increaseFallingSpeed(void);
void game_decreaseFallingSpeed(void);

#endif /* GAME_H_ */
