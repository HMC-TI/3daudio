#include <stdio.h>
#include <jni.h>

#define TIME_SAMPLES 128
#define SKIP_FRONT 127
#define SKIP_END 254


JNIEXPORT jfloatArray JNICALL Java_com_example_audioplayback_NativeConvolve_convolve(JNIEnv *env, jobject obj, jfloatArray xOld, jfloatArray xNew, jfloatArray irf) {
	
	float *xO, *x, *y;
	float out[TIME_SAMPLES];
	jfloatArray result = (*env)->NewFloatArray(env, TIME_SAMPLES);
	
	xO = (*env)->GetFloatArrayElements(env, xOld, NULL);
	x = (*env)->GetFloatArrayElements(env, xNew, NULL);
	y = (*env)->GetFloatArrayElements(env, irf, NULL);

	int i;
	for(i = 0; i < TIME_SAMPLES; i++) {
		int j;
		for(j = 0; j < (TIME_SAMPLES*2); j++) {
			// initializing out as zero
			if(i==0 & j==0 & j<TIME_SAMPLES) {
				out[j] = 0;
			}
			// regular stuff
			if ((!((i + j) < SKIP_FRONT)) && (!((i + j) > SKIP_END))) {
				if (j >= TIME_SAMPLES) {					
					out[i+j-SKIP_FRONT] = out[i+j-SKIP_FRONT] + x[j-TIME_SAMPLES]*y[i];
				} else {
					out[i+j-SKIP_FRONT] = out[i+j-SKIP_FRONT] + xO[j]*y[i];
				}
			}
		}
	}
	
	(*env)->SetFloatArrayRegion(env, result, 0, TIME_SAMPLES, out);
	return result;
	
}
