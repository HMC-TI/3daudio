/*
 * This calcualtes the gain from an old distance and a new distance
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<stdlib.h>
#include	"gain.h"
#define time_samples 128

/*
 * Calcuates the gain.
 *
 * Inputs:
 * rampup: array that goes from 0 to 1
 * oldDist: the distance from the previuos input
 * newDist: the distance for this input
 * gain: pointer to the gain vector that calculated gain will be placed in
 */
void gain(double *rampup, double oldDist, double newDist, double *gainVec) {
	printf("Entered gain()");
	//double calcGain[time_samples];
	double oldD, newD;
	//double calcGain[time_samples];
	
	// make sure that the old and new distances are not less than 1
	if (oldDist < 1) oldD = 1;
	else oldD = oldDist;
	if(newDist < 1) newD = 1;
	else newD = newDist;
	
	double distanceAdd = 1.0/pow(oldD, 2);
	double distanceChange = (1.0/pow(newD, 2)) - distanceAdd;
	
	/*printf("The new distance is: %f\n", newD);
	printf("The old distance is: %f\n", oldD);
	printf("The distAdd is: %f\n", distanceAdd);
	printf("The distChange is: %f\n", distanceChange);*/
	
	int i;
	for(i = 0; i < time_samples; i++) {
		gainVec[i] = rampup[i]*distanceChange + distanceAdd;
	}
	
	//gain = &calcGain;
}

/*
 * Tests the gain function
 * This function works.
 
void main(void) {
	double dist = 1;
	double distNew = 1;
	
	double *gainX = (double*) malloc(time_samples*sizeof(double));
	double *rampup = (double*) malloc(time_samples*sizeof(double));
	
	int i;
	for (i=0; i<time_samples; i++) {
		rampup[i] = i/(double)(time_samples-1);
	}
	
	// print rampup
	int j;
	for (j=0; j<time_samples; j++) {
		printf(" %f\n", rampup[j]);
	}
	
	printf("Now I'm printing the gain values \n");
	gain(rampup, dist, distNew, gainX);
	
	// print
	int k;
	for (k=0; k<time_samples; k++) {
		printf(" %f\n", gainX[k]);
	}
	
	return;
}*/
