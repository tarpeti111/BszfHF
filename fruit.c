/*
 * fruit.c
 *
 *  Created on: Nov 15, 2025
 *      Author: tarpe
 */
#include "stdbool.h"
#include "segmentlcd_individual.h"
#include "fruit.h"

Fruit fruit_array[NUM_OF_FRUITS];

Fruit* fruit_getFruits(void){
  return fruit_array;
}

void fruit_init(void){
  for(uint8_t i = 0; NUM_OF_FRUITS > i; i++){
      fruit_array[i].state = OFFLINE;
      fruit_array[i].pos = 255;
  }
}

bool fruit_addFruitAt(uint8_t position){
  for(uint8_t i = 0; NUM_OF_FRUITS > i; i++){
      Fruit* fruit = &fruit_array[i];

      if(OFFLINE == fruit->state){
          fruit->state = STARTING;
          fruit->pos = position;
          return true;
      }
  }
  return false;
}

bool fruit_removeFruit(uint8_t idx){
  if(NUM_OF_FRUITS <= idx){
      return false;
  }
  fruit_array[idx].state = OFFLINE;
  fruit_array[idx].pos = 255;
  return true;
}

void fruit_copyArray(Fruit* from, Fruit* to){
  for(uint8_t i = 0; NUM_OF_FRUITS > i; i++){
      to[i].pos = from[i].pos;
      to[i].state = from[i].state;
  }
}

void fruit_incrementOne(uint8_t idx){
  if(NUM_OF_FRUITS <= idx){
      return;
  }
  Fruit* fruit = &fruit_array[idx];
  if((void*)0 != fruit){
      if(OFFLINE != fruit->state && FALLEN != fruit->state){
          switch (fruit->state){
            case STARTING:
              fruit->state = FALLING_UPPER;
              break;
            case FALLING_UPPER:
              fruit->state = FALLING_LOWER;
              break;
            case FALLING_LOWER:
              fruit->state = FALLEN;
              break;
            default:
          }
      }
  }
}

void fruit_incrementAll(void){
  for(uint8_t i = 0; NUM_OF_FRUITS > i; i++){
      fruit_incrementOne(i);
  }
}

uint8_t fruit_getNextFallen(void){
  for(uint8_t i = 0; NUM_OF_FRUITS > i; i++){
        if(FALLEN == fruit_array[i].state){
            return i;
        }
    }
  return 255;
}

