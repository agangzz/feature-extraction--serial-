/*
 *this head file spectral transform the wav data, and get the peaks of every frame
 */

#ifndef _SPECTRALTRANSFORM_H_
#define _SPECTRALTRANSFORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <fftw3.h>

/*
 * spectral transform the wav data, and get the peaks of every frame
 * 
 * Input
 * ----------------------------------------
 * wavdata: original wav data extracted from wav file
 * datalength: the length of wavdata
 * sampleRate: sample rate of the wav file
 * N: the length of fft
 * framenumber: the frame number for fft of wavdata
 * framelength: the length of every frame
 * hoplength: the stride of frame
 * peaks_p: the pointer points to the spectral peaks. the space is allcoated in this function, so just transfer a NULL pointer
 * index_p: the pointer points to the spectral peaks index. the space is allcoated in this function, so just transfer a NULL pointer
 * peaksnum_p: the pointer points to the spectral peaks num. the space is allcoated in this function, so just transfer a NULL pointer
 */
void spectraltransform(double * wavdata,int datalength,unsigned long sampleRate,int N,int framenumber,int framelength,int hoplength,double **peaks_p,int **index_p,int **peaksnum_p);


/*
 * dot product the frame with the hanning data
 *
 * Input
 * ------------------------------------------
 * frame: the frame data
 * data: the hanning data
 * length: the frame length
 */
void dot_product(double *frame,double *data,int length);


/*
 * cal the fft result through abs the real part and the imaginary part of the complex
 *
 * Input
 * ------------------------------------------
 * data: the fft data with real and imaginary parts
 * fft_abs_resultL the fft result
 * length: the length of fft
 */
void fft_abs(fftw_complex *data,double *fft_abs_result,int length);


/*
 * divide the data with ws/2
 *
 * Input
 * ------------------------------------------
 * data: the data
 * ws: a double vale
 * length: the data length
 */
void divide(double *data,double ws,int length);


/*
 * print the data in a given frame
 *
 * Input
 * ------------------------------------------
 * data: the data
 * length: the data length
 * frame: the frame number
 */
void printresult(double *data,int length,int frame);


/*
 * filter the spectral peaks
 *
 * Input
 * ------------------------------------------
 * peaks: the spectral peaks before filter, with a fixed length : MAX_PEAK_LENGTH
 * index: the spectral peaks index before filter, with a fixed length : MAX_PEAK_LENGTH
 * length: the peaks number
 *
 * left*: the left peaks 
 * maxnum: the allowed maximum peaks num, namely PEAK_NUM
 * threshold: the filter threshold
 */
int biggerfilter(double *peaks,int *index,int length,double *left_peaks,int *left_index,int maxnum,double threshold);


/*
 * more filter the peaks
 *
 *
 */
void morefilter(double *peaks,int *index,int length,int frame,double *left_peaks,int *left_index,int *left_peaksnum,int maxnum,double threshold);


#endif
