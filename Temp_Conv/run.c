#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define round(x) ((int)(x)+0.5)

extern FlagStatus KeyPressed;   // Use to detect button presses



int main(void)
{
    char lcd_str[8];

    static float sign=1.0;
    /*
     * Local Variable Definitions
     */

    int flag = 1;
    int i;
    float T,V,C,F;
    int a,b;
    float *input = (float *)malloc(sizeof(float)*1);
    float *stage1_input_re = (float *)malloc(sizeof(float)*1);

    setblocksize(1);

    initialize(FS_2K, STEREO_IN, STEREO_OUT);       // Set up: ADC input, DAC output


    while(1){

         //DIGITAL_IO_SET(); 	// Use a scope on PD0 to measure execution time

           getblockstereo(input,stage1_input_re);	// Wait here until the input buffer is filled...

           //putblockstereo(stage1_output_re,stage1_output_re);


        if (KeyPressed) {
            KeyPressed = RESET;
            flag = !flag;
            sign *= -1.0;		// Invert output1
            /*
             * On each press, modify the LCD display, and toggle an LED
             * (LED4=red, LED5=green) (Red is used to show error conditions)
             *
             * Don't be surprised when these cause a Sample Overrun error,
             * depending on your sample rate.
             */
            //button_count++;
            BSP_LED_Toggle(LED5);
        }

        V = 1.5 + (input[0]*1.5);
        C = (159.3448842*V) - 273.15;
        F = C*1.8 + 32;
        a = (int)(C + 0.5);
        b = (int)(F + 0.5);
        for(i = 0; i < 40000000; i++){}
        BSP_LCD_GLASS_DisplayString( (uint8_t *)lcd_str);
        sprintf(lcd_str, "C%2dF%2d", a, b);
    }
    return 0;
}
