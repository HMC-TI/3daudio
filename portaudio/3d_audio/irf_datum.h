/*
 * An IRF is stored as left and right spectra for easy convolution.
 * The compact IRFs are 128 time samples, which give us 128 complex
 * spectral points (which are conjugate symmetric). Depending on the
 * type of fft used, we will either have 128 complex points (fft.h)
 * or 65 complex points (sgi_fft.h).*/
 
typedef struct {
	double *left;
	double *right;
} IRF_DATUM;

