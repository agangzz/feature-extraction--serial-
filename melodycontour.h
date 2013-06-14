#ifndef _MELODYCONTOUR_H_
#define _MELODYCONTOUR_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


/*
 * generate the contour of a wav file
 *
 * Input
 * ------------------------------------------
 * wavfile: the wav file
 * kind: the kind of wav file:1 for polyphonic music, 2 for monophonic
 */
void melodycontour(char* wavfile,int kind);

#endif
