package com.example.audioplayback;

import java.util.Arrays;
/**
 * @author acard
 * This contains the set of tests for all of the functions in the Audio3D class
 */
public class TestsAudio3D {
	static float[] input1 = new float[128];
	static float[] input2 = new float[128];
	static int azimuth;
	public static Audio3D audioTest = new Audio3D(input1, input2, azimuth, 5, 0, false);
	public static GetIRF irf = new GetIRF();
	
	public static void main(String[] args) {
		
		// now run the tests. Note that the tests should probably print some numbers
		testInit();
		testCreateNewOutputs();
		testCreateOldConvolveAndCrossfade();
		testCrossfade();
		
	}
	
	/**
	 * Tests the Init function to make sure the outputs are correct
	 */
	private static void testInit() {
		Audio3D.init();
		
		System.out.println("####################################################");
		System.out.println("Testing the init function in 3DAudio");
		System.out.println("####################################################");
		System.out.println(Arrays.toString(Audio3D.rampUp));
		System.out.println(Arrays.toString(Audio3D.rampDn));
		System.out.println(Arrays.toString(Audio3D.oldInBuf_l));
		System.out.println(Arrays.toString(Audio3D.oldInBuf_r));
		
	}
	/**
	 * Tests the createNewOutputs function
	 */
	private static void testCreateNewOutputs() {
		Audio3D.irfBuf = GetIRF.get_irf(0,0);
		Arrays.fill(Audio3D.inBuf_l, 1);
		Arrays.fill(Audio3D.inBuf_r, 2);
		
		Audio3D.createNewOutputs();
		
		System.out.println("####################################################");
		System.out.println("Create new outputs function with chanFlag=false");
		System.out.println("####################################################");
		System.out.println(Arrays.toString(Audio3D.irfBuf[0]));
		System.out.println(Arrays.toString(Audio3D.irfBuf[1]));
		System.out.println(Arrays.toString(Audio3D.newOut_l));
		System.out.println(Arrays.toString(Audio3D.newOut_r));
		
		Audio3D.chanFlag = true;
		Audio3D.elev = 0;
		Audio3D.az = 0;
		Audio3D.loadIRFs();
		
		Audio3D.createNewOutputs();
		
		System.out.println("####################################################");
		System.out.println("Create new outputs function with chanFlag=true");
		System.out.println("####################################################");
		System.out.println(Arrays.toString(Audio3D.newOut_l));
		System.out.println(Arrays.toString(Audio3D.newOut_r));
		
	}
	/**
	 * Tests the createOldConvolveAndCrossfade function
	 */
	private static void testCreateOldConvolveAndCrossfade() {
		Audio3D.oldIrf = Audio3D.irfBuf;
		
		Audio3D.createOldConvolveAndCrossfade();
		
		System.out.println("####################################################");
		System.out.println("Create new outputs function with chanFlag=true");
		System.out.println("####################################################");
		System.out.println(Arrays.toString(Audio3D.newOut_l));
		System.out.println(Arrays.toString(Audio3D.newOut_r));
	}
	/**
	 * Tests the crossfade function
	 */
	private static void testCrossfade() {
		
		Arrays.fill(Audio3D.newOut_l, 10);
		Arrays.fill(Audio3D.newOut_r, 20);
		Arrays.fill(Audio3D.oldOut_l, 1);
		Arrays.fill(Audio3D.oldOut_r, 2);
		
		Audio3D.crossfade(Audio3D.newOut_l, Audio3D.oldOut_l, Audio3D.newOut_l);
		Audio3D.crossfade(Audio3D.newOut_r, Audio3D.oldOut_r, Audio3D.newOut_r);
		
		System.out.println("####################################################");
		System.out.println("Testing cross-fading");
		System.out.println("####################################################");
		System.out.println(Arrays.toString(Audio3D.newOut_l));
		System.out.println(Arrays.toString(Audio3D.newOut_r));
	}
}
