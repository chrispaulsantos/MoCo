#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "includes/ece486_biquad.h"

#define round(x) ((int)(x)+0.5)
#define N 1
#define eta 1.01010101

extern FlagStatus KeyPressed;   // Use to detect button presses

/** These paramters are the coefficients created with the function
 *  create_filter, the gain to normalize the gain to 1 (0dB),
 *  and the number of sections which is the number of coefficients
 *  divided by 5 (the number of coefficients per section)
 */

//The gain is distributed to the coefficients already
static float G = 1;

//Number of second order sections
static int sections = 3;

//a and b coefficients
static float coeffs[15] = {0.1569,-0.2494,0.1569,-1.808,0.9173, //Section: 1
                           0.07004,-0.0791,0.07004,-1.703,0.7641, //Section: 2
                           0.1449,0.1449,0,-0.8295,0}; //Section: 3

int main(void)
{
    char lcd_str[8];

    static float sign=1.0;
    /*
     * Local Variable Definitions
     */

    int flag = 1;
    int i;
    float V[N],C,F;
    int a,b;
    float *input = (float *)malloc(sizeof(float)*N);
    float *output = (float *)malloc(sizeof(float)*N);
    BIQUAD_T *bq = init_biquad(sections,G,coeffs,N);

    setblocksize(N);

    initialize(FS_2K, MONO_IN, MONO_OUT);       // Set up: ADC input, DAC output


    while(1){
        //DIGITAL_IO_SET(); 	// Use a scope on PD0 to measure execution time

        getblock(input);	// Wait here until the input buffer is filled...

        for(i = 0; i < N; i ++){
            V[i] = 1.5 + (input[i]*1.5);
        }

        calc_biquad(bq,V,output);

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


        C = (159.2826542*V[0]*eta) - 273.15;
        F = C*1.8 + 32;
        a = (int)(C + 0.5);
        b = (int)(F + 0.5);
        for(i = 0; i < 4000000000; i++){}
        BSP_LCD_GLASS_DisplayString( (uint8_t *)lcd_str);
        sprintf(lcd_str, "C%2dF%2d", a, b);
    }
    return 0;
}
