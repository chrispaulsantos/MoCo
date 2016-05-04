#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define round(x) ((int)(x)+0.5)
#define N 1
#define eta 0.988

extern FlagStatus KeyPressed;   // Use to detect button presses

int main(void)
{
    char lcd_str[8];
    int flag = 1;
    int i,iF,iC;
    float V,C,F,VRef[1];
    VRef[0] = -0.45466666666;
    float *input = (float *)malloc(sizeof(float)*N);

    setblocksize(N);

    initialize(FS_2K, MONO_IN, MONO_OUT);       // Set up: ADC input, DAC output

    while(1){
        getblock(input);	// Wait here until the input buffer is filled...

        if (KeyPressed) {
            KeyPressed = RESET;
            flag = !flag;
            BSP_LED_Toggle(LED5);
        }
        // Calculate voltage value from normalized input
        V = 1.5 + (input[0]*1.5);

        // Calculate temperature in both F and C
        C = (159.2826542*V*eta) - 273.15;
        F = C*1.8 + 32;

        // Cast to int
        iC = (int)(C + 0.5);
        iF = (int)(F + 0.5);

        // Delay to display not so quickly
        for(i = 0; i < 20000000; i++){}

        // Display to LCD
        BSP_LCD_GLASS_DisplayString((uint8_t *)lcd_str);
        sprintf(lcd_str, "C%2dF%2d", iC, iF);

        // Output reference to the circuit
        putblock(VRef);
    }
    return 0;
}
