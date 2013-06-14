#include "saliencefunction.h"
#include "findpeaks.h"
#include "util.h"

#define BIN_NUM 480
#define PEAK_NUM 64
#define HALF_PEAK_NUM 32
#define QTR_PEAK_NUM 16
#define NH 8
#define ALPHA 0.8


void Saliencefunc(double *peaks,int *index,int *peaksnum,int N,int sampleRate,int framenumber,double **leftpeaks_p,int **leftindex_p,int **leftpeaksnum_p,double **filteredpeaks_p,int **filteredindex_p,int **filteredpeaksnum_p)
{
    double *saliencebins;

    double *speaks;
    int *sindex;
    int *speaksnum;

    double *maxpeaks;

    double *leftpeaks;
    int *leftindex;
    int *leftpeaksnum;

    double *left_peaks;
    int *left_index;
    int *left_peaksnum;

    double *filtered_peaks;
    int *filtered_index;
    int *filtered_peaksnum;

    double tfirst=0.25;
    double tplus=0.9;

    saliencebins=(double *)malloc(sizeof(double)*BIN_NUM*framenumber);

    for(int i=0;i<framenumber;i++)
    {
        for(int j=0;j<BIN_NUM;j++)
        {
            saliencebins[i*BIN_NUM+j]=salience(peaks,index,i,peaksnum[i],N,sampleRate,j+1);
        }
    }

    speaks=(double *)malloc(sizeof(double)*PEAK_NUM*framenumber);
    sindex=(int *)malloc(sizeof(int)*PEAK_NUM*framenumber);
    speaksnum=(int *)malloc(sizeof(int)*framenumber);

    maxpeaks=(double *)malloc(sizeof(double)*framenumber);

    leftpeaks=(double *)malloc(sizeof(double)*HALF_PEAK_NUM*framenumber);
    leftindex=(int *)malloc(sizeof(int)*HALF_PEAK_NUM*framenumber);
    leftpeaksnum=(int *)malloc(sizeof(int)*framenumber);

    left_peaks=(double *)malloc(sizeof(double)*QTR_PEAK_NUM*framenumber);
    left_index=(int *)malloc(sizeof(int)*QTR_PEAK_NUM*framenumber);
    left_peaksnum=(int *)malloc(sizeof(int)*framenumber);

    filtered_peaks=(double *)malloc(sizeof(double)*HALF_PEAK_NUM*framenumber);
    filtered_index=(int *)malloc(sizeof(int)*HALF_PEAK_NUM*framenumber);
    filtered_peaksnum=(int *)malloc(sizeof(int)*framenumber);

    findpeaks(saliencebins,BIN_NUM,framenumber,speaks,sindex,speaksnum,PEAK_NUM);

    max(speaks,speaksnum,PEAK_NUM,framenumber,maxpeaks);

    biggerfilter(speaks,sindex,maxpeaks,speaksnum,PEAK_NUM,framenumber,leftpeaks,leftindex,leftpeaksnum,HALF_PEAK_NUM,tfirst);

    spilt(leftpeaks,leftindex,leftpeaksnum,maxpeaks,HALF_PEAK_NUM,framenumber,left_peaks,left_index,left_peaksnum,QTR_PEAK_NUM,filtered_peaks,filtered_index,filtered_peaksnum,HALF_PEAK_NUM,tplus);

    /*
    for(int i=0;i<framenumber;i++)
    {
        printf("frame %d,leftpeaksnum %d,left %d,filtered %d\n",i+1,leftpeaksnum[i],left_peaksnum[i],filtered_peaksnum[i]);
    }
    */


    free(saliencebins);

    free(speaks);
    free(sindex);
    free(speaksnum);

    free(maxpeaks);

    free(leftpeaks);
    free(leftindex);
    free(leftpeaksnum);

    *leftpeaks_p=left_peaks;
    *leftindex_p=left_index;
    *leftpeaksnum_p=left_peaksnum;


    *filteredpeaks_p=filtered_peaks;
    *filteredindex_p=filtered_index;
    *filteredpeaksnum_p=filtered_peaksnum;
}

double salience(double *peaks,int *index,int frame,int peaks_num,int N,int sampleRate,int b)
{
    double ss=0;
    double rate=sampleRate*1.0/N;
    int framenum=frame*HALF_PEAK_NUM;
    for(int h=1;h<=NH;h++)
    {
        for(int p=0;p<peaks_num;p++)
        {
            ss+=weight(b,h,index[framenum+p]*rate)*peaks[framenum+p];
        }
    } 

    return ss;
}

double weight(int b,int harmonic,double frequency)
{
    double delta=fabs(bin(frequency/harmonic)-b)/10;

    if(delta<=1)
    {
        double c=cos(delta*1.5708);
        return c*c*pow(ALPHA,harmonic-1);
    }
    else
        return 0;
}

int bin(double frequency)
{
    return (int)floor(120*log2(frequency/90)+1);
}

void biggerfilter(double *peaks,int *index,double *maxpeaks,int *length,int framelength,int batch,double *leftpeaks,int *leftindex,int *peaks_num,int maxnum,double tfirst)
{
    for(int i=0;i<batch;i++)
    {
        int peaksnum=0;
        for(int j=0;j<length[i];j++)
        {
            if(peaks[i*framelength+j]>maxpeaks[i]*tfirst&&peaksnum<maxnum)
            {
                leftpeaks[i*maxnum+peaksnum]=peaks[i*framelength+j];
                leftindex[i*maxnum+peaksnum++]=index[i*framelength+j];
            }
            if(peaksnum>=maxnum)break;
        }
        peaks_num[i]=peaksnum;
    }
}

void spilt(double *peaks,int *index,int *peaksnum,double *maxpeaks,int framelength,int framenumber,double *leftpeaks,int *leftindex,int *leftpeaksnum,int leftmaxnum,double *filteredpeaks,int *filteredindex,int *filteredpeaksnum,int filteredmaxnum,double tplus)
{
    for(int i=0;i<framenumber;i++)
    {
        int lnum=0;
        int fnum=0;
        for(int j=0;j<peaksnum[i];j++)
        {
            if(peaks[i*framelength+j]>maxpeaks[i]*tplus&&lnum<leftmaxnum)
            {
                leftpeaks[i*leftmaxnum+lnum]=peaks[i*framelength+j];
                leftindex[i*leftmaxnum+lnum++]=index[i*framelength+j];
            }
            else
            {
                filteredpeaks[i*filteredmaxnum+fnum]=peaks[i*framelength+j];
                filteredindex[i*filteredmaxnum+fnum++]=index[i*framelength+j];
            }

        }

        leftpeaksnum[i]=lnum;
        filteredpeaksnum[i]=fnum;
    }
}
