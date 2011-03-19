#ifndef DCT_H
#define DCT_H

#define PI 3.1415926535897931
#define DCT_THRESHOLD 0.01

double C (int x);
void DCT(double **ip, double **op);
void populateTICM(int ****ticm);

#endif
