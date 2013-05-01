#include <stdio.h>
#include "convolve.h"

#define TIME_SAMPLES 128
#define SKIP_FRONT 127
#define SKIP_END 254



void convolve(double *xOld, double *x, double *y, double *out) {
	/*
	* This version of Convolve returns a POINTER to the array
	* of size TIME_SAMPLES that contains the convolved data.
	*/

	int i;
	for(i = 0; i < TIME_SAMPLES; i++) {
		int j;
		for(j = 0; j < (TIME_SAMPLES*2); j++) {
			if ((!((i + j) < SKIP_FRONT)) && (!((i + j) > SKIP_END))) {
				if (j >= TIME_SAMPLES) {					
					out[i+j-SKIP_FRONT] = out[i+j-SKIP_FRONT] + x[j-TIME_SAMPLES]*y[i];
				} else {
					out[i+j-SKIP_FRONT] = out[i+j-SKIP_FRONT] + xOld[j]*y[i];
				}
			}
		}
	}
}

/* Uncomment this to test convolve()!
 * This function works.

void testConvolve() {
	int i;
	double testXOld[TIME_SAMPLES];
	double testX[TIME_SAMPLES];
	double testY[TIME_SAMPLES];
	for (i = 0; i < TIME_SAMPLES; i++) {
		testXOld[i] = (double)2;
		testX[i] = (double)1;
		testY[i] = (double)3;
	}
	double testOut[TIME_SAMPLES];
	convolve(testXOld, testX, testY, testOut);
	int j;	
	for (j = 0; j < TIME_SAMPLES; j++) {
		printf("Out[%d]: %f\n", j, testOut[j]);
	}
}

void main(void) {
	testConvolve();
}*/
