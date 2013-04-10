package com.example.audioplayback;

import android.os.Bundle;
import android.app.Activity;
import android.util.FloatMath;

public class AudioPlayback extends Activity {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		new Thread(new Runnable() {
			public void run() {
				AndroidAudioDevice device = new AndroidAudioDevice();
				Audio3D audioModder = new Audio3D(0, 5);
				audioModder.init();
				// Up until this point, all data is what we think it should be
				HACKED_SAMPLES finalOut = new HACKED_SAMPLES();

				while (true) {
					// Update position
					audioModder.updateLocation(0.0, 0.0, 5.0);
					;

					// Futz the input sound
					finalOut = audioModder.runAudio3D();

					// Return final output
					// device.writeSamples(finalOut.left );

					// we need to fix this so that we can do both audio changes
					// at once
					device.writeSamples(finalOut.left);
				}
			}
		}).start();
	}
}