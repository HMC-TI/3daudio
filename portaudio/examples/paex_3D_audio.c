/** @file paex_sine.c
	@ingroup examples_src
	@brief Play a sine wave for several seconds.
	@author Ross Bencina <rossb@audiomulch.com>
    @author Phil Burk <philburk@softsynth.com>
*/
/*
 * $Id: paex_sine.c 1752 2011-09-08 03:21:55Z philburk $
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com/
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however, 
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included along with the 
 * license above.
 */
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "portaudio.h"
#include "getIRF.h"
#include "audio3d.h"
#include "gain.h"
#include "convolve.h"



#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (128)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)
typedef struct
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    int azim;
    char message[20];
}
paTestData;

/*
 * An IRF is stored as left and right spectra for easy convolution.
 * The compact IRFs are 128 time samples, which give us 128 complex
 * spectral points (which are conjugate symmetric). Depending on the
 * type of fft used, we will either have 128 complex points (fft.h)
 * or 65 complex points (sgi_fft.h).
 */
/*typedef struct {
	double *left;
	double *right;
} IRF_DATUM; */

/****************************************************************************
									THIS IS ALL FROM 3D AUDIO
****************************************************************************/

#define TIME_SAMPLES 128
#define time_samples 128
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
	
	//init();

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
	//printf("az is %d and elev is %d\n", az, elev);
	if ((oldAz != az) || (oldElev != elev))
		cfFlag = 1;
	else
		cfFlag = 0;
	//printf("cfFlag is %d\n", cfFlag);
	loadIRFs(chan, az, elev, irfBuf_l, irfBuf_r);
	
	//printf("loaded irfs");

	//printf("Picked out appropriate IRFs\n");
	createNewOutputs(chan);
	//printf("Finished createNewOutputs()\n");
	
	// calculate gain
	gain(rampUp, oldDist, dist, gainVec);

	if (cfFlag == 1) {
		createOldConvolveAndCrossfade(chan, oldOut_l, oldOut_r);
	}
	oldAz = az;
	oldElev = elev;
	oldDist = dist;
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		oldInBuf_l[i] = inBuf_l[i];
		oldInBuf_r[i]= inBuf_r[i];
	}
	
	oldIrf_l->right = irfBuf_l->right;
	oldIrf_l->left = irfBuf_l->left;
	oldIrf_r->right = irfBuf_r->right;
	oldIrf_r->left = irfBuf_r->left;

	// where did calc gain go?
	for (i = 0; i < TIME_SAMPLES; i++) {
		outputL[i] = newOut_l[i]*gainVec[i];
		outputR[i] = newOut_r[i]*gainVec[i];
	}
	
	
//	printf("Loaded output into outputL and outputR\n");
}

void init(int az, double dist, int elev) {
	//printf("We've entered init()\n");
	// Allocate irf buffers
	oldIrf_l = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	oldIrf_r = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	irfBuf_l = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	irfBuf_r = (IRF_DATUM*) malloc(sizeof(IRF_DATUM));
	gainVec = (double*) malloc(TIME_SAMPLES*sizeof(double));
	rampUp = (double*) malloc(TIME_SAMPLES*sizeof(double));
	rampDn = (double*) malloc(TIME_SAMPLES*sizeof(double));
	//printf("Should be done mallocing\n");
	int i;
	for (i = 0; i < TIME_SAMPLES; i++) {
		rampUp[i] = i/((double)(TIME_SAMPLES - 1));
		rampDn[i] = 1 - rampUp[i];
		// TODO: Determine whether we really need to
		// reset OldInBufs here
	}
	// Initialize old values to input values
	oldDist = dist;
	oldAz = az;
	oldElev = elev;	
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
	double filt_rr[128];*/
	//printf("We've entered loadIRFs.\n");
	if (chan == 1) {
		//int i;
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
		//int i;
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

/****************************************************************************
									THIS IS ALL FROM GAIN
****************************************************************************/

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
	//printf("Entered gain()");
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


/****************************************************************************
									THIS IS ALL FROM READ IRF
****************************************************************************/

/*
 * Default root path, can be overridden by setting environment
 * variable IRFROOT.
 */
#define ROOT_ENV	"IRFROOT"
#define ROOT_PATH	"/Users/carda/Documents/portaudio/irfs/"
#define PATH_FMT	"%s/elev%d/H%de%03da%s"
#define EXTENSION	".txt"
#define IRF_LEN	256
#define FILTER_LEN 128

#define TRUE 1
#define FALSE 0

char rootpath[256];



/*
 * A two-dimensional array of IRF_DATA, stored by elevation
 * and azimuth. Use get_indices() to get the indices for this
 * array.
 */
IRF_DATUM **irf_data;

#define MIN_ELEV	-40
#define MAX_ELEV	90
#define ELEV_INC	10
#define N_ELEV		(((MAX_ELEV - MIN_ELEV) / ELEV_INC) + 1)

#define MIN_AZIM	-180
#define MAX_AZIM	180

/*
 * This array gives the total number of azimuths measured
 * per elevation, and hence the AZIMUTH INCREMENT. Note that
 * only azimuths up to and including 180 degrees actually
 * exist in file system (because the data is symmetrical.
 */
int elev_data[N_ELEV] = {
56, 60, 72, 72, 72, 72, 72, 60, 56, 45, 36, 24, 12, 1 };


/*
 * Workspace for SGI FFT library. Used by irf.c and procbuf.c.
 */
float *workspace;


/**************************************************************************
 * Functions
 */


/*static int round(double val)
{
	return (val > 0) ? val + 0.5 : val - 0.5;
}*/

// Think this should be a double not long
static void bufclr(s,n)
char *s;
long n;
{
	while (n-- > 0) *s++ = 0;
}

/* 
 * Still not sure what this does, but apparently I need it
 */
char *ralloc(size)
long size;
{
char *ptr;
	if ((ptr = malloc(size)) == NULL) {
		fprintf(stderr,"can't malloc %ld bytes\n",size);
		exit(0);
	}
	bufclr(ptr,size);
	return ptr;
}

/*
 * Return the number of azimuths actually stored in file system.
 * Stands for number fo phases
 */
int get_nfaz(int el_index)
{
	return (elev_data[el_index] / 2) + 1;
}

/*
 * Get (closest) elevation index for given elevation.
 * Elevation is clipped to legal range.
 */
int get_el_index(double elev)
{
	int el_index;
	el_index = round((elev - MIN_ELEV) / ELEV_INC);
	if (el_index < 0) el_index = 0;
	else if (el_index >= N_ELEV) el_index = N_ELEV - 1;
	return el_index;
}

/*
 * For a given elevation and azimuth in degrees, return the
 * indices for the proper IRF. *p_flip will be set TRUE if
 * left and right channels need to be flipped.
 *
 * *p_el is the pointer to the elevation index
 * *p_az is the pointer to the azimuth index 
 */
void get_indices(double elev, double azim, int *p_el, int *p_az, int *p_flip)
{
	int naz, nfaz;
	int el_index;
	int az_index;
	el_index = get_el_index(elev);
	naz = elev_data[el_index];
	nfaz = get_nfaz(el_index);
	/*
	 * Coerce azimuth into legal range and calculate
	 * flip if any.
	 */
	azim = fmod(azim, 360.0);
	if (azim < 0) azim += 360;
	if (azim > 180) {
		azim = 360 - azim;
		*p_flip = TRUE;
	}
	else *p_flip = FALSE;

	/*
	 * Now 0 <= azim <= 180. Calculate index and clip to
	 * legal range just to be sure.
	 */
	//double temp = 
	az_index = round(azim / (360.0 / (double)naz));
	if (az_index < 0) az_index = 0;
	else if (az_index >= nfaz) az_index = nfaz - 1;

	*p_el = el_index;
	*p_az = az_index;
}

/*
 * Convert indices to angles.
 */
int index_to_elev(int el_index)
{
	return MIN_ELEV + el_index * ELEV_INC;
}

int index_to_azim(int el_index, int az_index)
{
	return round(az_index * 360.0 / elev_data[el_index]);
}

/*
 * Return pathname of IRF specified by indices.
 */
char *irf_name(int el_index, int az_index)
{
	static char buf[128];
	int elev;
	int azim;

	elev = index_to_elev(el_index);
	azim = index_to_azim(el_index, az_index);

	sprintf(buf,PATH_FMT,
		ROOT_PATH,elev,elev,azim,EXTENSION);
	return buf;
}


/*
 * Compute spectrum of short array as needed by convolution alg.
 * Filter response is copied into FILT_LEN size buffer, PRE-padded
 * with FILT_LEN zeros, and transformed. The resulting size of
 * returned spectrum depends on which fft package we're using.
 */
/*complex *xform_chan(short *sx, long n)
{
long i;
float *x;

	x = (float *) c_alloc(BUF_LEN / 2 + 1);
	for (i = 0; i < n; i++)
		x[FILT_LEN + i] = (double) sx[i] / 32768;
	sfft1du(-1, BUF_LEN, x, 1, workspace);

	return (complex *) x;
} */


/*
 * Read and transform the IRF specified by the indices.
 * Store into pre-allocated hd. We always store into
 * 128-pt result, even if IRF data is smaller.
 * 
 * This function works!!!
 */
void read_irf(int el_index, int az_index, IRF_DATUM *hd)
{
	char *name;
	double *sl = malloc(FILTER_LEN*sizeof(double));
	double *sr = malloc(FILTER_LEN*sizeof(double));

	name = irf_name(el_index, az_index);
	
	// opens the file
	FILE* f = fopen(name, "r");
	double n = 0;
	int i = 0;
	
	// make sure the file exists
	if (f == NULL) {
		printf("Cannot open file \n");
		exit(0);
	}
	
	// reads the file and puts the values in sl or sr depending on where in the file
	// it is
	while(fscanf(f, "%lf\n", &n) > 0) {
		if(i<FILTER_LEN) {
			sl[i++] = n;
		}
		else if (i >= FILTER_LEN*2) {
			printf("unexpected length of IRF \n");
			break;
		}
		else {
			sr[i++ - FILTER_LEN] = n;
		}
	}
	// close the file
	fclose(f);
	
	hd->left = sl;
	hd->right = sr;
	
	
	/*// Testing...
	printf("The left one is");
	int j;
	for (j=0; j<FILTER_LEN; j++) {
		printf(" %f\n", hd->left[j]);
	}*/
	
	// This is what is causing problems. Why is this?
	//free(sl);
	//free(sr);
	
}

/*
 * Read and transform all the IRFs.
 */
void read_irfs()
{
	int el_index;
	int az_index;
	int nfaz;
	char *root;
	
	
	root = getenv(ROOT_ENV);
	strcpy(rootpath, root ? root : ROOT_PATH);
	printf("Loading IRFs from '%s'\n",rootpath);
	
	// Initiate the array containing the IRF data
	// Have to change ralloc to something else. Not entirely sure what it does
	irf_data = (IRF_DATUM **) malloc(N_ELEV * sizeof(IRF_DATUM *));
	
	// goes through all the elevations
	for (el_index = 0; el_index < N_ELEV; el_index++) {
		printf("elevation %d...\n", index_to_elev(el_index));
		nfaz = get_nfaz(el_index);
		irf_data[el_index] = 
			(IRF_DATUM *) malloc(nfaz * sizeof(IRF_DATUM));
		// goes through all of the azimuths
		for (az_index = 0; az_index < nfaz; az_index++) {
			read_irf(el_index, az_index, &irf_data[el_index][az_index]);
		}
	}
	
	/* Why does this not have what I put in it????
	IRF_DATUM *hd = &irf_data[4][0];
	double *sl = hd->left;
	printf("The left one is");
	int j;
	for (j=0; j<FILTER_LEN; j++) {
		printf(" %f\n", sl[j]);
	}*/
}
/*
 * clean up the buffers when the system is done
 */
void free_irfs()
{
	int el_index, az_index;
	int nfaz;
	
	for (el_index = 0; el_index < N_ELEV; el_index++) {
		nfaz = get_nfaz(el_index);
		for (az_index = 0; az_index < nfaz; az_index++) {
			free(irf_data[el_index][az_index].left);
			free(irf_data[el_index][az_index].right);
		}
		free(irf_data[el_index]);
	}
	free(irf_data);
}

/*
 * Current and last values for position. Need these to print
 * position values only when things change. Pretty gross.
 */
int cur_el_index;
int cur_az_index;
int cur_flip_flag;
int last_el_index;
int last_az_index;
int last_flip_flag;
double cur_atten;
double last_atten;
int changed;

/*
 * Get the closest IRF to the specified elevation and azimuth
 * in degrees. Return via left and right channel pointers.
 
 * The **pointer means pointer to the address of a pointer
 *  *pointer gives the values of the pointer it's pointing to
 *   pointer gives the value of the pointer's object
 */
void get_irf(double elev, double azim, double **p_left, double **p_right)
{
	IRF_DATUM *hd;
	/*
	 * Clip angles and convert to indices.
	 */
	get_indices(elev, azim, &cur_el_index, &cur_az_index, &cur_flip_flag);
	
	// see if a change has occured
	if (cur_el_index != last_el_index ||
		cur_az_index != last_az_index ||
		cur_flip_flag != last_flip_flag)
			changed = TRUE;
	else changed = FALSE;
			
	// set the new last vales
	last_el_index = cur_el_index;
	last_az_index = cur_az_index;
	last_flip_flag = cur_flip_flag;
	
	//printf("The el index is %d\n", cur_el_index);
	//printf("The az index is %d\n", cur_az_index);

	/*
	 * Get data and flip channels if necessary.
	 */
	hd = &irf_data[cur_el_index][cur_az_index];
	
	//printf("Declared hd");
	
	if (cur_flip_flag) {
		*p_left = hd->right;
		*p_right = hd->left;
	}
	else {
		*p_left = hd->left;
		*p_right = hd->right;
	}
	
	//printf("Successfully finished get_irf");
}



/****************************************************************************
									THIS IS ALL FROM CONVOLVE
****************************************************************************/

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

/****************************************************************************
									THIS IS ALL FROM PORT AUDIO
****************************************************************************/


/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData; 
    float *out = (float*)outputBuffer;
    double leftout[FRAMES_PER_BUFFER];
    double rightout[FRAMES_PER_BUFFER];
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        out[i] = data->sine[data->left_phase];  /* left */
        //*out++ = data->sine[data->right_phase];  /* right */
        data->left_phase += 1;
        if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
        //data->right_phase += 3; /* higher pitch so we can distinguish left and right. */
        //if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
        
    }
    
    data->azim += 1;
    
    /*****************************
    Add in our function here
    
    Looks like we are going to need to do a couple of things:
    1. float to double
    	- looks like there is no double format for this code so we are going to
    	need to change our code to use floats
    	- ok I think this is done
    2. deconstruct the l,r,l,r,l,r list
      - putting the input in as a deconstructed list and then changing the output
      to be of the appropriate style
    3. make sure there are not mallocs in a normal run of our code
    4. must add this to my make file
    
    ******************************/
    int az_pa = data->azim;
    double dist_pa = 1;
    int elev_pa = 10;
    int chan_pa = 0;
    
    //printf("The azimuth is %d\n", az_pa);
    
    runAudio3D(out, out, az_pa, dist_pa, elev_pa, chan_pa, leftout, rightout);
    
    // so the in buffer is working because that's non-zero, but the output is not getting
    // assigned appropriately
    //printf("The leftout is %f\n", leftout[0]);
    
    unsigned long j;
    for( j=0; j<framesPerBuffer; j++ )
    {
    	*out++ = (float) leftout[j];
    	*out++ = (float) rightout[j];
    }
    
    return paContinue;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( void* userData )
{
   paTestData *data = (paTestData *) userData;
   printf( "Stream Completed: %s\n", data->message );
}

/*******************************************************************/
int main(void);
int main(void)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;
    int i;

    /*************
    Init 3D Audio
    **************/
    init(0,0,0);
    
    /* I don't know why I need to do this, but apparently something in here needs
    to be run at least once with 0,0,0 as it's first coordinate */
    double leftout[FRAMES_PER_BUFFER];
    double rightout[FRAMES_PER_BUFFER];
    float out[FRAMES_PER_BUFFER];
    int j;
    for (j=0; j<FRAMES_PER_BUFFER; j++) {
    	out[j] = 0;
    }
    runAudio3D(out, out, 0, 0, 0, 1, leftout, rightout);
    /* .....  */
    
    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
    
    /* initialise sinusoidal wavetable */
    for( i=0; i<TABLE_SIZE; i++ )
    {
        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    data.left_phase = data.right_phase = 0;
    data.azim = -1;
    
    
    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &data );
    if( err != paNoError ) goto error;

    sprintf( data.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Play for %d seconds.\n", NUM_SECONDS );
    Pa_Sleep( NUM_SECONDS * 1000 );

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
    
    // clean up 3D audio
    cleanup();
}