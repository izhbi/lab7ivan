#include "calibrate.h"
#include "servos.h"
#include "touchscreen.h"
#include "mymaths.h"
#include <libpic30.h>

static uint16_t touchscreen_data[2][3];
static int16_t servos_data[2][2];


uint16_t* touchscreen_calibration_data(uint8_t axis) {
    return touchscreen_data[axis];
}

int16_t* servos_calibration_data(uint8_t axis) {
    return servos_data[axis];
}

void set_touchscreen_calibration(uint8_t axis, uint16_t v1, uint16_t v2) {
    touchscreen_data[axis][0] = v1;
    touchscreen_data[axis][1] = v2;
    touchscreen_data[axis][2] = (v1 + v2) / 2;
}

void set_servos_calibration(uint8_t axis, int16_t v1, int16_t v2) {
    servos_data[axis][0] = v1;
    servos_data[axis][1] = v2;
}

void calibrate_touchscreen(uint8_t calibration_axis) {
    uint8_t fixed_axis = (calibration_axis + 1) % 2;
    servo_set_duty(fixed_axis, SERVO_MIN);
    touchscreen_select_dim(calibration_axis);
    
    servo_set_duty(calibration_axis, SERVO_MIN);
    __delay_ms(3500);
    if (calibration_axis == TOUCH_X_AXIS)
        touchscreen_data[calibration_axis][0] = touchscreen_read_x();
    else
        touchscreen_data[calibration_axis][0] = touchscreen_read_y();
    
    servo_set_duty(calibration_axis, SERVO_MAX);
    __delay_ms(3500);
    if (calibration_axis == TOUCH_X_AXIS)
        touchscreen_data[calibration_axis][1] = touchscreen_read_x();
    else
        touchscreen_data[calibration_axis][1] = touchscreen_read_y();
    
    touchscreen_data[calibration_axis][2] = (touchscreen_data[calibration_axis][0] + touchscreen_data[calibration_axis][1]) / 2;
}

void calibrate_servos(uint8_t calibration_axis) {
    int16_t i, angle, original_read, new_read, new_abs, smallest_abs;
    uint8_t fixed_axis = (calibration_axis + 1) % 2;
    servo_set_duty(fixed_axis, SERVO_MIN);
    touchscreen_select_dim(calibration_axis);
    
    servo_set_duty(calibration_axis, SERVO_MIN);
    __delay_ms(3000);
    original_read = (int16_t) touchscreen_read();
    
    for (angle = SERVO_MIN + 10; angle <= SERVO_MAX; angle+=5) {
        servo_set_duty(calibration_axis, angle);
        __delay_ms(100);
        
        smallest_abs = 10000;
        for (i = 0; i < 10; i ++) {
            new_read = (int16_t) touchscreen_read();
            new_abs = abs(new_read - original_read);
            if (new_abs < smallest_abs)
                smallest_abs = new_abs;
        }
     
        if (smallest_abs > 1000) {
            break;
        }
    }
    
    servos_data[calibration_axis][0] = angle;
    
    servo_set_duty(calibration_axis, SERVO_MAX);
    __delay_ms(3000);
    original_read = (int16_t) touchscreen_read();
    
    for (angle = SERVO_MAX - 10; angle >= SERVO_MIN; angle-=5) {
        servo_set_duty(calibration_axis, angle);
        __delay_ms(100);

        smallest_abs = 10000;
        for (i = 0; i < 10; i ++) {
            new_read = (int16_t) touchscreen_read();
            new_abs = abs(new_read - original_read);
            if (new_abs < smallest_abs)
                smallest_abs = new_abs;
        }
     
        if (smallest_abs > 1000) {
            break;
        }
    }
    
    servos_data[calibration_axis][1] = angle;
}