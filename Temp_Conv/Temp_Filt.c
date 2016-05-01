	/** These paramters are the coefficients created with the function
	 *  create_filter, the gain to normalize the gain to 1 (0dB),
	 *  and the number of sections which is the number of coefficients
	 *  divided by 5 (the number of coefficients per section)
	 */

	//The gain is distributed to the coefficients already
	float G = 1;

	//Number of second order sections
	int sections = 3;

	//a and b coefficients
	float coeffs[15] = {0.1569,-0.2494,0.1569,-1.808,0.9173, //Section: 1
						0.07004,-0.0791,0.07004,-1.703,0.7641, //Section: 2
						0.1449,0.1449,0,-0.8295,0}; //Section: 3
						
