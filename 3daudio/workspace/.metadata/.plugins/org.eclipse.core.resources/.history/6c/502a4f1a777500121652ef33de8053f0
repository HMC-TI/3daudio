package com.example.audioplayback;
import java.util.Arrays;


public class Audio3D {
	// Just switch this to 0 to disable debug statements
	static final int debug = 1;
	
	// Constants

	//number of time samples
	static final int time_samples = 128;
	
	// Public members
	static boolean firstRun = true;
	static int az;
	static int dist;
	static int oldAz = 0;
	static int elev = 0;
	static int oldElev = 0;
	static float[] inBuf_l = new float[time_samples];
	static float[] inBuf_r = new float[time_samples];
	static float[] oldInBuf_l = new float[time_samples];
	static float[] oldInBuf_r = new float[time_samples];
	// why do we need outBuf and newOut? Aren't they the same thing?
	//static double[] outBuf_l = new double[time_samples];
	//static double[] outBuf_r = new double[time_samples];
	static boolean chanFlag = false;
	
	
	/*****************************************************
	 * If these are private then shouldn't they have a private out front?
	 * Also, what is the meaning of static? Should we really be using it here?
	 ******************************************************/
	// Private members
	// 2D arrays, the first row's destination is the 
	// left speaker, and the first row's destination
	// is the right speaker.
	static float[][] irfBuf = new float[2][time_samples];
	/******************************************************
	 * New variables
	 ******************************************************/
	float[][] irfBufR = new float [2][time_samples];
	float[][] oldIrfR = new float [2][time_samples];
	/*******************************************************/
	static float[][] oldIrf = new float [2][time_samples];
	static float[] oldOut_l = new float[time_samples];
	static float[] oldOut_r = new float[time_samples];
	static float[] newOut_l = new float[time_samples];
	static float[] newOut_r = new float[time_samples];
	static boolean cfFlag; // Are we cross-fading or what?
	static float[] rampUp = new float[time_samples];
	static float[] rampDn = new float[time_samples];
	
	public GetIRF getIrf;
	
	Audio3D(float[] inputL, float[] inputR, int azimuth, int distance, int elevation, boolean channels){
		Audio3D.az = azimuth;
		Audio3D.dist = distance;
		Audio3D.elev = elevation;
		// If chanFlag is true, then we are taking in both L and R channel inputs. 
		// Otherwise, we are only taking in one channel of input.
		Audio3D.chanFlag = channels; 
		if (chanFlag) {
			for (int i = 0; i < time_samples; i++) {
				Audio3D.inBuf_l[i] = (float) inputL[i];
				Audio3D.inBuf_r[i] = (float) inputR[i];
			}
		} else {
			for (int i = 0; i < time_samples; i++) {
				Audio3D.inBuf_l[i] = (float) inputL[i];
			}
		}
	}
	
	public IRF_DATUM runAudio3D() {
		if (debug == 1) System.out.println("Entered runAudio3D");


		// Initialize ramp- Moved this outside the while loop in AudioPlayback
		//init(); 
		
		// Initialize IRF stuff
		/******************************
		 *  Because we moved the IRF stuff we need to initialize the class
		 *******************************/
		/*GetIRF getIRF = new GetIRF();
		
		// continually updates the tone, azimuth etc and then computes calculations and plays them
		while (true) {
			// Generate changing azimuth
			GenAzim aziGen = new GenAzim(0); // read azimuth by using aziGen.az
			
			 * Here to...
			 
			// Runs the methods to get new azimuth and a new set of tones
			toneGen.start();
			aziGen.start();
			
			// waits for this thread to complete before setting the new
			// azimuth to the old value
			aziGen.join();
			az = aziGen.az;
			
			// waits for the tone to die
			toneGen.join();
			// setting up the input buffer
			inBuf_l = toneGen.inBuf;
			
			 * Here should be in the other main and this should be a function which takes in
			 * inbuf, az, elev, dist
			 */

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
		/***************************************
		 * New!!
		 ****************************************/
		loadIRFs(); 
		
		// The IRFs are intact for the first run, but they go to 0 from the second
		// run on.. what's going on?
		if (debug == 1) {
			for (int i = 0; i < irfBuf[0].length; i++) {
				System.out.println("Before createNewOutputs, left irf["+i+"]: "+irfBuf[0][i]);
				System.out.println("Before createNewOutputs, left input["+i+"]: "+inBuf_l[i]);
			}
		}

		// ////////////
		// Convolve //
		// ////////////
		createNewOutputs();
		// After the first run, everything is ruined by createNewOutputs, apparently
		if (debug == 1) {
			for (int i = 0; i < irfBuf[0].length; i++) {
				//System.out.println("newOut_l["+i+"]: "+newOut_l[i]);
			}
		}
		
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
		// Carries over the old IRFs, the old time samples
		oldInBuf_l = inBuf_l;
		oldInBuf_r = inBuf_r;
		oldIrf = irfBuf;
		oldIrfR = irfBufR;
		
		IRF_DATUM outputs = new IRF_DATUM();
		outputs.right = newOut_r;
		outputs.left = newOut_l;
		
		if (debug == 1) {
			for (int i = 0; i < newOut_l.length; i++) {
				//System.out.println("Left output["+i+"]: "+outputs.left[i]);
			}
		}
		if (!firstRun) {
			while (true) {}
		} else {
			System.out.println("This was the first run!");
		}
		firstRun = false; 
		return outputs;
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
		// Check irfs
		/*for (int i = 0; i < getIrf.irf_data[4][4].left.length; i++) {
			// so we know irf_data gets populated properly
			//System.out.println("Sample irf["+i+"]: "+getIrf.irf_data[4][4].left[i]);
		}*/
		firstRun = true;
		for (int i = 0; i < time_samples; i++) { // Initialize ramps
			rampUp[i] = (float) (i/(time_samples-1));
			rampDn[i] = 1 - rampUp[i];
			oldInBuf_r[i] = 0;
			oldInBuf_l[i] = 0;
		}
	}
	
	/*************************************************
	 * New function
	 **************************************************/
	void loadIRFs(){
		if (chanFlag) {
			// when you get the irf data for two channels then you need to get the irfs from 90 if the az = 0
			irfBuf = getIrf.get_irf(elev, az + 90);
			irfBufR = getIrf.get_irf(elev, 180-(az+90));
		}
		else {
			// Exception at this point. Why?
			irfBuf = getIrf.get_irf(elev, az);
		}
	}
	
	/**
	 * This function calculates the newOut buffers from the input buffers and the irfs.
	 * 
	 * If our input consists of a left and a right chanel, then we do the convolustion 4 times,
	 * we convolve the left chanel with both the left and the right irfs and do the same with the right
	 * chanel. In this way we hope to capture the data from both the chanels.
	 */
	void createNewOutputs() {
		if (chanFlag) { // If we're taking in 2 inputs, we need 4 convolutions
			/************************************************
			 * This has been changed
			 ************************************************/
			Convolve new_rl = new Convolve(oldInBuf_l, inBuf_l, irfBuf[1]);
			Convolve new_rr = new Convolve(oldInBuf_r, inBuf_r, irfBufR[0]);
			Convolve new_ll = new Convolve(oldInBuf_l, inBuf_l, irfBuf[0]);
			Convolve new_lr = new Convolve(oldInBuf_r, inBuf_r, irfBufR[1]);
			
			// starts threads
			new_rl.start();
			new_rr.start();
			new_ll.start();
			new_lr.start();
			
			// waits for them to finish
			try {
				new_rl.join();
				new_rr.join();
				new_ll.join();
				new_lr.join();
			}
			catch (InterruptedException e) {
				System.out.println(e);
			}
			// ///////
			// Sum //
			// ///////
			for (int i = 0; i < time_samples; i++) {
				newOut_l[i] = new_ll.out[i] + new_lr.out[i];
				newOut_r[i] = new_rl.out[i] + new_rr.out[i];
			}
		} else { // Otherwise, we need only two
					// note: if we only have one input stream then we put it in
					// inBuf_l
			Convolve new_ll = new Convolve(oldInBuf_l, inBuf_l, irfBuf[0]);
			Convolve new_lr = new Convolve(oldInBuf_l, inBuf_l, irfBuf[1]);
			
			if (debug == 1) {
				for (int i = 0; i < newOut_l.length; i++) {
					System.out.println("Left irf["+i+"]: "+irfBuf[0][i]);
					System.out.println("Left inbuf["+i+"]: "+inBuf_l[i]);
					System.out.println("Old left inbuf["+i+"]: "+oldInBuf_l[i]);
				}
			}
			// starts the threads and then waits for them to complete
			new_ll.start();
			new_lr.start();
			try {
				new_ll.join();
				new_lr.join();
			}
			catch (InterruptedException e) {
				System.out.println(e);
			}

			// ////////////////////////
			// No summing necessary //
			// ////////////////////////
			newOut_l = new_ll.out;
			newOut_r = new_lr.out;
		}
	}
	/**
	 * If we have had a change in our irf buffer then we need to compute what the output signal
	 * would be with the old irf buffer and then crossfade between the two outputs.
	 */
	void createOldConvolveAndCrossfade() {
		// we have both a left and a right chanel input
		if (chanFlag) {
			/*****************************************
			 * Changed
			 ******************************************/
			Convolve old_rl = new Convolve(oldInBuf_l, inBuf_l, oldIrf[1]);
			Convolve old_rr = new Convolve(oldInBuf_r, inBuf_r, oldIrfR[0]);
			Convolve old_lr = new Convolve(oldInBuf_l, inBuf_l, oldIrf[0]);
			Convolve old_ll = new Convolve(oldInBuf_r, inBuf_r, oldIrfR[1]);
			
			// starts the threads
			old_rl.start();
			old_rr.start();
			old_ll.start();
			old_lr.start();
			// waits for them to complete
			try {
				old_rl.join();
				old_rr.join();
				old_ll.join();
				old_lr.join();
			}
			catch (InterruptedException e) {
				System.out.println(e);
			}

			// ///////
			// Sum //
			// ///////
			for (int i = 0; i < time_samples; i++) {
				oldOut_l[i] = old_ll.out[i] + old_lr.out[i];
				oldOut_r[i] = old_rl.out[i] + old_rr.out[i]; 
			}

		} else {
			Convolve old_ll = new Convolve(oldInBuf_l, inBuf_l, oldIrf[0]);
			Convolve old_lr = new Convolve(oldInBuf_l, inBuf_l, oldIrf[1]);
			// starts threads and then waits for them to complete
			old_ll.start();
			old_lr.start();
			try {
				old_ll.join();
				old_lr.join();
			}
			catch (InterruptedException e) {
				System.out.println(e);
			}

			// ////////////////////////
			// No summing necessary //
			// ////////////////////////
			oldOut_l = old_ll.out;
			oldOut_r = old_lr.out;

		}

		// ////////////////
		// Ramp and sum //
		// ////////////////
		/********************************************************
		 * We should check and make sure that this actually gives
		 * us the desired values in newOut
		 ********************************************************/
		crossfade(newOut_l, oldOut_l, newOut_l);
		crossfade(newOut_r, oldOut_r, newOut_r);

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
	
	void updateLocation(int newaz, int newelev, int newdist) {
		// Saves old az and elev- this may not be necessary, as it is done in runAudio3D
		oldAz = az;
		oldElev = elev;
		dist = newdist;
		az = newaz;
		elev = newelev;
	}
}