/*
 * display.h
 *
 *  Created on: Nov 16, 2025
 *      Author: tarpe
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "fruit.h"
#include "stdint.h"

void display_init(void);

void display_updatePlayerPostition(uint8_t player_pos);
void display_updateFruits(Fruit* fruits);
void display_updateScore(uint8_t missed, uint8_t caught);

void display_clear(void);

#endif /* DISPLAY_H_ */
