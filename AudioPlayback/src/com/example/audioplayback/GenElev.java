package com.example.audioplayback;

public class GenElev extends Thread {
	public int elev;
	private int direction = 5;
	
	GenElev(int elev) {
		this.elev = elev;
	}
	// Goes up and down between -40 and 90
	public void run() {
		if (elev >= 90) {
			direction = -5;
		}
		else if (elev <= -40) {
			direction = 5;
		}
		elev += direction;
	}
}
