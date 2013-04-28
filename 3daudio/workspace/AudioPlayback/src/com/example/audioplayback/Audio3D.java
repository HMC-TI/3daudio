package com.example.audioplayback;
import java.util.Arrays;


public class Audio3D {	
	// Constants
	
	// Public members
	static double az;
	static double elev = 0;
	
	static HACKED_SAMPLES newOut = new HACKED_SAMPLES();
	float[] finalOut = new float[HACKED_SAMPLES.sample_size*2];
	//float[] finalOut = new float[HACKED_SAMPLES.sample_size];
	
	public GetIRF getIrf = new GetIRF();
	
	Audio3D(int azimuth, int elevation){
		Audio3D.az = azimuth;
		Audio3D.elev = elevation;
	}
	
	public float[] runAudio3D() {

		// ////////////
		// Get current IRFs //
		// ////////////
		loadIRFs();
		
		for (int i = 0; i < newOut.left.length; i++) {
			finalOut[i*2] = newOut.left[i];
			finalOut[i*2 + 1] = newOut.right[i];
		}
		
		return finalOut;
		//return newOut.left;
	}
	
	/*************************************************
	 * New function
	 **************************************************/
	void loadIRFs(){
		newOut = getIrf.get_irf(elev, az);
	}
	
	void updateLocation(double newaz, double newelev) {
		az = newaz;
		elev = newelev;
	}
}