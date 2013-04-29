package com.example.audioplayback;

public class COMPLEX_DATUM {
	public static int sample_size = 128;

	//float[] left;
	//float[] right;
	float[][] left = new float[2][sample_size/2 + 1];
	float[][] right = new float[2][sample_size/2 + 1];
}