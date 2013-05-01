#include "irf_datum.h"

int get_nfaz(int el_index);
int get_el_index(double elev);
void get_indices(double elev, double azim, int *p_el, int *p_az, int *p_flip);
int index_to_elev(int el_index);
int index_to_azim(int el_index, int az_index);
char *irf_name(int el_index, int az_index);
void read_irf(int el_index, int az_index, IRF_DATUM *hd);
// read_irfs loads full set of irfs into irf_data[][]
void read_irfs(void); 
void free_irfs(void);
// get_irf reads irfs from irf_data[][] into p_left and p_right
void get_irf(double elev, double azim, double **p_left, double **p_right); 
