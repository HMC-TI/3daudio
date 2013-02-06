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
 
      new Thread( new Runnable( ) 
      {
         public void run( )
         {        		
            final float frequency = 440;
            float increment = (float)(2*Math.PI) * frequency / 44100; // angular increment for each sample
            float angle = 0;
            AndroidAudioDevice device = new AndroidAudioDevice( );
            float samples[] = new float[128];
 
            while( true )
            {
               for( int i = 0; i < samples.length; i++ )
               {
                  samples[i] = (float)FloatMath.sin(angle);
                  angle += increment;
               }
               
           	// Generate input azimuth
           	GenAzim azimGen = new GenAzim(0);
           	azimGen.run();
           	
           	// Generate input elevation
           	int el = 0;
           	
           	// Generate input distance
           	int dist = 5;
           	
           	// Instantiate Audio3D class and output datum
           	Audio3D audioModder = new Audio3D(samples, samples, azimGen.az, dist, el, false);
            IRF_DATUM finalOut = new IRF_DATUM();
               
               // Futz the input sound 
           	finalOut = audioModder.runAudio3D();
           	
           	// Return final output
              device.writeSamples( finalOut.left );
 
              // device.writeSamples( samples );
            }        	
         }
      } ).start();
   }
}