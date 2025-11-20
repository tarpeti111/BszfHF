/*
 * fruit.h
 *
 *  Created on: Nov 15, 2025
 *      Author: tarpe
 */
#ifndef FRUIT_H_
#define FRUIT_H_

#include "stdbool.h"
#include "stdint.h"
#include "stdbool.h"

#define SEGMENT_LCD_FRUIT_STARTING 0
#define SEGMENT_LCD_FRUIT_FALLING_UPPER 8
#define SEGMENT_LCD_FRUIT_FALLING_LOWER 12

//maximum number of fruits
#define NUM_OF_FRUITS SEGMENT_LCD_NUM_OF_LOWER_CHARS

typedef enum {
  OFFLINE = 255,
  STARTING = SEGMENT_LCD_FRUIT_STARTING,
  FALLING_UPPER = SEGMENT_LCD_FRUIT_FALLING_UPPER,
  FALLING_LOWER = SEGMENT_LCD_FRUIT_FALLING_LOWER,
  FALLEN
} FruitState;

typedef struct {
  FruitState state;
  uint8_t pos;
} Fruit;

void fruit_init(void);
bool fruit_addFruitAt(uint8_t position);
bool fruit_removeFruit(uint8_t idx);
void fruit_copyArray(Fruit* from, Fruit* to);
void fruit_incrementAll(void);
void fruit_incrementOne(uint8_t idx);
Fruit* fruit_getFruits(void);
uint8_t fruit_getNextFallen(void);

#endif /* FRUIT_H_ */
