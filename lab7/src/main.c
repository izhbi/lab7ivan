/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: simple HelloWorld application     */
/*                for Amazing Ball platform         */
/*                                                  */
/****************************************************/

#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#include <stdio.h>
#include <libpic30.h>
#include <uart.h>
#include <math.h>

#include "lcd.h"
#include "mytimer.h"
#include "pid.h"
#include "servos.h"
#include "touchscreen.h"
#include "types.h"
#include "calibrate.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);

volatile uint8_t touchscreen_callback = FALSE;
volatile uint8_t pid_callback = FALSE;
volatile uint8_t pid_calculation_ready = FALSE;

volatile uint8_t touchscreen_callback_timer = 0;
volatile uint8_t pid_callback_timer = 0;

void __attribute__((__interrupt__)) _T1Interrupt(void) { 
    if (touchscreen_callback) {
        touchscreen_20ms();

        touchscreen_callback_timer = (touchscreen_callback_timer + 1) % 2;
        touchscreen_select_dim(touchscreen_callback_timer);

        if (pid_callback) {
            do_pid_calculation((touchscreen_callback_timer + 1) % 2);

            if (touchscreen_callback_timer = 0) {
                pid_calculation_ready = true;
            }
        }
    }

    IFS0bits.T1IF = 0; // clear the interrupt flag
}

int main() {
    /* LCD Initialization Sequence */
    __C30_UART=1;
    lcd_initialize();
    lcd_clear();
    
    /* Initialize and start timer*/
    start_timer_32hz(PERIOD_10MS);

    /* Initialize touchscreen */
    touchscreen_init();
    touchscreen_callback = TRUE;

    /* Initialize servos */
    servos_init();
    
    set_servos_calibration(TOUCH_X_AXIS, 120, -120);
    set_servos_calibration(TOUCH_Y_AXIS, 120, -120);

    calibrate_touchscreen(TOUCH_X_AXIS);
    calibrate_touchscreen(TOUCH_Y_AXIS);
    
    //set_touchscreen_calibration(TOUCH_X_AXIS, 278, 2850);
    
    //calibrate_servos(TOUCH_X_AXIS);
    //calibrate_servos(TOUCH_Y_AXIS);
    
    uint16_t* x_touch_cal_data = touchscreen_calibration_data(TOUCH_X_AXIS);
    uint16_t* y_touch_cal_data = touchscreen_calibration_data(TOUCH_Y_AXIS);
    
    pid_callback = TRUE;
    
    //set_servos_calibration(TOUCH_X_AXIS, 120, -120);
    //set_servos_calibration(TOUCH_X_AXIS, 90, 15);
    //set_servos_calibration(TOUCH_Y_AXIS, 30, -5);
    
    //int16_t* x_servo_cal_data = servos_calibration_data(TOUCH_X_AXIS);
    //int16_t* y_servo_cal_data = servos_calibration_data(TOUCH_Y_AXIS);
    
    //touchscreen_select_dim(TOUCH_X_AXIS);
    
    int i = 0;
    extern int16_t servo_action[2];
    while(1){
        if (pid_calculation_ready) {
            servo_set_duty(TOUCH_X_AXIS, servo_action[0]);
            servo_set_duty(TOUCH_Y_AXIS, servo_action[1]);

            pid_calculation_ready = FALSE;

            if (++i == 5) {
                lcd_clear();
                lcd_locate(0, 0);
                lcd_printf("x-range: %d %d", x_touch_cal_data[0], x_touch_cal_data[1]);
                lcd_locate(0, 1);
                lcd_printf("y-range: %d %d", y_touch_cal_data[0], y_touch_cal_data[1]);
                lcd_locate(0, 2);
                lcd_printf("readings: %f %f", touchscreen_read_x(), touchscreen_read_y());
                lcd_locate(0, 3);
                lcd_printf("actions: %d %d", servo_action[0], servo_action[1]);
                i = 0;
            } 
        }

        //if (++i == 20000) {
            //lcd_locate(0, 2);
            //lcd_printf("Servos range:");
            //lcd_locate(0, 3);
            //lcd_printf("X: %d %d", x_servo_cal_data[0], x_servo_cal_data[1]);
            //lcd_locate(0, 4);
            //lcd_printf("Y: %d %d", y_servo_cal_data[0], y_servo_cal_data[1]);
            
            /*
            extern float action, real_action, error, reading, reading_raw;
            lcd_locate(0, 5);
            lcd_printf("%f %f", reading, reading_raw);
            lcd_locate(0, 6);
            lcd_printf("%f", action);
            lcd_locate(0, 7);
            lcd_printf("%f", real_action);
            */
            /*
            lcd_clear();
            lcd_locate(0, 0);
            lcd_printf("cal: %d, %d, %d", touch_min, touch_max, touch_mid); //return sample
            lcd_locate(0, 1);        
            lcd_printf("reading: %f", x_filtered_readings[0]);
            lcd_locate(0, 2);        
            lcd_printf("error: %f", errors[0]);
            lcd_locate(0, 3);        
            lcd_printf("error_sum: %f", error_sum);
            lcd_locate(0, 4);        
            lcd_printf("duty: %f", duty);
             */
        //}
    }
    return 0;
}
