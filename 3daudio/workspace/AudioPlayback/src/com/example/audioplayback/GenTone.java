package com.example.audioplayback;

public class GenTone extends Thread {
	private double fs = 44100;
	// Note, the 1.0 is needed to make this a double rather than an int
	private double dt = 1.0/fs;
	private double tone = 440;
	private double time = 0;
	public double[] inBuf = new double[128];
	
	GenTone(int tone){
		this.tone = tone;
	}
	
	public void run() {
		//while (true){
			for (int i = 0; i < inBuf.length; i++) {
				inBuf[i] = Math.sin(2*Math.PI*tone*time);
				time += dt;
				//System.out.format("Loaded %f into buffer %n", inBuf[i]);
			}
		//}
	}
}
