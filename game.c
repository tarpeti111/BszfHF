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
#include "uart.h"
#include "string.h"
#include "rand_gen.h"

#define RANDOM_MAXIMUM SEGMENT_LCD_NUM_OF_LOWER_CHARS
#define GAME_END_COUNT 25

typedef struct {
    uint8_t player_pos;
    uint8_t falling_speed;
    uint8_t missed;
    uint8_t caught;
    bool started;
} GameState;

static GameState game;

void game_init(void){
  display_init();
  uart_init();
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
  display_updatePlayerPostition(game.player_pos);
}

void game_over(void){
  game.started = false;
  display_clear();
  display_updateScore(game.missed, game.caught);
}

uint8_t game_getFallingSpeed(){
  return game.falling_speed;
}

void game_spawnFruit(void){
  if(game.started){
    uint8_t randpos = rand_gen_getRand(RANDOM_MAXIMUM);

    fruit_addFruitAt(randpos);
    display_updateFruits(fruit_getFruits());
  }
}

void game_action(void){
    if(!game.started) return;

    // 1. advance all fruit states first
    fruit_incrementAll();

    // 2. process fallen fruit (those that reached the ground this frame)
    uint8_t fallen_idx;
    while ((fallen_idx = fruit_getNextFallen()) < NUM_OF_FRUITS && game.started) {
        Fruit* fruit_array = fruit_getFruits();

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
    display_updateFruits(fruit_getFruits());
    display_updateScore(game.missed, game.caught);
}

void transmit_speed(){
  char msg[13] = "speed is: ";
  char tmp = ((game.falling_speed / 10) % 10) + '0';
  char str_falling_speed[] = {(tmp == '0' ? ' ' : tmp),(game.falling_speed % 10) + '0', '\0'};
  strcat(msg, str_falling_speed);
  strcat(msg, "/s\n\r");
  USART_TxString(UART0, msg);
}

void clamp_falling_speed(uint8_t min, uint8_t max){
  if(min >= game.falling_speed){
      game.falling_speed = min;
  }
  else if(max <= game.falling_speed){
      game.falling_speed = max;
  }
}

void game_handleInputStarting(char c){
  switch (c){
        case 's':
          game_start();
          break;
        case '+':
          game.falling_speed++;
          clamp_falling_speed(1, 99);
          transmit_speed();
          break;
        case '-':
          game.falling_speed--;
          clamp_falling_speed(1, 99);
          transmit_speed();
          break;
      }
}
void game_handleInputInGame(char c){
  switch (c){
      case 'b':
        game.player_pos += 6;
        break;
      case 'j':
        game.player_pos += 1;
        break;
    }
  game.player_pos %= SEGMENT_LCD_NUM_OF_LOWER_CHARS;
  display_updatePlayerPostition(game.player_pos);
}

void game_handleInput(char c){
  if (c == '+' || c == '-' || c == 'j' || c == 'b' || c == 's') {
    if(game.started){
        game_handleInputInGame(c);
    }
    else{
        game_handleInputStarting(c);
    }
  }
}
