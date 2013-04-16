package com.example.audioplayback;

import java.util.Arrays;

import android.os.Bundle;
import android.app.Activity;
import android.util.FloatMath;


public class AudioPlayback extends Activity {
	public static int sample_size = 150;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		new Thread(new Runnable() {
			public void run() {
				AndroidAudioDevice device = new AndroidAudioDevice();
				Audio3D audioModder = new Audio3D(0, 5);
				audioModder.init();
				// Up until this point, all data is what we think it should be
				float[] out;
				double az = 0;

				while (true) {
					az = az + 0.1;
					// Update position
					audioModder.updateLocation(az, 0.0, 5.0);

					// Futz the input sound
					out = audioModder.runAudio3D();

					// Return final output
					// device.writeSamples(finalOut.left );

					// we need to fix this so that we can do both audio changes
					// at once
					device.writeSamples(out);
				}
			}
		}).start();
	}
}