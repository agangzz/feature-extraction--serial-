#ifndef _SALIENCEFILTER_H_
#define _SALIENCEFILTER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/*
 * filter the salience in the left part using the salience mean and salience std
 *
 * Input
 * ------------------------------------------
 * leftpeaks: the left peaks of every frame, with a fixed length of every frame: QTR_PEAK_NUM
 * leftindex: the left peaks index of every frame, with a fixed length of every frame: QTR_PEAK_NUM
 * leftpeaksnum: the left peaks num of every frame
 * filteredpeaks: the filtered peaks of every frame, with a fixed length of every frame: HALF_PEAK_NUM
 * filteredindex: the filtered peaks index of every frame, with a fixed length of every frame: HALF_PEAK_NUM
 * filteredpeaksnum: the filtered peaks num of every frame
 * framenumber: how many frames in all
 */
void saliencefilter(double *leftpeaks,int *leftindex,int *leftpeaksnum,double *filteredpeaks,int *filteredindex,int *filteredpeaksnum,int framenumber);

#endif
