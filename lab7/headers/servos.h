/* 
 * File:   servos.h
 * Author: team-6a
 *
 * Created on April 11, 2022, 8:32 PM
 */
#include "types.h"

#ifndef SERVOS_H
#define	SERVOS_H

#define SERVO_MIN -120
#define SERVO_MAX 120

#define SERVO_X_CHAN 0
#define SERVO_Y_CHAN 1

void servos_init(void);
void servo_set_duty(uint8_t, int16_t);

#endif	/* SERVOS_H */

