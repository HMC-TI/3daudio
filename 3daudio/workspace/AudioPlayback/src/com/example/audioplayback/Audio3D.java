package com.example.audioplayback;
import java.util.Arrays;
import java.util.concurrent.ExecutionException;


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
	static float[] inBuf_l = new float[time_samples];
	static float[] inBuf_r = new float[time_samples];
	static float[] oldInBuf_l = new float[time_samples];
	static float[] oldInBuf_r = new float[time_samples];
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
	
	public GetIRF getIrf = new GetIRF();
	
	private IRF_DATUM outputs = new IRF_DATUM();
	
	Audio3D(float[] inputL, float[] inputR, int azimuth, int distance, int elevation, boolean channels){
		Audio3D.az = azimuth;
		Audio3D.dist = distance;
		Audio3D.elev = elevation;
		// If chanFlag is true, then we are taking in both L and R channel inputs. 
		// Otherwise, we are only taking in one channel of input.
		Audio3D.chanFlag = channels; 
		/* We don't need to do this any more because our input is a float.
		 * if (chanFlag) {
		 
			for (int i = 0; i < time_samples; i++) {
				Audio3D.inBuf_l[i] = (float) inputL[i];
				Audio3D.inBuf_r[i] = (float) inputR[i];
			}
		} else {
			for (int i = 0; i < time_samples; i++) {
				Audio3D.inBuf_l[i] = (float) inputL[i];
			}
		}*/
		
		if (chanFlag){
			Audio3D.inBuf_l = inputL;
			Audio3D.inBuf_r = inputR;
		}
		else {
			Audio3D.inBuf_l = inputL;
		}
	}
	
	public IRF_DATUM runAudio3D() {
		// ///////////////////////////
		// Checks for cross-fading //
		// ///////////////////////////
		/*
		 * This could be optimized because if the az = 1 and the oldAz = 0, we
		 * are not actually going to need to crossfade.
		 */
		if ((oldAz != az) || (oldElev != elev))
			cfFlag = true;
		else
			cfFlag = false;

		// ////////////
		// Get current IRFs //
		// ////////////
		loadIRFs(); 

		// ////////////
		// Convolve //
		// ////////////
		createNewOutputs();
		
		// /////////////
		// Crossfade //
		// /////////////
		if (cfFlag) {
			createOldConvolveAndCrossfade();
		}

		// Updating input buffers and az/elev 
		// is now taken care of in updateInbuf()
		// and updateLocation()
		
		/* We don't need to create a new one everytime, we can just have this be a variable.
		 * IRF_DATUM outputs = new IRF_DATUM();
		 * outputs.right = newOut_r;
		 * outputs.left = newOut_l;
		 */
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

		// Initialize ramps
		for (int i = 0; i < time_samples; i++) { 
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
			new_rl.execute();
			new_rr.execute();
			new_ll.execute();
			new_lr.execute();

			// We make sure that it has completed or wait until it has.
			try {
				new_rl.get();
				new_rr.get();
				new_ll.get();
				new_lr.get();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (ExecutionException e) {
				e.printStackTrace();
			}

			// ///////
			// Sum //
			// ///////
			/* We can save ourselves data transfer by just setting this to the output
			 * if we are not crossfading.
			 */
			if (cfFlag) {
				for (int i = 0; i < time_samples; i++) {
					newOut_l[i] = new_ll.out[i] + new_lr.out[i];
					newOut_r[i] = new_rl.out[i] + new_rr.out[i];
				}
			}
			else {
				for (int i = 0; i < time_samples; i++) {
					outputs.left[i] = new_ll.out[i] + new_lr.out[i];
					outputs.right[i] = new_rl.out[i] + new_rr.out[i];
				}
			}
			
		} else { // Otherwise, we need only two
					// note: if we only have one input stream then we put it in
					// inBuf_l
			Convolve new_ll = new Convolve(oldInBuf_l, inBuf_l, irfBuf[0]);
			Convolve new_lr = new Convolve(oldInBuf_l, inBuf_l, irfBuf[1]);
			
			// starts the threads and then waits for them to complete
			new_ll.execute();
			new_lr.execute();
			
			// We make sure that it has completed or wait until it has.
			try {
				new_ll.get();
				new_lr.get();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (ExecutionException e) {
				e.printStackTrace();
			}
			
			// ////////////////////////
			// No summing necessary //
			// ////////////////////////
			/*
			 * If there is no crossfading, we can save ourselves some data
			 * transfer but just making this the output.
			 */
			if (cfFlag) {
				newOut_l = new_ll.out;
				newOut_r = new_lr.out;
			}
			else {
				outputs.right = new_lr.out;
				outputs.left = new_ll.out;
			}
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
			old_rl.execute();
			old_rr.execute();
			old_ll.execute();
			old_lr.execute();

			// We make sure that it has completed or wait until it has.
			try {
				old_rl.get();
				old_rr.get();
				old_ll.get();
				old_lr.get();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (ExecutionException e) {
				e.printStackTrace();
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
			old_ll.execute();
			old_lr.execute();

			// We make sure that it has completed or wait until it has.
			try {
				old_ll.get();
				old_lr.get();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (ExecutionException e) {
				e.printStackTrace();
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
		crossfade();

	}
	
	void crossfade(){
		/*
		 * double[] temp = new double[time_samples]; I don't think you want to
		 * do this because otherwise you have to set you values again. Instead
		 * add an out to the function and just put your desired variable in
		 */
		for (int i = 0; i < time_samples; i++) {
			outputs.left[i] = newOut_l[i] * rampUp[i] + oldOut_l[i] * rampDn[i];
			outputs.right[i] = newOut_r[i] * rampUp[i] + oldOut_r[i] * rampDn[i];
		}
		return;
	}
	
	void updateInbuf(float[] new_l, float[] new_r) {
		oldInBuf_l = inBuf_l;
		oldInBuf_r = inBuf_r;
		inBuf_l = new_l;
		inBuf_r = new_r;
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
