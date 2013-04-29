package main.nativefft;

import android.util.Log;

public class IRFTransform {
	static final int time_samples = 128;
	static {  
        // load library  
		try {
		    System.loadLibrary("irftransform");
		} catch (UnsatisfiedLinkError use) {
		    Log.e("JNI", "WARNING: Could not load libirftransform.so");
		}
    }  

	// native method signature  
	public native float[] FFT(float[] irf);
    
    public float[][] getComplexResults(float[] irf) {
    	float[] temp = this.FFT(irf);
    	float[][] out = new float[time_samples/2 + 1][2];
    	for (int i = 0; i < (time_samples/2 + 1); i++) {
    		out[i][0] = temp[i];
    		out[i][1] = temp[2*i];
    	}
    	return out;
    }
}
