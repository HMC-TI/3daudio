package com.example.audioplayback;

import android.os.Bundle;
import android.app.Activity;
import android.util.FloatMath;

public class AudioPlayback extends Activity
{	    
   @Override
   public void onCreate(Bundle savedInstanceState) 
   {
		super.onCreate(savedInstanceState);

		new Thread(new Runnable() {
			public void run() {
				double az = 0;
				final float frequency = 440;
				float increment = (float) (2 * Math.PI) * frequency / 44100; // angular
																				// increment
																				// for
																				// each
																				// sample
				float angle = 0;
				AndroidAudioDevice device = new AndroidAudioDevice();
				float samples[] = new float[128];
				for (int i = 0; i < samples.length; i++) {
					samples[i] = (float) FloatMath.sin(angle);
					angle += increment;
				}
				// Instantiate Audio3D class and output datum
				// Audio3D audioModder = new Audio3D(samples, samples,
				// azimGen.az, dist, el, false);
				float[] out;
				Audio3D audioModder = new Audio3D(samples, samples, 0, 5, 0,
						false);
				audioModder.init();
				audioModder.updateInbuf(samples, samples); // Load in samples (won't change)
				IRF_DATUM finalOut = new IRF_DATUM();

				while (true) {
					az = az + 0.1;
					audioModder.updateLocation(az, 0.0, 5.0);  // Update position



					// Futz the input sound
					out = audioModder.runAudio3D();

					// Return final output
					// device.writeSamples(finalOut.left );

					device.writeSamples(out);
				}
			}
		}).start();
	}
}