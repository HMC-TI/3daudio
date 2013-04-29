#include <jni.h>
#include <fftw3.h> //This is how we include the fftw library
#include <stdlib.h> // Necessary to use NULL

// Note I want to use floats in fftw so I replaced all instances of fftw_ with fftwf_
JNIEXPORT jfloatArray JNICALL
Java_main_nativefft_IRFTransform_FFT(JNIEnv *env, jobject this, jfloatArray irf) {
	// The size of the array
	int N = 128;
	// internal storage of the inputs
	float *in;
	// temporary array to store transformed data
	fftwf_complex *out;
	fftwf_plan p;
	// the output which has the real part in the first (N/2+1) and the imaginary part in the last (N/2+1)
	jfloatArray result = (*env)->NewFloatArray(env, N+2);

	/* I'm not going to use this becuase I can use JNI to get the pointer to
	the data I want.
		in = (double*) fftw_malloc(sizeof(double) * N);*/
	in = (*env)->GetFloatArrayElements(env, irf, NULL);

	out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * (N/2+1));

	/* Create a fftw plan that takes in floats, the destroy input allows the
	algorithm to be more efficient. I think this is fine because the
	GetFloatArrayElements function input NULL specifies that it should create a
	copy of the data.*/
	p = fftwf_plan_dft_r2c_1d(N, in, out, FFTW_DESTROY_INPUT);

	fftwf_execute(p);

	fftwf_destroy_plan(p);


	/*int i;
	for (i=0; i<2; i++) {
		// Creat output
		jfloatArray floatArray = (*env)->NewFloatArray(env, N/2+1);
		(*env)->SetFloatArrayRegion(env, floatArray, 0, N/2+1, out[i]);
    (*env)->SetObjectArrayElement(env, result2D, i, floatArray);
    (*env)->DeleteLocalRef(env, floatArray);
	}*/

	(*env)->SetFloatArrayRegion(env, result, 0, N/2+1, out[0]);
	(*env)->SetFloatArrayRegion(env, result, N/2+1, N+2, out[1]);

	fftwf_free(out);

	if (result == NULL) {
		LOGE("ARRAY NOT CREATED");
		return NULL;
	}

	return result;
}
