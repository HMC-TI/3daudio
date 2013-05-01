/*
 * Code to load/transform/lookup IRF values 
 *
 * Code modified from:
 * Bill Gardner
 * Copyright (c) 1995 MIT Media Lab All Rights Reserved.
 */
#include	<stdio.h>
#include	<math.h>
#include 	<stdlib.h>
/*#include	"config.h"
#include	"sgi_fft.h"
#include	"irf.h"
#include	"procbuf.h"
#include	"libstuff.h"
#include	"arraylib.h"
#include	"misc.h"*/

/*************************************************************
 * Default root path, can be overridden by setting environment
 * variable IRFROOT.
 */
#define ROOT_ENV	"IRFROOT"
#define ROOT_PATH	"../irfs/"
#define PATH_FMT	"%s/elev%d/H%de%03da%s"
#define EXTENSION	".txt"
#define IRF_LEN	256
#define FILTER_LEN 128

#define TRUE 1
#define FALSE 0

char rootpath[256];

/*
 * An IRF is stored as left and right spectra for easy convolution.
 * The compact IRFs are 128 time samples, which give us 128 complex
 * spectral points (which are conjugate symmetric). Depending on the
 * type of fft used, we will either have 128 complex points (fft.h)
 * or 65 complex points (sgi_fft.h).
 */
typedef struct {
	double *left;
	double *right;
} IRF_DATUM;

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

/*
 make sure this doesn't casue problems
 */
int az = 0;
int elev = 0;


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
	
	/*printf("The el index is %d\n", cur_el_index);
	printf("The az index is %d\n", cur_az_index);*/

	/*
	 * Get data and flip channels if necessary.
	 */
	hd = &irf_data[cur_el_index][cur_az_index];
	if (cur_flip_flag) {
		*p_left = hd->right;
		*p_right = hd->left;
	}
	else {
		*p_left = hd->left;
		*p_right = hd->right;
	}
}

void main(void) {
	read_irfs();
	
	printf("Successfully loaded the files into the double array \n");
	
	double elev = 10;
	double azim = 1;
	
	double *filt_L;
	double *filt_R;
	
	struct IRF_DATUM *hd = malloc(sizeof(IRF_DATUM *));
	
	//read_irf(4,0, hd); 
	// it reads in all the files correctly, so what is going wrong here?
	
	get_irf(elev, azim, &filt_L, &filt_R);
	
	printf("The left filter is \n");
	int i;
	for(i=0; i<FILTER_LEN; i++) {
		printf(" %f\n", filt_L[i]);
	}
	printf("Done testing \n");
}
