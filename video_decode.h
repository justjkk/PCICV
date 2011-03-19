#ifndef VIDEO_DECODE_H
#define VIDEO_DECODE_H

void decodeVideo(const char *filename, void (*processFrame)(double **));

#endif
