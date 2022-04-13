/* 
 * File:   touchscreen.h
 * Author: team-6a
 *
 * Created on April 11, 2022, 8:36 PM
 */

#include "types.h"

#ifndef TOUCHSCREEN_H
#define	TOUCHSCREEN_H

#define TOUCH_X_AXIS 0
#define TOUCH_Y_AXIS 1
#define TOUCH_IDLE_AXIS 2

void touchscreen_init(void);

void touchscreen_10ms_interrupt(void);
void touchscreen_select_dim(uint8_t);
void touchscreen_20ms(void);
float touchscreen_read_x(void);
float touchscreen_read_y(void);

#endif	/* TOUCHSCREEN_H */

