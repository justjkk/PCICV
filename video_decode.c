#include<libswscale/swscale.h>
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<assert.h>

#include "video_decode.h"

void decodeVideo(const char *filename, void (*processFrame)(double **))
{
  double **ip = (double **)malloc(sizeof(double *) * 8);
  for(int i = 0; i < 8; i++)
  {
    ip[i] = (double *)malloc(sizeof(double) * 8);
  }

  int videoStream;
  AVCodecContext *pCodecCtx;
  AVCodec *pCodec;
  AVFrame *pFrame, *pFrameRGB;
  AVFormatContext *pFormatCtx;
  uint8_t *buffer;
  int numBytes;
  int frameFinished;
  AVPacket packet;
  struct SwsContext *sws_context;
  av_register_all();

  // Open video file
  assert(av_open_input_file(&pFormatCtx, filename, NULL, 0, NULL) == 0);

  // Retrieve stream information
  assert(av_find_stream_info(pFormatCtx) >= 0);

  // Find the first video stream
  videoStream = -1;
  for(int i = 0; i < pFormatCtx->nb_streams; i++)
  {
    if(pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO)
    {
      videoStream=i;
      break;
    }
  }
  assert(videoStream != -1);

  // Get a pointer to the codec context for the video stream
  pCodecCtx=pFormatCtx->streams[videoStream]->codec;

  // Find the decoder for the video stream
  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL)
  {
    fprintf(stderr, "Unsupported codec!\n");
    exit(1); // Codec not found
  }

  // Open codec
  assert(avcodec_open(pCodecCtx, pCodec) >= 0);

  // Allocate video frame
  pFrame = avcodec_alloc_frame();

  // Allocate an AVFrame structure
  //pFrameRGB = avcodec_alloc_frame();
  //assert(pFrameRGB != NULL);

  // Determine required buffer size and allocate buffer
  numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
  buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

  // Assign appropriate parts of buffer to image planes in pFrameRGB
  // Note that pFrameRGB is an AVFrame, but AVFrame is a superset
  // of AVPicture
  avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
                 pCodecCtx->width, pCodecCtx->height);
  int frameIx = 0;
  while(av_read_frame(pFormatCtx, &packet)>=0)
  {
    // Is this a packet from the video stream?
    if(packet.stream_index==videoStream)
    {
	    // Decode video frame
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

      // Did we get a video frame?
      if(frameFinished)
      {
        // Process only one out of 20 frames (approx. 1 frame per second)
        if((frameIx++) % 20 != 0)
        {
          // Free the packet that was allocated by av_read_frame
          av_free_packet(&packet);
          continue;
        }

        // First level of scaling [ from original to 32x32 through bicubic filter ]
        sws_context = sws_getContext(pCodecCtx->width, pCodecCtx->height, 0, 32, 32,
                                     PIX_FMT_GRAY16, SWS_BICUBIC, NULL, NULL, NULL);

        // Convert the image from its native format to GRAY16
        sws_scale(sws_context,
                  (const uint8_t * const*)pFrame->data,
                  pFrame->linesize,
                  0,
                  pCodecCtx->height,
                  pFrameRGB->data,
                  pFrameRGB->linesize);

        sws_freeContext(sws_context);

	      // Second level of scaling [ from 32x32 to 8x8 through fast bilinear filter ]
	      sws_context = sws_getContext(32, 32, PIX_FMT_GRAY16, 8, 8, PIX_FMT_GRAY16,
	                                   SWS_FAST_BILINEAR, NULL, NULL, NULL);
	      sws_scale(sws_context,
	                (const uint8_t * const*)pFrameRGB->data,
	                pFrameRGB->linesize,
	                0,
                  32,
	                pFrame->data,
	                pFrame->linesize);

	      sws_freeContext(sws_context);

        for(int i = 0; i < 8; i++)
        {
          for(int j = 0; j < 8; j++)
          {
            ip[i][j] = *(pFrame->data[0] + 16 * j + 2 * i) / 255.0;
          }
        }

        // Call processFrame callback with 8x8 data
        processFrame(ip);
      }
    }

    // Free the packet that was allocated by av_read_frame
    av_free_packet(&packet);
  }
}
