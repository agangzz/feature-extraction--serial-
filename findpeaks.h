#ifndef _FINDPEAKS_H_
#define _FINDPEAKS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/*
 * find the peak in an array.
 *
 * Input
 * ------------------------------------------
 * data: the data array
 * length: the array length
 * peaks: the peaks found in the array
 * index: the index of the found peaks in the array
 * maxnum: the maxinum num of the peaks allowed
 *
 * Output
 * ------------------------------------------
 * return the peaks num
 */
int findpeaks(double *data,int length,double *peaks,int *index,int maxnum);

/*
 * find the peak in arrays.
 *
 * Input
 * ------------------------------------------
 * data: the data array
 * length: the array length
 * batch: the array number
 * peaks: the peaks found in the array
 * index: the index of the found peaks in the array
 * peaks_num: the peaks num of every array
 * maxnum: the maxinum num of the peaks allowed
 */
void findpeaks(double *data,int length,int batch,double *peaks,int *index,int *peaks_num,int maxnum);


#endif
