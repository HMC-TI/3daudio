

void runAudio3D(float *inputL, float *inputR, int az, double dist, int elev, int chan, double *outputL, double *outputR);
void init();
void createNewOutputs();
void createOldConvolveAndCrossfade();
void loadIRFs();
void crossfade(double *newIn, double *oldIn, double *out);
