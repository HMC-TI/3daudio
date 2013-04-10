package com.example.audioplayback;
import java.util.Arrays;


public class Audio3D {	
	// Constants

	//number of time samples
	static final int time_samples = 128;
	
	// Public members
	static double az;
	static double dist;
	static double oldAz = 0;
	static double elev = 0;
	static double oldElev = 0;
	
	static HACKED_SAMPLES oldOut = new HACKED_SAMPLES();
	static HACKED_SAMPLES newOut = new HACKED_SAMPLES();
	
	static boolean cfFlag; // Are we cross-fading or what?
	static float[] rampUp = new float[time_samples];
	static float[] rampDn = new float[time_samples];
	
	public GetIRF getIrf = new GetIRF();
	
	Audio3D(int azimuth, int elevation){
		Audio3D.az = azimuth;
		Audio3D.oldAz = azimuth;
		Audio3D.elev = elevation;
		Audio3D.oldElev = elevation;
	}
	
	public HACKED_SAMPLES runAudio3D() {
		// ///////////////////////////
		// Checks for cross-fading //
		// ///////////////////////////
		if ((oldAz != az) || (oldElev != elev))
			cfFlag = true;
		else
			cfFlag = false;

		// ////////////
		// Get current IRFs //
		// ////////////
		loadIRFs();
		
		// /////////////
		// Crossfade //
		// /////////////
		if (cfFlag) {
			createOldConvolveAndCrossfade();
		}

		// Carries over the old azimuth and elevation for cross-fading
		// purposes
		oldAz = az;
		oldElev = elev;
		oldOut = newOut;
		
		return newOut;
	}
	
	//////////////////////////////
	// Generic helper functions //
	//////////////////////////////
	/*****************************************************
	 * Changed this function
	 *****************************************************/
	void init() {
		// Read in 3D audio data from file
		GetIRF getIrf = new GetIRF();
		getIrf.read_irfs();
		
		for (int i = 0; i < time_samples; i++) { // Initialize ramps
			rampUp[i] = (float) (i/(time_samples-1));
			rampDn[i] = 1 - rampUp[i];
		}
	}
	
	/*************************************************
	 * New function
	 **************************************************/
	void loadIRFs(){
		newOut = getIrf.get_irf(elev, az);
	}
	
	
	/**
	 * If we have had a change in our irf buffer then we need to compute what the output signal
	 * would be with the old irf buffer and then crossfade between the two outputs.
	 */
	void createOldConvolveAndCrossfade() {
		crossfade(newOut.left, oldOut.left, newOut.left);
		crossfade(newOut.right, oldOut.right, newOut.right);

	}
	
	void crossfade(float[] newIn, float[] oldIn, float[] out){
		/*double[] temp = new double[time_samples];
		 * I don't think you want to do this because otherwise you have to set you values
		 * again. Instead add an out to the function and just put your desired variable in
		 */
		for (int i = 0; i < time_samples; i++) {
			out[i] = newIn[i]*rampUp[i] + oldIn[i]*rampDn[i];
		}
		return;
	}
	
	void updateLocation(double newaz, double newelev, double newdist) {
		// Saves old az and elev- this may not be necessary, as it is done in runAudio3D
		oldAz = az;
		oldElev = elev;
		dist = newdist;
		az = newaz;
		elev = newelev;
	}
}