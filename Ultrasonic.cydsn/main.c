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

#define ON 1
#define OFF 0
#define UPDATE_PERIOD 4 //ms
#define COMPARE_VAL_MIN 51
#define COMPARE_VAL_MAX 11949 

int timerToCm(uint16 timerCounter);

uint16 ms_count = 0;
uint16 comp_val = 50;
uint8 flip = 1;
uint16 echo;

CY_ISR(echo_interupt_isr) {
    echo = Timer_Echo_ReadCounter();
}

CY_ISR(led_isr) {
    //Led_Write(flip);
}

CY_ISR(ms_isr) {
    ms_count++;
}

CY_ISR(tc_isr) {
    //Led_Write(OFF);
}

int main(void)
{
    int cm;    
    char string[30];
    //Timer_Trigger_Start();
    Timer_Echo_Start();
    PWM_Trigger_Start();
    UART_Start();
    
    echo_interupt_isr_StartEx(echo_interupt_isr);
    
    //led_isr_StartEx(led_isr); // Point to BLUE_ISR to turn on the blue LED
    //tc_isr_StartEx(tc_isr); // Point to TC_ISR to turn off both LEDs
    //ms_isr_StartEx(ms_isr); // Point to MS_ISR to update the compare values
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    for(;;)
    {
        cm = timerToCm(echo);
        sprintf(string,"\n Echo: %d",cm);
        UART_PutString(string);
        CyDelay(1500);
    }
}

int timerToCm(uint16 timerCounter) {
    float us = 0.0416666667;
    float cm;
    cm = us*(65536-timerCounter)/58;
    
    return cm;
}

/* [] END OF FILE */
