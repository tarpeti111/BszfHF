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
uint8_t game_getFallingSpeed();
void game_spawnFruit(void);
void game_action(void);
void game_handleInput(char c);

#endif /* GAME_H_ */
