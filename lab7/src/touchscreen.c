#include "touchscreen.h"
#include <p33Fxxxx.h>

static volatile float readings[2][3], filtered_readings[2][3];
static volatile uint8_t cur_dimension;

static const float coefs_a[3] = {1, -1.143, 0.4128};
static const float coefs_b[3] = {0.0675, 0.1349, 0.0675};

void touchscreen_init(void) {
        int i = 0;
        //disable ADC
        CLEARBIT(AD1CON1bits.ADON);
        //initialize PIN
        SETBIT(TRISBbits.TRISB15); //set TRISB RB15 to input
        SETBIT(TRISBbits.TRISB9); //set TRISB RB9 to input
        CLEARBIT(AD1PCFGLbits.PCFG15); //set AD1 AN15 input pin as analog
        CLEARBIT(AD1PCFGLbits.PCFG9); //set AD1 AN9 input pin as analog
        //Configure AD1CON1
        SETBIT(AD1CON1bits.AD12B); //set 12b Operation Mode
        AD1CON1bits.FORM = 0; //set integer output
        AD1CON1bits.SSRC = 0x7; //set automatic conversion
        //Configure AD1CON2
        AD1CON2 = 0; //not using scanning sampling
        //Configure AD1CON3
        CLEARBIT(AD1CON3bits.ADRC); //internal clock source
        AD1CON3bits.SAMC = 0x1F; //sample-to-conversion clock = 31Tad
        AD1CON3bits.ADCS = 0x2; //Tad = 3Tcy (Time cycles)
        //Leave AD1CON4 at its default value

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                readings[i][j] = 0;
                filtered_readings[i][j] = 0;
            }
        }

        //enable ADC
        SETBIT(AD1CON1bits.ADON);

        touchscreen_select_dim(TOUCH_X_AXIS);
}

void touchscreen_select_dim(uint8_t dimension) {
    cur_dimension = dimension;
    //set up the I/O pins E1, E2, E3 to be output pins
    CLEARBIT(TRISEbits.TRISE1); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE2); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE3); //I/O pin set to output
    if (dimension == TOUCH_X_AXIS) { // X
        CLEARBIT(LATEbits.LATE1);
        SETBIT(LATEbits.LATE2);
        SETBIT(LATEbits.LATE3);
        AD1CHS0bits.CH0SA = 0x0F;
    } else if (dimension == TOUCH_Y_AXIS) { // Y
        SETBIT(LATEbits.LATE1);
        CLEARBIT(LATEbits.LATE2);
        CLEARBIT(LATEbits.LATE3);   
        AD1CHS0bits.CH0SA = 0x09;
    } else { // stand-by
        SETBIT(LATEbits.LATE1);
        SETBIT(LATEbits.LATE2);
        CLEARBIT(LATEbits.LATE3);
    }
}

float touchscreen_read_x(void) {
    return filtered_readings[TOUCH_X_AXIS][0];
}

float touchscreen_read_y(void) {
    return filtered_readings[TOUCH_Y_AXIS][0];
}

void touchscreen_20ms(void) {
    SETBIT(AD1CON1bits.SAMP); //start to sample
    while(!AD1CON1bits.DONE); //wait for conversion to finish
    CLEARBIT(AD1CON1bits.DONE); //MUST HAVE! clear conversion done bit
    uint16_t sample = ADC1BUF0; //return sample

    readings[cur_dimension][2] = readings[cur_dimension][1];
    readings[cur_dimension][1] = readings[cur_dimension][0];
    readings[cur_dimension][0] = (float) sample;

    filtered_readings[cur_dimension][0] = (coefs_b[0] * readings[cur_dimension][0])
        + (coefs_b[1] * readings[cur_dimension][1]) + (coefs_b[2] * readings[cur_dimension][2])
        - ((coefs_a[1] * filtered_readings[cur_dimension][1]) + (coefs_a[2] * filtered_readings[cur_dimension][2]));
    
    filtered_readings[cur_dimension][2] = filtered_readings[cur_dimension][1];
    filtered_readings[cur_dimension][1] = filtered_readings[cur_dimension][0];

    return filtered_readings[cur_dimension][0];
}
