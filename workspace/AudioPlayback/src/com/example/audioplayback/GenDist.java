package com.example.audioplayback;

public class GenDist extends Thread {
	public double dist;
	private double direction = 0.5;
	
	GenDist(int dist) {
		this.dist = dist;
	}
	// Goes up and down between -40 and 90
	public void run() {
		if (dist >= 10) {
			direction = -1;
		}
		else if (dist <= 1) {
			direction = 1;
		}
		dist += direction;
	}
}
