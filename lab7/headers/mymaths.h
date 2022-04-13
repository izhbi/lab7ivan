/* 
 * File:   good_median_function.h
 * Author: team-6a
 *
 * Created on April 11, 2022, 12:12 PM
 */

#ifndef MYMATHS_H
#define	MYMATHS_H

#include "types.h"

// Fastest median computation of 5 numbers possibly in existence
// author claims only 6!! comparisons and NOTHING else 
// (source: https://stackoverflow.com/questions/480960/how-do-i-calculate-the-median-of-five-in-c)
uint16_t MedianOfFive(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

int16_t abs(int16_t);
float absf(float);

float clamp(float, float, float);

#endif	/* MYMATHS_H */

