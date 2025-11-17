/*
 * display.c
 *
 *  Created on: Nov 16, 2025
 *      Author: tarpe
 */
#include "display.h"
#include "segmentlcd_individual.h"
#include "stdint.h"
#include "segmentlcdconfig.h"
#include "segmentlcd.h"

SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegmentsOFF[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

uint8_t prev_player_pos;
Fruit prev_fruits_array[NUM_OF_FRUITS];

void display_init(void){
  for(uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
      lowerCharSegmentsOFF[i].raw = 0;
  }
  for(uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
      lowerCharSegments[i].raw = 0;
  }

  SegmentLCD_Init(false);

  SegmentLCD_Number(0);
  SegmentLCD_Symbol(LCD_SYMBOL_COL10, 1);
}

void display_updatePlayerPostition(uint8_t player_pos){
  //disable the bottom part of the LCD character segment where the player was
  lowerCharSegments[prev_player_pos].d = 0;
  //set the previous player position to the current player position
  prev_player_pos = player_pos;

  //enable the bottom part of the LCD character segment where the player is
  lowerCharSegments[player_pos].d = 1;

  //update the lcd screen
  SegmentLCD_LowerSegments(lowerCharSegments);
}

static void toggleFruits(Fruit* fruits, bool on){
  for(uint8_t i = 0; NUM_OF_FRUITS > i; i++){
        Fruit* fruit = &fruits[i];
        switch (fruit->state){
          case STARTING:
            lowerCharSegments[fruit->pos].a = on;
            break;
          case FALLING_UPPER:
            lowerCharSegments[fruit->pos].j = on;
            break;
          case FALLING_LOWER:
            lowerCharSegments[fruit->pos].p = on;
            break;
          default:
            break;
        }
  }
}

void display_updateFruits(Fruit fruits[]){
  //disable previously on fruits
  toggleFruits(prev_fruits_array, false);
  //set previous_fruit_array-s elements to the current fruit_array-s elements
  fruit_copyArray(fruits, prev_fruits_array);
  //enable current fruits
  toggleFruits(fruits, true);
  //update the lcd screen
  SegmentLCD_LowerSegments(lowerCharSegments);
}

void display_updateScore(uint8_t missed, uint8_t caught){
  //update score
  SegmentLCD_Number(caught*100 + missed);
}

void display_clear(void){
  for(uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
      lowerCharSegments[i].raw = 0;
  }
  SegmentLCD_LowerSegments(lowerCharSegments);
}
