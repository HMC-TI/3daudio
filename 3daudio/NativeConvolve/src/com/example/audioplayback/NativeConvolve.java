package com.example.audioplayback;
import android.os.AsyncTask;
import android.util.Log;

import java.util.Arrays;

public class NativeConvolve extends AsyncTask <Void, Void, Void>{
	static final int time_samples = 128;

	float[] xOld = new float[time_samples];
	float[] x = new float[time_samples];
	float[] y = new float[time_samples];
	public float[] out = new float[time_samples];

	
		static {  
	        // load library  
	        //System.loadLibrary("convolve");  
			try {
			    System.loadLibrary("convolve");
			} catch (UnsatisfiedLinkError use) {
			    Log.e("JNI", "WARNING: Could not load libmysharedlibrary.so");
			}
	    }  
	    // native method signature  
	    public native float[] convolve(float[] xOld, float[] xNew, float[] irf);  
	    
	    public Void doInBackground(Void... args0) {
	    	/*****
	    	 * Mental note, this returns something and we're not dealing with that
	    	 */
	    	new NativeConvolve(xOld, x, y).convolve(xOld, x, y);
	    	return null;
	    }
	    
	    NativeConvolve(float[] xOld, float[] x, float[] y) {
			// Constructor
			this.xOld = xOld;
			this.x = x;
			this.y = y;
			// fills out with zeros so that there is no confusion
			Arrays.fill(this.out, 0);
	    }
}
