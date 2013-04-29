package main.nativefft;

import java.util.Arrays;

import android.os.AsyncTask;
import android.util.Log;

public class AudioFFT extends AsyncTask <Void, Void, Void>{
	static final int time_samples = 128;

	float[] irfReal;
	float[] irfImag;
	float[] data;
	float[] out;
	
	static {  
        // load library  
		try {
		    System.loadLibrary("audiofft");
		} catch (UnsatisfiedLinkError use) {
		    Log.e("JNI", "WARNING: Could not load libaudiofft.so");
		}
    }  
    // native method signature  
    public native float[] performAudioTransform(float[] irfReal, float[] irfImag, float[] data);
    
    public Void doInBackground(Void... args0) {
    	AudioFFT fft = new AudioFFT(irfReal, irfImag, data);
    	out = fft.performAudioTransform(irfReal, irfImag, data);
    	return null;
    }
    
    AudioFFT(float[] irfReal, float[] irfImag, float[] data) {
		// Constructor
		this.irfReal = irfReal;
		this.irfImag = irfImag;
		this.data = data;

    }
    
}
