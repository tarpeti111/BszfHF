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

/** @brief Array representing the state of each lower LCD character. */
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
    Fruit fruit = fruits[i];
    uint16_t mask = (1 << (uint16_t)fruit.state);
    if(on){
        lowerCharSegments[fruit.pos].raw |= mask;
    }
    else{
        lowerCharSegments[fruit.pos].raw &= ~mask;
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
