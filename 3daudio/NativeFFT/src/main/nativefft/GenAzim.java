package main.nativefft;

public class GenAzim extends Thread{
	public int az;
	
	GenAzim(int az) {
		this.az = az;
	}
	
	public void run() {
		//while (true){
			az = (az + 1) % 360; // Azimuth will always be within 0 and 360
			/*System.out.format("Azimuth is %d%n", az);
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}*/
		//}
	}
}
