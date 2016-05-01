/*!
 * @file
 *
 * @ece486_biquad main function file
 *
 * @author ECE486 Lab Group 02
 * @author Kyle Parry
 * @author Chris Santos
 * @author Dakota Carll
 *
 * @date Febuary 14, 2016
 *
 * Function used to impliment an FIR filter with convolution
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include "ece486_biquad.h"

 BIQUAD_T* init_biquad(int sections, float g, float *biquad_coefs, int blocksize)
 {
    int secDex=0,i=0,j=0,ct=0;
    // Check for errors in input cases which don't match expected inputs
    if (blocksize <= 0) {
        printf("Error, invalid block size \n");
        return 0;
    }
    if (sections <= 0 ) {
        printf("Error, invalid section size \n");
        return 0;
    }
    if (g == 0) {
        printf("Error, gain must not be zero \n");
        return 0;
    }

 // Allocate memory needed to store information and inputs data into Biquad struct
    BIQUAD_T* BQ = (BIQUAD_T*)malloc(sizeof(BIQUAD_T));
    BQ->sections = sections;
    BQ->blocksize = blocksize;
    BQ->g = g;
    BQ->a_coeffs = (float**)malloc(sections*sizeof(float*));
    BQ->b_coeffs = (float**)malloc(sections*sizeof(float*));
    BQ->circ_buff = (float**)malloc(sections*sizeof(float*));
    for(i=0;i<sections;i++) {
        BQ->circ_buff[i] =( float*)calloc(2,sizeof(float));
        BQ->b_coeffs[i] = (float*)calloc(3,sizeof(float));
        BQ->a_coeffs[i] = (float*)calloc(3,sizeof(float));
    }


 // Fill coeficients with input data
 // Take coefficent array and seperate into a and b coeficents using a two demensional array
 // to divide coefficents by sections

    for (i = 0; i < sections; i++) { //for i < size of biquad_coefs
        BQ->a_coeffs[secDex][0] = 1;//set a0 to 1 for all filters

        for (j = 0; j < 5; j++) { //for j < coeffs of one stage
            if(j < 3) {
                BQ->b_coeffs[secDex][j] = biquad_coefs[ct];
            }
            else {
                BQ->a_coeffs[secDex][j-2] = biquad_coefs[ct];
            }
            ct++;
        }
        secDex++; //increment section index
    }
    return BQ;
}

/*!
 * Run the input through the filter and output the data
 */
void calc_biquad(BIQUAD_T *s, float *x, float *y) {
    int i,n;

    for(i = 0; i < s->sections; i++){
        for(n = 0; n < s->blocksize; n++){
            s->circ_buff[i][0] = (x[n]) - (s->a_coeffs[i][1]*s->circ_buff[i][1]) - (s->a_coeffs[i][2]*s->circ_buff[i][2]);
            x[n] = (s->b_coeffs[i][0]*s->circ_buff[i][0]) + (s->b_coeffs[i][1]*s->circ_buff[i][1]) + (s->b_coeffs[i][2]*s->circ_buff[i][2]);

            s->circ_buff[i][2]=s->circ_buff[i][1];
            s->circ_buff[i][1]=s->circ_buff[i][0];
        }
    }
    for(n=0;n<s->blocksize;n++){
        y[n]=s->g*x[n];
    }
}
/*!
 * Deallocates memory in all buffers
 */
void destroy_biquad(BIQUAD_T *s) {
    int i;
    for(i=0;i<s->sections;i++){
        free(s->circ_buff[i]);
        free(s->a_coeffs[i]);
        free(s->b_coeffs[i]);
    }
    free(s->circ_buff);
    free(s->a_coeffs);
    free(s->b_coeffs);
    free(s->biquad_coefs);
    free(s);
    s = NULL;
    return;
}
