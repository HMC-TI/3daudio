package com.example.audioplayback;
import java.util.Arrays;

public class Convolve extends Thread {
			
	/* 
	 * This function convolves the inputs x*y and puts them into the
	 * output out. All of the inputs to this function should be 512
	 * in length and should be declared in the over arching class so
	 * that pointers are passed instead of creating new variables
	 * 
	 */
	
	//number of time samples
	static final int time_samples = 128;
	
	float[] xOld = new float[time_samples];
	float[] x = new float[time_samples];
	float[] y = new float[time_samples];
	public float[] out = new float[time_samples];
	
	
	
	Convolve(float[] xOld, float[] x, float[] y){
		// Constructor
		this.xOld = xOld;
		this.x = x;
		this.y = y;
		// fills out with zeros so that there is no confusion
		Arrays.fill(this.out, 0);
	}
	
	public void run(){
		// length of out - 128 - N
		int skipFront = 127;
		// length of out - 128
		int skipEnd = 254;
		
		
		for (int i = 0; i < time_samples; i++){
			for (int j = 0; j < (time_samples + time_samples); j++){
				if (!((i+j) < skipFront) && !((i+j) > skipEnd)) {
					if (j >= time_samples) {
						this.out[i+j-skipFront] = this.out[i+j-skipFront] + this.x[j-time_samples]*this.y[i];
					}
					else {
						this.out[i+j-skipFront] = this.out[i+j-skipFront] + this.xOld[j]*this.y[i];
					}
						
				}
			}
		}
		
		return;
	}
}
