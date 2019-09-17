/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "ms_isr.h"

#define ON 1
#define OFF 0
#define UPDATE_PERIOD 4 //ms
#define COMPARE_VAL_MIN 51
#define COMPARE_VAL_MAX 11949 

uint16 ms_count = 0;
uint16 comp_val = 50;
uint8 increment = 1;

CY_ISR(led_isr) {
    Led_Write(ON);
}

CY_ISR(ms_isr) {
    ms_count++;
    
    if(ms_count == UPDATE_PERIOD) {
     
        if(increment) {
            comp_val += 50; // increment compare value 1
        }else{
            comp_val -= 50; // decrement compare value 1
        }
         
        if(comp_val < COMPARE_VAL_MIN) { // make increment "true"
            increment = 1;
        }
        if(comp_val > COMPARE_VAL_MAX) { // make decrement "true"
            increment = 0;
        }
         
        ms_count = 0; // reset ms counter
    }
}

CY_ISR(tc_isr) {
    Led_Write(OFF);
}

int main(void)
{
    Timer_1_Start();
    PWM_1_Start();
    
    led_isr_StartEx(led_isr); // Point to BLUE_ISR to turn on the blue LED
    tc_isr_StartEx(tc_isr); // Point to TC_ISR to turn off both LEDs
    ms_isr_StartEx(ms_isr); // Point to MS_ISR to update the compare values
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    for(;;)
    {
        PWM_1_WriteCompare(comp_val); // Write new compare value to PWM output 1
    }
}

/* [] END OF FILE */
