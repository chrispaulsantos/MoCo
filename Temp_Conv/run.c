#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define round(x) ((int)(x)+0.5)
#define N 1
#define eta 0.989


extern FlagStatus KeyPressed;   // Use to detect button presses

int main(void)
{
    char lcd_str[8];
    int flag = 1;
    int i;
    float V,C,F,VRef[1];
    VRef[0] = -.45466666666;
    int a,b,c;
    float *input = (float *)malloc(sizeof(float)*N);

    setblocksize(N);

    initialize(FS_2K, MONO_IN, MONO_OUT);       // Set up: ADC input, DAC output

    while(1){
        //DIGITAL_IO_SET(); 	// Use a scope on PD0 to measure execution time

        getblock(input);	// Wait here until the input buffer is filled...

        if (KeyPressed) {
            if(c > 200){c = 0;}
            KeyPressed = RESET;
            flag = !flag;
            BSP_LED_Toggle(LED5);
            //VRef[0] = VRef[0] + 0.01;
            c++;
        }

        V = 1.5 + (input[0]*1.5);
        C = (159.2826542*V*eta) - 273.15;
        F = C*1.8 + 32;
        a = (int)(C + 0.5);
        b = (int)(F + 0.5);
        for(i = 0; i < 20000000; i++){}
        BSP_LCD_GLASS_DisplayString( (uint8_t *)lcd_str);
        sprintf(lcd_str, "C%2dF%2d", a, b);
        putblock(VRef);
    }
    return 0;
}
