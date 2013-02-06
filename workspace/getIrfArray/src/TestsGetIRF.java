

import java.util.Arrays;

/**
 * @author acard
 * This class should test the GetIRF class and associated functions. See TestsAudio3D for 
 * generic setup of a testing class
 */
public class TestsGetIRF {
	
	//bounds on elevation values
	static final int MIN_ELEV = -40;
	static final int MAX_ELEV = 90;
	static final int ELEV_INC = 10;
	static final int N_ELEV = (((MAX_ELEV - MIN_ELEV) / ELEV_INC) + 1);
	
	public static GetIRF getIrf = new GetIRF();

	public static void main(String[] args) {
		System.out.println("public static IRF_DATUM[][] irf_data = new IRF_DATUM[14][72]");
		
		//load IRFs
		getIrf.read_irfs();
		printIrfs();
		
		//testElIndex();
		
		//print IRF

	}
	
	public static void printIrfs() {
		//loops through # of different indices (N_ELEV)
		int nfaz;
		//number of azimuth data points per elevation (-40 to 90, increments of 10)
		final int[] elev_data = {56, 60, 72, 72, 72, 72, 72, 60, 56, 45, 36, 24, 12, 1};
		
		for (int el_index = 0; el_index < N_ELEV; el_index++) 
		{
			//uses elev_data to find how many associated azimuths there are
			nfaz = (elev_data[el_index] / 2) + 1;

			//loops through all azimuths
			for (int az_index = 0; az_index < nfaz; az_index++) 
			{
				//actually load IRFs
				float[][] irf = getIrf.get_irf(el_index, az_index);
				System.out.format("irf_data[%d][%d].left = new float[]{", el_index, az_index);
				for (int i = 0; i < irf[0].length; i++) {
					
					System.out.format("%ff, ", irf[0][i]);
				}
				System.out.format("};%n");
				System.out.format("irf_data[%d][%d].right = new float[]{", el_index, az_index);
				for (int i = 0; i < irf[0].length; i++) {
					System.out.format("%ff, ", irf[1][i]);
				}
				System.out.format("};%n");
				
				//System.out.println("irf_data["+el_index+"]["+az_index+"].left = "+Arrays.toString(irf[0])+";");
				//System.out.println("irf_data["+el_index+"]["+az_index+"].right = "+Arrays.toString(irf[1])+";");
			}
		}
		/*float[][] irf = getIrf.get_irf(0, 0);
		float[] right = irf[1];
		float[] left = irf[0];
		System.out.format("Printing irfs for e:%d and a:%d%n", getIrf.cur_el_index, getIrf.cur_az_index);
		System.out.println(Arrays.toString(getIrf.irf_data[0][0].right));
		System.out.println("left " + Arrays.toString(left) + " right " + Arrays.toString(right));*/
	}
	
	public static void testElIndex() {
		for (int i = 0; i < 50; i++) {
			int el_index = getIrf.get_el_index(i);
			System.out.format("For an elevation of %d cur_el_index is %d%n", i, el_index);
		}
	}
}
