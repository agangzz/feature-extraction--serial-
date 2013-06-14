#ifndef _SALIENCEFUNCTION_H_
#define _SALIENCEFUNCTION_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/*
 * cal the salience of every frame, and return the salient peaks of the salience
 *
 * Input
 * ------------------------------------------
 * peaks: the spectral peaks of every frame, with a fixed length of every frame: PEAK_NUM
 * index: the spectral peaks index of every frame, with a fixed length of every frame: PEAK_NUM
 * peaksnum: the peaks num of every frame
 * N: the fft length
 * sampleRate: the sample rate of the wav file
 * framenumber: how many frame in all
 * leftpeaks_p: the pointer points to the left peaks result. space is allocated in this function. so just transfer a NULL pointer
 * leftindex_p: the pointer points to the left peaks index result. space is allocated in this function. so just transfer a NULL pointer
 * leftpeaksnum_p: the pointer points to the left peaks num result. space is allocated in this function. so just transfer a NULL pointer
 * filteredpeaks_p: the pointer points to the filtered peaks result. space is allocated in this function. so just transfer a NULL pointer
 * filteredindex_p: the pointer points to the filtered peaks index result. space is allocated in this function. so just transfer a NULL pointer
 * filteredpeaksnum_p: the pointer points to the filtered peaks num result. space is allocated in this function. so just transfer a NULL pointer
 */
void Saliencefunc(double *peaks,int *index,int *peaksnum,int N,int sampleRate,int framenumber,double **leftpeaks_p,int **leftindex_p,int **leftpeaksnum_p,double **filteredpeaks_p,int **filteredindex_p,int **filteredpeaksnum_p);


/*
 * cal the salience of the given frame and bin num
 *
 * Input
 * ------------------------------------------
 * peaks: the spectral peaks, namely amplitude
 * index: the spectral index, can changed to frequency
 * frame: the frame number
 * peaks_num: the spectral peaks number of the given frame
 * int N: the fft length
 * sampleRate: the sample rate of the wav file
 * b: the bin number
 */
double salience(double *peaks,int *index,int frame,int peaks_num,int N,int sampleRate,int b);


/*
 * cal the weight of given frequency
 *
 * Input
 * ------------------------------------------
 * b: the bin num
 * harmonic: the harmonic number
 * frequency: the given frequency
 */
double weight(int b,int harmonic,double frequency);


/*
 * change a frequency to bin number
 *
 * Input
 * ------------------------------------------
 * frequency: the given frequency
 */
int bin(double frequency);


/*
 * filter the salience peaks, only save the peaks bigger than the threshold
 *
 * Input
 * ------------------------------------------
 * peaks: the salience peaks of every frame, with a fixed length of every frame: PEAK_NUM
 * index: the salience peaks index of every frame, with a fixed length of every frame: PEAK_NUM
 * maxpeaks: the max peaks in every frame
 * length: the peaks number in every frame
 * framelength: the max fixed peaks num in every frame, namely PEAK_NUM
 * batch: how many frames in all
 * leftpeaks: the left peaks after filter in every frame
 * leftindex: the left peaks index after filter in every frame
 * peaks_num: the left peaks number in every frame
 * maxnm: the maximum num of the left peaks, namely HALF_PEAK_NUM
 * tfirst: the first filter threshold
 */
void biggerfilter(double *peaks,int *index,double *maxpeaks,int *length,int framelength,int batch,double *leftpeaks,int *leftindex,int *peaks_num,int maxnum,double tfirst);


/*
 * spilt the salience peaks to two parts: the left and the filtered part.
 *
 * Input
 * ------------------------------------------
 * peaks: the salience peaks before the spilt, with a fixed length of every frame: HALF_PEAK_NUM
 * index: the salience peaks index before the spilt, with a fixed length of every frame: HALF_PEAK_NUM
 * peaksnum: the peaks num of every frame
 * maxpeaks: the max peaks in every frame
 * framelength: the max fixed peaks num in every frame, namely HALF_PEAK_NUM
 * framenumber: how many frames in all
 *
 * left*: the left part
 * filtered: the filtered part
 */
void spilt(double *peaks,int *index,int *peaksnum,double *maxpeaks,int framelength,int framenumber,double *leftpeaks,int *leftindex,int *leftpeaksnum,int leftmaxnum,double *filteredpeaks,int *filteredindex,int *filteredpeaksnum,int filteredmaxnum,double tplus);


#endif
