/*
 * rand_gen.h
 *
 *  Created on: Nov 17, 2025
 *      Author: tarpe
 */

#ifndef RAND_GEN_H_
#define RAND_GEN_H_

/**
 * @brief Initializes the random number generator.
 *
 * Seeds the standard library `rand()` function using ADC noise.
 * Sets the `initialized` flag to true.
 */
void rand_gen_init(void);

/**
 * @brief Returns a random integer in the range [0, exclusive_max).
 *
 * @param exclusive_max Upper bound for random number (exclusive)
 * @return Random uint8_t in range [0, exclusive_max)
 */
uint8_t rand_gen_getRand(uint8_t exclusive_max);

#endif /* RAND_GEN_H_ */
