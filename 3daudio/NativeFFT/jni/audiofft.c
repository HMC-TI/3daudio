#include <jni.h>
#include <fftw3.h> //This is how we include the fftw library
#include <stdlib.h>

// Note I want to use floats in fftw so I replaced all instances of fftw_ with fftwf_
jfloatArray Java_main_nativefft_AudioFFT_performAudioTransform(JNIEnv *env, jobject obj, jfloatArray irfReal, jfloatArray irfImag, jfloatArray data) {
	// The size of the array
	int N = 128;
	// internal storage of the inputs
	float *in, *real, *img;
	// temporary array to store transformed data
	fftwf_complex *out;
	fftwf_plan p;
	// the output
	jfloatArray result = env->NewFloatArray(N);

	/* I'm not going to use this becuase I can use JNI to get the pointer to
	the data I want.
		in = (double*) fftw_malloc(sizeof(double) * N);*/
	in = env->GetFloatArrayElements(env, data, NULL);
	real = env->GetFloatArrayElements(env, irfReal, NULL);
	img = env->GetFloatArrayElements(env, irfImag, NULL);

	out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (N/2+1));

	/* Create a fftw plan that takes in floats, the destroy input allows the
	algorithm to be more efficient. I think this is fine because the
	GetFloatArrayElements function input NULL specifies that it should create a
	copy of the data.*/
	p = fftwf_plan_dft_r2c_1d(N, in, out, FFTW_DESTROY_INPUT);

	fftwf_execute(p);

	fftwf_destroy_plan(p);

	/* Now I'm going to perform the desired calculations to multiply the data
	together. Note: I divide by 9 because for some reason the fftw function tripples
	every time it goes through the function*/
	int i;
	for(i=0; i<N/2+1; i++) {
		// Multiply the real parts
		out[i][0] = out[i][0]*real[i]/9.0;
		// Multiply the imaginary parts
		out[i][1] = out[i][1]*real[i]/9.0;
	}

	// Now turn the data back into the time domain
	p = fftwf_plan_dft_c2r_1d(N, out, in, FFTW_DESTROY_INPUT);

	fftwf_execute(p);

	fftwf_destroy_plan(p);

	fftwf_free(out);

	// Creat output
	env->SetFloatArrayRegion(result, 0, N, in);
	return result;
}

// This command works kind of:  gcc fftTest.c -lfftw3f -lm
