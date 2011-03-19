#include "stdio.h"

#include "sigextract.h"
#include "video_decode.h"

int main(int argc, char **argv)
{
  ticm = allocTICM();
  dct = allocDCT();

  // Check commandline argument
  if(argc != 2)
  {
    printf("Usage: %s <video_file>\n", argv[0]);
    return 0;
  }
  decodeVideo(argv[1], &processFrame);
  printf("Signature = (%d, %d, %d)\n", T[SIMPLE], T[MODERATE], T[COMPLEX]);
  int total = T[SIMPLE] + T[MODERATE] + T[COMPLEX];
  printf("Normalised Signature = (%f, %f, %f)\n", T[SIMPLE] / (float)total,
                                                  T[MODERATE] / (float)total,
                                                  T[COMPLEX] / (float)total
        );

  deallocTICM(ticm);
  deallocDCT(dct);
  return 0;
}
