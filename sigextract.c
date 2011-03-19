#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "dct.h"
#include "sigextract.h"

int ****allocTICM()
{
  // Allocate Global variables
  
  //Threshold Inverse Convolution Matrix
  int ****ticm = (int ****)malloc(8 * sizeof(int ***));
  for(int i = 0; i < 8; i++)
  {
    ticm[i] = (int ***)malloc(8 * sizeof(int **));
    for(int j = 0; j < 8; j++)
    {
      ticm[i][j] = (int **)malloc(8 * sizeof(int *));
      for(int u = 0; u < 8; u++)
      {
        ticm[i][j][u] = (int *)malloc(8 * sizeof(int));
      }
    }
  }
  populateTICM(ticm);
  return ticm;
}

double **allocDCT()
{
  // Global array for holding DCT matrix
  double **dct = (double **)malloc(8 * sizeof(double *));
  for(int i = 0; i < 8; i++)
  {
    dct[i] = (double *)malloc(8 * sizeof(double));
  }
  return dct;
}

void deallocTICM(int ****ticm)
{
  free(ticm);
}

void deallocDCT(double **dct)
{
  free(dct);
}

void processFrame(double **ip)//, double **dct, int ****ticm, int *T)
{
  DCT(ip, dct);

  // Find Texture for every input block
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      int count = 0;
      for(int u = 0; u < 8; u++)
      {
        for(int v = 0; v < 8; v++)
        {
          if(ticm[i][j][u][v] == 0)
            continue;
          if(fabs(dct[u][v]) >= DCT_THRESHOLD)
            count++;
        }
      }
      if(count < 25)
        T[SIMPLE]++;
      else if(count < 30)
        T[MODERATE]++;
      else //if(count < 62)
        T[COMPLEX]++;
    }
  }
}
