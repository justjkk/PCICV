#ifndef SIGEXTRACT_H
#define SIGEXTRACT_H

int ****ticm;
double **dct;
int T[3];
  
enum {SIMPLE, MODERATE, COMPLEX};

int ****allocTICM();
double **allocDCT();
void deallocTICM(int ****ticm);
void deallocDCT(double **dct);
void processFrame(double **ip);//, double **dct, int ****ticm, int *T);

#endif
