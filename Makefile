CC = gcc
LIBS = -lm
FF_LIBS = -lavcodec -lavformat -lswscale
CFLAGS = -g -std=c99
GTK_CONFIG = -export-dynamic `pkg-config --cflags --libs gtk+-2.0 gthread-2.0`

all: sigextract sigextract_nogui

sigextract: dct.o video_decode.o sigextract.o sigextract_gui.c
	$(CC) $(CFLAGS) $(FF_LIBS) $(LIBS) $(GTK_CONFIG) sigextract_gui.c sigextract.o dct.o video_decode.o -o $@

sigextract_nogui: dct.o video_decode.o sigextract.o sigextract_cui.c
	$(CC) $(CFLAGS) $(FF_LIBS) $(LIBS) sigextract_cui.c sigextract.o dct.o video_decode.o $(CFLAGS) $(LIBS) -o $@

sigextract.o: sigextract.h
dct.o: dct.h
video_decode.o: video_decode.h

clean:
	-rm -f *.o *~ sigextract sigextract_nogui
