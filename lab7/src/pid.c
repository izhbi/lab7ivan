#include "pid.h"
#include "servos.h"
#include "touchscreen.h"
#include "mymaths.h"
#include "calibrate.h"
#include "lcd.h"

static const float xKp = 0.0003, xKd = -0.0001, xKi = -0.001;// 0.0015, 0.00001, 0.001 for working for any one-dimension
static const float yKp = 0.0015, yKd = 0.0001, yKi = 0.001;

static volatile float x_errors[32];
static volatile float x_error_sum = 0;

static volatile float y_errors[32];
static volatile float y_error_sum = 0;

volatile int16_t servo_action[2]; 

static void do_pid_for_axis(uint16_t axis, float position, float target, 
        float* errors, int errorslen, float* error_sum,
        float Kp, float Kd, float Ki) {
    int i;
    
    uint16_t* cal_data = touchscreen_calibration_data(axis);
    position = clamp(position, (float) cal_data[0], (float) cal_data[1]);
    
    *error_sum = *error_sum - errors[errorslen - 1];
    for (i = errorslen - 1; i > 0; i--) {
        errors[i] = errors[i - 1];
    }
    errors[0] = position - target;
    *error_sum = *error_sum + errors[0];
    
    float action = Kp * errors[0] * 100 - Kd * ((errors[0] - errors[1]) / 0.02f) * 100 - Ki * (*error_sum) * 0.02 * 100;
    action = clamp(action, -100, 100);
    
    int16_t* servo_cal_data = servos_calibration_data(axis);
    float servo_min = (float) -servo_cal_data[0];
    float servo_max = (float) -servo_cal_data[1];
    
    servo_action[axis] = -(((action + 100) / 200) * (servo_max - servo_min) + servo_min);
    
    /*
    servo_set_duty(axis, (int16_t) real_action);
    
    reading = y[0];
    reading_raw = x[0];
    error = errors[0];
    */
}

void do_pid_calculation(uint8_t axis) {
    if (axis == TOUCH_X_AXIS) {
        do_pid_for_axis(TOUCH_X_AXIS, touchscreen_read_x(), (float) touchscreen_calibration_data(TOUCH_X_AXIS)[2], 
            x_errors, 32, &x_error_sum, xKp, xKd, xKi);
    } else if (axis == TOUCH_Y_AXIS) {
         do_pid_for_axis(TOUCH_Y_AXIS, touchscreen_read_y(), (float) touchscreen_calibration_data(TOUCH_Y_AXIS)[2], 
            x_errors, 32, &x_error_sum, yKp, yKd, yKi);
    }
}