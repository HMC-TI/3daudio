package com.example.audioplayback;
import java.lang.Math;

public class Gain extends Thread {
	private double oldDistance;
	private double newDistance;
	private double[] rampup; 
	public double[] gain = new double[128];
	
	//number of time samples
	static final int time_samples = 128;
	
	Gain(double oldDistance, double newDistance, double[] rampup){
		// Constructor
		this.oldDistance = oldDistance;
		this.newDistance = newDistance;
		this.rampup = rampup;
	}
	
	public void run() {
		if (this.newDistance < 1) {
			this.newDistance = 1;
		}
		if (this.oldDistance < 1) {
			this.oldDistance = 1;
		}
		double distanceAdd = 1.0/Math.pow(this.oldDistance, 2);
		double distanceChange = (1.0/Math.pow(this.newDistance, 2) - distanceAdd);
		
		for (int i = 0; i < time_samples; i++) {
			gain[i] = rampup[i]*distanceChange + distanceAdd;
		}
		return;
	}
}
