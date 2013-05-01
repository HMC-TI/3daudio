#include "audio3d.h"
#include "getIRF.h"
#include "gain.h"
#include "convolve.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_SAMPLES 128
#define INBUF_TIME 44100*10

	// TODO: Create IRF_DATUM class, replace all 2D arrays
	// Perhaps we should rename the class to 2D array?
	static double inBuf_l[TIME_SAMPLES]; // Should these
	static double inBuf_r[TIME_SAMPLES]; // be static?
	static IRF_DATUM *irfBuf_r;
	static IRF_DATUM *irfBuf_l;


/*
	* We want these arrays to be static so that they don't
	* reset each time runAudio3D is called.
	*/
	static double oldInBuf_l[TIME_SAMPLES];
	static double oldInBuf_r[TIME_SAMPLES];
	static IRF_DATUM *oldIrf_l;
	static IRF_DATUM *oldIrf_r;
	static double newOut_l[TIME_SAMPLES];
	static double newOut_r[TIME_SAMPLES];
	static double *rampUp;
	static double *rampDn;
	static double *gainVec;

	static double oldDist = 1;
	static int oldAz = 0;
	static int oldElev = 0;
	static int cfFlag; // Booleans don't exist in C

	// What are the oldOuts used for??
	static double oldOut_l[TIME_SAMPLES];
	static double oldOut_r[TIME_SAMPLES];

/*	double irfBuf_ll[TIME_SAMPLES];
	double irfBuf_lr[TIME_SAMPLES];
	double irfBuf_rl[TIME_SAMPLES];
	double irfBuf_rr[TIME_SAMPLES];
	double oldIrf_ll[TIME_SAMPLES];
	double oldIrf_lr[TIME_SAMPLES];
	double oldIrf_rl[TIME_SAMPLES];
	double oldIrf_rr[TIME_SAMPLES];
*/

void runAudio3D(float *inputL, float *inputR, int az, double dist, int elev, int chan, double *outputL, double *outputR) {
	
	init();

	// Sample input
	//printf("chan is %d\n", chan);
	
	/****************************
	Why do we do this???
	I'm changing it so that it makes our code ok
	******************************/
	
	if (chan == 1) {
		int i;
		for (i = 0; i < TIME_SAMPLES; i++) {
			inBuf_l[i] = (double) inputL[i];
			inBuf_r[i] = (double) inputR[i];
		}
	} else {
		int i;
		for (i = 0; i < TIME_SAMPLES; i++) {
			inBuf_l[i] = (double) inputL[i];
		}
	}
	//printf("oldAz is %d and oldElev is %d\n", oldAz, oldElev);
	if ((oldAz != az) || (oldElev != elev))
		cfFlag = 1;
	else
		cfFlag = 0;
	//printf("cfFlag is %d\n", cfFlag);
	loadIRFs(chan, az, elev, irfBuf_l, irfBuf_r);
	//printf("Loaded IRFs:\n");
	/*int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		printf("irfBuf_l.left[%d]: %f\n", i, irfBuf_l->left[i]);
		printf("irfBuf_r.right[%d]: %f\n", i, irfBuf_r->right[i]);
	}*/
	// Unpack structs for use in createNewOutputs and
	// createOldConvolveAndCrossfade
	
	/*for (i = 0; i < TIME_SAMPLES; i++) {
		irfBuf_ll[i] = irfBuf_l->left[i];
		irfBuf_lr[i] = irfBuf_l->right[i];
		irfBuf_rl[i] = irfBuf_r->left[i];
		irfBuf_rr[i] = irfBuf_r->right[i];
		oldIrf_ll[i] = oldIrf_l->left[i];
		oldIrf_lr[i] = oldIrf_l->right[i];
		oldIrf_rl[i] = oldIrf_r->left[i];
		oldIrf_rr[i] = oldIrf_r->right[i];
	}
	irfBuf_ll = irfBuf_l->left;
	irfBuf_lr = irfBuf_l->right;
	irfBuf_rl = irfBuf_r->left;
	irfBuf_rr = irfBuf_*/


	//printf("Picked out appropriate IRFs\n");
	createNewOutputs(chan);
	//printf("Finished createNewOutputs()\n");

	if (cfFlag == 1) {
		createOldConvolveAndCrossfade(chan, oldOut_l, oldOut_r);
	}
	oldAz = az;
	oldElev = elev;
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		oldInBuf_l[i] = inBuf_l[i];
		oldInBuf_r[i]= inBuf_r[i];
/*		oldIrf_l->right[i] = irfBuf_l->right[i];
		oldIrf_l->left[i] = irfBuf_l->left[i];
		oldIrf_r->right[i] = irfBuf_r->right[i];
		oldIrf_r->left[i] = irfBuf_r->left[i];*/
	}
	oldIrf_l->right = irfBuf_l->right;
	oldIrf_l->left = irfBuf_l->left;
	oldIrf_r->right = irfBuf_r->right;
	oldIrf_r->left = irfBuf_r->left;

	//printf("Dealt with old stuff\n");
	/*oldInBuf_l = inBuf_l;
	oldInBuf_r = inBuf_r;
	oldIrf_l = irfBuf_l;
	oldIrf_r = irfBuf_r;*/
	// TODO: Make sure all 2D arrays are being passed
	// properly. For reference:
	// If you have a function void foo(int ** in),
	// you would pass in a 2D array in[2][128] as
	// int *in[128] = {in[0], in[1]};
	// foo(in);
	// Or is it int *in[2] = {in[0], ..., in[127]}?
	// Find out with array experiments.
	// Figure out if it's possible to return 2D arrays.

	/*assert(gainVec);
	assert(rampUp);
	assert(oldDist);
	assert(dist);
	printf("Passed gain asserts\n");
	printf("oldDist and dist are %f and %f\n", oldDist, dist);
	printf("rampUp[5] is %f\n", rampUp[5]);
	gain(rampUp, oldDist, dist, gainVec);
	printf("gainVec[5] is %f\n", gainVec[5]);
	printf("Now trying to create output\n");*/
	//IRF_DATUM finalOut;
	//finalOut.left = (double *) malloc(TIME_SAMPLES*sizeof(double));
	//finalOut.right = (double *) malloc(TIME_SAMPLES*sizeof(double));
	for (i = 0; i < TIME_SAMPLES; i++) {
		outputL[i] = newOut_l[i]*gainVec[i];
		outputR[i] = newOut_r[i]*gainVec[i];
	}
//	printf("Loaded output into outputL and outputR\n");
}

void init() {
//	printf("We've entered init()\n");
	// Allocate irf buffers
	oldIrf_l = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	oldIrf_r = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	irfBuf_l = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	irfBuf_r = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	/*oldIrf_l->left = (double*) malloc(TIME_SAMPLES*sizeof(double));
	oldIrf_l->right = (double*) malloc(TIME_SAMPLES*sizeof(double));
	oldIrf_r->left = (double*) malloc(TIME_SAMPLES*sizeof(double));
	oldIrf_r->right = (double*) malloc(TIME_SAMPLES*sizeof(double));
	irfBuf_l->left = (double*) malloc(TIME_SAMPLES*sizeof(double));
	irfBuf_l->right = (double*) malloc(TIME_SAMPLES*sizeof(double));
	irfBuf_r->left = (double*) malloc(TIME_SAMPLES*sizeof(double));
	irfBuf_r->right = (double*) malloc(TIME_SAMPLES*sizeof(double));*/
	gainVec = (double*) malloc(TIME_SAMPLES*sizeof(double));
	rampUp = (double*) malloc(TIME_SAMPLES*sizeof(double));
	rampDn = (double*) malloc(TIME_SAMPLES*sizeof(double));
//	printf("Should be done mallocing\n");
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		rampUp[i] = i/((double)(TIME_SAMPLES - 1));
		rampDn[i] = 1 - rampUp[i];
		// TODO: Determine whether we really need to
		// reset OldInBufs here
	}
	read_irfs();
}

void cleanup() {
	free(oldIrf_l);
	free(oldIrf_r);
	free(irfBuf_l);
	free(irfBuf_r);
	free(irfBuf_l->left);
	free(irfBuf_l->right);
	free(irfBuf_r->left);
	free(irfBuf_r->right);
	free(oldIrf_l->left);
	free(oldIrf_r->right);
	free(oldIrf_l->right);
	free(oldIrf_r->left);
	free_irfs();
}

void createNewOutputs(int chan) {
	
	if (chan == 1) {
		double out_rl[TIME_SAMPLES];
		double out_rr[TIME_SAMPLES];
		double out_ll[TIME_SAMPLES];
		double out_lr[TIME_SAMPLES];
		int i;
		for (i = 0; i < TIME_SAMPLES; i++) {
			out_ll[i] = 0;
			out_lr[i] = 0;
			out_rr[i] = 0;
			out_rl[i] = 0;
		}
	
		convolve(oldInBuf_l, inBuf_l, irfBuf_l->left, out_ll);
		convolve(oldInBuf_r, inBuf_r, irfBuf_r->right, out_rr);
		convolve(oldInBuf_r, inBuf_r, irfBuf_r->left, out_rl);
		convolve(oldInBuf_l, inBuf_l, irfBuf_l->right, out_lr);
	
		for (i = 0; i < TIME_SAMPLES; i++) {
			// newOut_l is coming out right, but t_r isn't....
			newOut_l[i] = out_ll[i] + out_rl[i];
			newOut_r[i] = out_lr[i] + out_rr[i];
			/*printf("newOut_l[%d]: %f\n", i, newOut_l[i]);
			printf("newOut_r[%d]: %f\n", i, newOut_r[i]);*/
		}
	} else {
		double out_ll[TIME_SAMPLES];
		double out_lr[TIME_SAMPLES];
		int i;
		for (i = 0; i < TIME_SAMPLES; i++) {
			out_ll[i] = 0;
			out_lr[i] = 0;
		}
		convolve(oldInBuf_l, inBuf_l, irfBuf_l->left, out_ll);
		convolve(oldInBuf_l, inBuf_l, irfBuf_l->right, out_lr);
	
		for (i = 0; i < TIME_SAMPLES; i++) {
			newOut_l[i] = out_ll[i];
			newOut_r[i] = out_lr[i];
			//printf("Loaded newOut_l[%d] and newOut_r[%d]: %f and %f respectively\n", i, i, newOut_l[i], newOut_r[i]);
		}
	}
	return;
}

void createOldConvolveAndCrossfade(int chan, double *oldOut_l, double *oldOut_r) {
	if (chan == 1) {
		double old_rl[TIME_SAMPLES];
		double old_rr[TIME_SAMPLES];
		double old_lr[TIME_SAMPLES];
		double old_ll[TIME_SAMPLES];
		int i;
		for (i = 0; i < TIME_SAMPLES; i++) {
			old_ll[i] = 0;
			old_lr[i] = 0;
			old_rr[i] = 0;
			old_rl[i] = 0;
		}
		convolve(oldInBuf_l, inBuf_l, oldIrf_l->right, old_rl);
		convolve(oldInBuf_r, inBuf_r, oldIrf_r->left, old_rr);
		convolve(oldInBuf_l, inBuf_l, oldIrf_l->left, old_lr);
		convolve(oldInBuf_r, inBuf_r, oldIrf_r->right, old_ll);

		for (i = 0; i < TIME_SAMPLES; i++) {
			oldOut_l[i] = old_ll[i] + old_lr[i];
			oldOut_r[i] = old_rl[i] + old_rr[i];
		} 
	} else {
		double old_ll[TIME_SAMPLES];
		convolve(oldInBuf_l, inBuf_l, oldIrf_l->left, old_ll);
		double old_lr[TIME_SAMPLES];
		convolve(oldInBuf_l, inBuf_l, oldIrf_l->right, old_lr);
		int i;
		for (i = 0; i < TIME_SAMPLES; i++) {
			oldOut_l[i] = old_ll[i];
			oldOut_r[i] = old_lr[i];
		}
	}
}	

void loadIRFs(int chan, int az, int elev, IRF_DATUM *irfBuf_l, IRF_DATUM *irfBuf_r) {
	/*double *ptr_ll = irfBuf_l->left;
	double *ptr_rl = irfBuf_r->left;
	double *ptr_lr = irfBuf_l->right;
	double *ptr_rr = irfBuf_r->right;*/
	/*double filt_ll[128];
	double filt_lr[128];
	double filt_rl[128];
	double filt_rr[128];
	printf("We've entered loadIRFs.\n");*/
	if (chan == 1) {
		int i;
		get_irf(elev, az + 90, &(irfBuf_l->left), &(irfBuf_l->right));
		get_irf(elev, az - 90, &(irfBuf_r->left), &(irfBuf_r->right));
		/*for (i = 0; i < TIME_SAMPLES; i++) {
			printf("irfBuf_l->left[%d]: %f\n", i, irfBuf_l->left[i]);
			printf("irfBuf_l->right[%d]: %f\n", i, irfBuf_l->right[i]);
			printf("irfBuf_r->left[%d]: %f\n", i, irfBuf_r->left[i]);
			printf("irfBuf_r->right[%d]: %f\n", i, irfBuf_r->right[i]);
		}*/
	} else {
		//TODO: Why doesn't it depend on elev/az in this case?

		get_irf(elev, az, &(irfBuf_l->left), &(irfBuf_l->right));
		//printf("Got filters:\n");
		int i;
		/*for (i = 0; i < TIME_SAMPLES; i++) {
			printf("irfBuf_l->left[%d]: %f\n", i, irfBuf_l->left[i]);
			printf("irfBuf_l->right[%d]: %f\n", i, irfBuf_l->right[i]);
		}*/
	}
}

void crossfade(double *newIn, double *oldIn, double *out) {
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		out[i] = newIn[i]*rampUp[i] + oldIn[i]*rampDn[i];
	}
	return;
}

void testInit() {
	// Checks rampUp.
	init();
	int i;
	int errflag = 0;
	printf("Starting testInit!()\n");
	for (i = 0; i < TIME_SAMPLES; i++) {
		double curr = (double)i/(TIME_SAMPLES-1);
		if (rampUp[i] != curr) {
			printf("Incorrect result in rampUp[%d]: read %f, expected  %f \n", i, rampUp[i], curr);
			errflag = 1;
		} else {
			printf("rampUp[%d] is %f as expected\n", i, rampUp[i]);
		}
	}
	if (errflag == 0) {
		printf("Passed testInit() with no errors.\n");
	}
}

void testRunAudio3DMono() {
	double input[TIME_SAMPLES];
	double *output_l = (double *) malloc(TIME_SAMPLES*sizeof(double));
	double *output_r = (double *) malloc(TIME_SAMPLES*sizeof(double));
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		input[i] = 1;
		printf("input[%d]: %f\n", i, input[i]);
	}
	int chan = 0;
	double dist = 1;
	int elev = 0;
	int az = 0;
	runAudio3D(input, input, az, dist, elev, chan, output_l, output_r);
	printf("Successfully ran runAudio3D\n");
	for (i = 0; i < TIME_SAMPLES; i++) {
		printf("output_l[%d]: %f\n", i, output_l[i]);
	}
	for (i = 0; i < TIME_SAMPLES; i++) {
		printf("output_r[%d]: %f\n", i, output_r[i]);
	}
	free_irfs();
}

void testRunAudio3DStereo() {
	double inputL[TIME_SAMPLES];
	double inputR[TIME_SAMPLES];
	double *output_l = (double *) malloc(TIME_SAMPLES*sizeof(double));
	double *output_r = (double *) malloc(TIME_SAMPLES*sizeof(double));
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		inputL[i] = 1;
		inputR[i] = 2;
		printf("inputL[%d]: %f\n", i, inputL[i]);
		printf("inputR[%d]: %f\n", i, inputR[i]);
	}
	int chan = 1;
	double dist = 1;
	int elev = 0;
	int az = 0;
	runAudio3D(inputL, inputR, az, dist, elev, chan, output_l, output_r);
	printf("Successfully ran runAudio3D\n");
	for (i = 0; i < TIME_SAMPLES; i++) {
		printf("output_l[%d]: %f\n", i, output_l[i]);
	}
	for (i = 0; i < TIME_SAMPLES; i++) {
		printf("output_r[%d]: %f\n", i, output_r[i]);
	}
	free_irfs();

}

void run3DAudio(void) {
	double *input = (double*) malloc(INBUF_TIME*sizeof(double));
	
	double *output_l = (double *) malloc(TIME_SAMPLES*sizeof(double));
	double *output_r = (double *) malloc(TIME_SAMPLES*sizeof(double));
	
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		input[i] = sin(440*2*math.PI*i/(double)44100);
		//printf("input[%d]: %f\n", i, input[i]);
	}
	int chan = 0;
	double dist = 1;
	int elev = 0;
	int az = 0;
	
	int runtime = INBUF_TIME/TIME_SAMPLES;
	int i;
	for(i=0; i < runtime; i++) {
		runAudio3D(input + i*TIME_SAMPLES, input, az, dist, elev, chan, output_l, output_r);
		playAudio();
	}
}

int main(void) {
	//testInit();
	//testRunAudio3DMono();
	//testRunAudio3DStereo();
	return 1;
}
