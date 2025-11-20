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

/** Starting segment index for a newly spawned fruit on the lower LCD */
#define SEGMENT_LCD_FRUIT_STARTING 0
/** Segment index for the upper part of a falling fruit */
#define SEGMENT_LCD_FRUIT_FALLING_UPPER 8
/** Segment index for the lower part of a falling fruit */
#define SEGMENT_LCD_FRUIT_FALLING_LOWER 12

/** Maximum number of fruits allowed at once (matches number of lower LCD characters) */
#define NUM_OF_FRUITS SEGMENT_LCD_NUM_OF_LOWER_CHARS

/**
 * @brief Possible states of a fruit in the game
 */
typedef enum {
  OFFLINE = 255,               /**< Fruit slot is empty / inactive */
  STARTING = SEGMENT_LCD_FRUIT_STARTING,   /**< Fruit just spawned */
  FALLING_UPPER = SEGMENT_LCD_FRUIT_FALLING_UPPER, /**< Fruit in upper falling segment */
  FALLING_LOWER = SEGMENT_LCD_FRUIT_FALLING_LOWER, /**< Fruit in lower falling segment */
  FALLEN                        /**< Fruit has reached the bottom and needs processing */
} FruitState;

/**
 * @brief Represents a fruit and its position on the LCD
 */
typedef struct {
  FruitState state;   /**< Current state of the fruit */
  uint8_t pos;        /**< Index of the lower LCD character where the fruit is displayed */
} Fruit;

/**
 * @brief Initialize all fruit slots to OFFLINE
 */
void fruit_init(void);

/**
 * @brief Adds a fruit at a specific position if a slot is available
 * @param position Index of the lower LCD character to place the fruit
 * @return true if a fruit was added, false if no slots were available
 */
bool fruit_addFruitAt(uint8_t position);

/**
 * @brief Removes a fruit at the given slot index
 * @param idx Index of the fruit in the internal array
 * @return true if removal succeeded, false if index was invalid
 */
bool fruit_removeFruit(uint8_t idx);

/**
 * @brief Increment the state of all active fruits
 */
void fruit_incrementAll(void);

/**
 * @brief Increment the state of a single fruit
 * @param idx Index of the fruit in the internal array
 */
void fruit_incrementOne(uint8_t idx);

/**
 * @brief Returns the internal array of fruits
 * @return Pointer to the internal fruit array
 */
Fruit* fruit_getFruits(void);

/**
 * @brief Get the index of the next fallen fruit
 * @return Index of a fruit with state FALLEN, or 255 if none are fallen
 */
uint8_t fruit_getNextFallen(void);

#endif /* FRUIT_H_ */
