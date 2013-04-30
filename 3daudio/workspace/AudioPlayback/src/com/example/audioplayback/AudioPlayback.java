package com.example.audioplayback;

import java.util.Arrays;

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
				Audio3D audioModder = new Audio3D(0, 0);
				// Up until this point, all data is what we think it should be
				float[] out;
				double az = 0;
				double elev = 0;

				while (true) {
					az = az + 0.1;
				
					//az = -90;
					
					//System.out.println("The azimuth is " + az%360);
					
					// Update position
					audioModder.updateLocation(az, elev);

					// The sound to be played
					out = audioModder.runAudio3D();
					
					//System.out.println(Arrays.toString(out));
//					System.out.println(out.length);
//					System.out.println(out[390]);
//					System.out.println(out[398]);
//					System.out.println(out[399]);
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