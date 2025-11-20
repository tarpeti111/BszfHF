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

void display_init(void){
  for(uint8_t i = 0; i < SEGMENT_LCD_NUM_OF_LOWER_CHARS; i++){
      lowerCharSegments[i].raw = 0;
  }

  SegmentLCD_Init(false);

  SegmentLCD_Number(0);
  SegmentLCD_Symbol(LCD_SYMBOL_COL10, 1);
}

void display_togglePlayerPostition(uint8_t player_pos, bool on){

  lowerCharSegments[player_pos].d = on;

  //update the lcd screen
  SegmentLCD_LowerSegments(lowerCharSegments);
}

void display_toggleFruits(Fruit fruits[], bool on){
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
