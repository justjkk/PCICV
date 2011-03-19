#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "dct.h"

// Constant co-efficient
double C (int x)
{
  if (x == 0)
    return 0.70710678118654757;	// 1 / sqrt(2)
  return 1.0;
}

// Discrete Cosine Transformation
void DCT (double **ip, double **op)
{
  for (int u = 0; u < 8; u++)
  {
    for (int v = 0; v < 8; v++)
    {
      op[u][v] = 0;
      for (int i = 0; i < 8; i++)
      {
        for (int j = 0; j < 8; j++)
        {
          op[u][v] += ip[i][j] * cosf ((2 * i + 1) * u * PI / 16) * \
                      cosf ((2 * j + 1) * v * PI / 16);
        }
      }
      op[u][v] *= C (u) * C (v) / 4.0;
    }
  }
}

void populateTICM(int ****ticm)
{
  for (int u = 0; u < 8; u++)
  {
    for (int v = 0; v < 8; v++)
    {
      for (int i = 0; i < 8; i++)
      {
        for (int j = 0; j < 8; j++)
        {
          if ((u == 0 && v == 0) || (u == 0 && v == 1) || (u == 1 && v == 0))
          {
            ticm[i][j][u][v] = 0;	// Eliminating DC & Low frequency Coefficients
          }
          else
          {
            double value = cos ((2 * i + 1) * u * PI / 16) * \
                           cos ((2 * j + 1) * v * PI / 16);
            if (value > 0)
              ticm[i][j][u][v] = 1;
            else
              ticm[i][j][u][v] = 0;
          }
        }
      }
    }
  }
}
