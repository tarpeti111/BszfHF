/*
 * rand_gen.c
 *
 *  Created on: Nov 17, 2025
 *      Author: tarpe
 */

#include <stdlib.h>
#include "stdbool.h"
#include "em_adc.h"
#include "em_cmu.h"

bool initialized = false; /**< Flag indicating if the random generator has been initialized */

/**
 * @brief Initializes the ADC for generating entropy/noise.
 *
 * Configures ADC0 with default settings and a single conversion channel.
 */
void adc_init(void) {
  CMU_ClockEnable(cmuClock_ADC0, true);

  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_Init(ADC0, &init);

  ADC_InitSingle_TypeDef sinit = ADC_INITSINGLE_DEFAULT;

  sinit.acqTime = adcAcqTime16;              // safe acquisition
  sinit.reference = adcRef1V25;              // internal reference
  sinit.prsSel = adcPRSSELCh0;        // unconnected / noise channel
  ADC_InitSingle(ADC0, &sinit);
}

/**
 * @brief Generates a 32-bit seed from ADC noise.
 *
 * Reads 32 single-bit values from the ADC and assembles them into a 32-bit integer.
 *
 * @return 32-bit random seed
 */
uint32_t seed_from_adc_noise(void) {
  uint32_t s = 0;
  for (int i = 0; i < 32; i++) {
    ADC_Start(ADC0, ADC_CMD_SINGLESTART);
    while (!(ADC0->STATUS & ADC_STATUS_SINGLEDV));
    uint32_t v = ADC_DataSingleGet(ADC0);
    s = (s << 1) | (v & 1);
  }
  return s;
}

void rand_gen_init(){
  adc_init();
  srand(seed_from_adc_noise());
  initialized = true;
}

uint8_t rand_gen_getRand(uint8_t exclusive_max) {

  return (uint8_t)(rand() % exclusive_max);
}
