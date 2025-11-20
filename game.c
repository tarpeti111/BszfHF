/*
 * game.c
 *
 *  Created on: Nov 15, 2025
 *      Author: tarpe
 */
#include "fruit.h"
#include "segmentlcd_individual.h"
#include "game.h"
#include "stdbool.h"
#include "display.h"
#include "string.h"
#include "rand_gen.h"

#define FALLING_SPEED_MAX 99 /**< Maximum allowed falling speed */
#define FALLING_SPEED_MIN 1  /**< Minimum allowed falling speed */
#define RANDOM_MAXIMUM SEGMENT_LCD_NUM_OF_LOWER_CHARS /**< Maximum random position for fruit spawn */
#define GAME_END_COUNT 25 /**< Total number of fruits before game ends */

/**
 * @brief Structure representing the game state.
 */
typedef struct {
    volatile uint8_t player_pos;   /**< Player's current position */
    volatile uint8_t falling_speed;/**< Current falling speed of fruits */
    volatile bool started;         /**< True if the game has started */

    uint8_t missed;                /**< Number of fruits missed */
    uint8_t caught;                /**< Number of fruits caught */
} GameState;

static GameState game; /**< Static instance of the game state */

void game_init(void){
  display_init();
  rand_gen_init();

  game.player_pos = 0;
  game.falling_speed = 1;
  game.started = false;
  game.missed = 0;
  game.caught = 0;
}

void game_start(void){
  fruit_init();

  game.started = true;
  game.missed = 0;
  game.caught = 0;
  display_togglePlayerPostition(game.player_pos, true);
}

void game_over(void){
  game.started = false;
  display_clear();
  display_updateScore(game.missed, game.caught);
}

void game_spawnFruit(void){
  if(game.started){
    uint8_t randpos = rand_gen_getRand(RANDOM_MAXIMUM);

    fruit_addFruitAt(randpos);
    display_toggleFruits(fruit_getFruits(), true);
  }
}

void game_action(void){
    if(!game.started) return;

    Fruit* fruit_array = fruit_getFruits();

    display_toggleFruits(fruit_array, false);
    // 1. advance all fruit states first
    fruit_incrementAll();

    // 2. process fallen fruit (those that reached the ground this frame)
    uint8_t fallen_idx;
    while ((fallen_idx = fruit_getNextFallen()) < NUM_OF_FRUITS && game.started) {

        if (game.player_pos == fruit_array[fallen_idx].pos) {
            game.caught++;
        } else {
            game.missed++;
        }

        fruit_removeFruit(fallen_idx);

        if (game.caught + game.missed >= GAME_END_COUNT) {
            game_over();
        }
    }

    // 3. now render fruits and score AFTER logic is done
    if(game.started){
        display_toggleFruits(fruit_array, true);
    }
    display_updateScore(game.missed, game.caught);
}

bool game_getStarted(void){
  return game.started;
}

uint8_t game_getPlayerPosition(){
  return game.player_pos;
}

void game_increasePlayerPosition(){
  game.player_pos = (game.player_pos >= (SEGMENT_LCD_NUM_OF_LOWER_CHARS - 1)) ? 0 : game.player_pos + 1;
}

void game_decreasePlayerPosition(){
  game.player_pos = (game.player_pos == 0) ? (SEGMENT_LCD_NUM_OF_LOWER_CHARS - 1) : game.player_pos - 1;
}

uint8_t game_getFallingSpeed(){
  return game.falling_speed;
}

void game_increaseSpeed(){
  game.falling_speed = (game.falling_speed >= FALLING_SPEED_MAX) ? FALLING_SPEED_MAX : game.falling_speed + 1;
}

void game_decreaseSpeed(){
  game.falling_speed = (game.falling_speed <= FALLING_SPEED_MIN) ? FALLING_SPEED_MIN : game.falling_speed - 1;
}
