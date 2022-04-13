/* 
 * File:   calibrate.h
 * Author: team-6a
 *
 * Created on April 11, 2022, 8:57 PM
 */

#include "types.h"

#ifndef CALIBRATE_H
#define	CALIBRATE_H

uint16_t* touchscreen_calibration_data(uint8_t);
int16_t* servos_calibration_data(uint8_t);

void set_servos_calibration(uint8_t, int16_t, int16_t);
void set_touchscreen_calibration(uint8_t, uint16_t, uint16_t);

void calibrate_touchscreen(uint8_t);
void calibrate_servos(uint8_t);

#endif	/* CALIBRATE_H */

