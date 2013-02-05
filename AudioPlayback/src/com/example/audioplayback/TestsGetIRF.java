package com.example.audioplayback;

import java.util.Arrays;

/**
 * @author acard
 * This class should test the GetIRF class and associated functions. See TestsAudio3D for 
 * generic setup of a testing class
 */
public class TestsGetIRF {
	public static GetIRF getIrf = new GetIRF();

	public static void main(String[] args) {
		//load IRFs
		getIrf.read_irfs();
		printIrfs();
		
		//testElIndex();
		
		//print IRF

	}
	
	public static void printIrfs() {
		//access a specific IRF
		float[][] irf = getIrf.get_irf(0, 0);
		float[] right = irf[1];
		float[] left = irf[0];
		System.out.format("Printing irfs for e:%d and a:%d%n", getIrf.cur_el_index, getIrf.cur_az_index);
		System.out.println(Arrays.toString(getIrf.irf_data[0][0].right));
		System.out.println("left " + Arrays.toString(left) + " right " + Arrays.toString(right));
	}
	
	public static void testElIndex() {
		for (int i = 0; i < 50; i++) {
			int el_index = getIrf.get_el_index(i);
			System.out.format("For an elevation of %d cur_el_index is %d%n", i, el_index);
		}
	}
}
