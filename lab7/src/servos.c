#include "servos.h"
#include <p33Fxxxx.h>

void servos_init(void) {
    CLEARBIT(T2CONbits.TON); // Disable Timer
    CLEARBIT(T2CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T2CONbits.TGATE); // Disable Gated Timer mode
    TMR2 = 0x00; // Clear timer register
    T2CONbits.TCKPS = 0b10; // Select 1:64 Prescaler
    CLEARBIT(IFS0bits.T2IF); // Clear Timer2 interrupt status flag
    CLEARBIT(IEC0bits.T2IE); // Disable Timer2 interrupt enable control bit
    PR2 = 4000; // Set timer period 20ms:
    // 4000= 20*10^-3 * 12.8*10^6 * 1/64
    
    //setup OC8
    CLEARBIT(TRISDbits.TRISD7); /* Set OC8 as output */
    OC8R = 3700; /* Set the initial duty cycle to 1.5ms*/
    OC8RS = 3700; /* Load OCRS: next pwm duty cycle */
    OC8CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    
    //setup OC7
    CLEARBIT(TRISDbits.TRISD6); /* Set OC8 as output */
    OC7R = 3700; /* Set the initial duty cycle to 1.5ms*/
    OC7RS = 3700; /* Load OCRS: next pwm duty cycle */
    OC7CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    
    SETBIT(T2CONbits.TON); /* Turn Timer 2 on */
}

// duty: goes from -120 to 120
void servo_set_duty(uint8_t chan, int16_t duty) {
    // 3820 to 3580
    uint16_t duty_us = 3700 - duty;
    
    if (duty_us < 3580)
        duty_us = 3580;
    else if (duty_us > 3820)
        duty_us = 3820;
    
    if (chan == SERVO_X_CHAN) { // X-axis
        OC8RS = duty_us;
    } else if (chan == SERVO_Y_CHAN) { // Y-axis
        OC7RS = duty_us;
    }
}