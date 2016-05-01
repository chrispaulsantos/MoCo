/*!
 * @file
 *
 * @biquad main function file
 *
 * @author ECE486 Lab Group 02
 * @author Chris Santos
 * @author Kyle Parry
 * @author Dakota Carl
 *
 * @date Febuary 18, 2016
 *
 */
typedef struct {
  int sections;
  float g;
  float *biquad_coefs;
  int blocksize;
  float **circ_buff;
  float **a_coeffs;
  float **b_coeffs;
}BIQUAD_T;
/**
 * [creates a pointer to a struct used for calc_biquad]
 * @param  sections     [sections is the number of second order filter sections]
 * @param  g            [g gives the scale factor G]
 * @param  biquad_coefs [the array biquad_coefs contains the IIR filter coefficients in the following format:
 *                                  {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ... as2}]
 * @param  blocksize    [size of block]
 * @return              [returns pointer to the allocated struct]
 */
BIQUAD_T* init_biquad(int sections, float g, float *biquad_coefs, int blocksize);
/**
 * [calculates the response to the filter with the given coefficients]
 * @param s [pointer to the returned struct from init_biquad]
 * @param x [input block]
 * @param y [output block]
 */
void calc_biquad(BIQUAD_T *s, float *x, float *y);
/**
 * [Deallocates memmory from BIQUAD_T]
 * @param s [The pointer to the struct]
 */
void destroy_biquad(BIQUAD_T *s);
