all: melodycontour

#complier
CC = nvcc

#include path
CFLAGS+= -O3 -pg

OUTPUT_OPTION+= -O3

#lib path,this variable is first defined in ~/.bashrc file as an environment variable, the same as cflags.
LDFLAGS+=  -lm  -lfftw3

melodycontour:   spectraltransform.o main.o melodycontour.o characteristic.o ReadWav.o  util.o findpeaks.o  saliencefunction.o saliencefilter.o generatecontour.o contour.o filtercontour.o 
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^


clean:
	$(RM) *.o *.s *.i
