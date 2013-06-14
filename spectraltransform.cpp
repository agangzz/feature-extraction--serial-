#include "spectraltransform.h"
#include "findpeaks.h"
#include "util.h"

#define MAX_PEAK_NUM 128
#define PEAK_NUM 64
#define HALF_PEAK_NUM 32
#define QTR_PEAK_NUM 16

void spectraltransform(double * wavdata,int datalength,unsigned long sampleRate,int N,int framenumber,int framelength,int hoplength,double **peaks_p,int **index_p,int **peaksnum_p)
{
    double *hann_data;
    double hann_sum;

    fftw_plan plan;
    double *frame;
    fftw_complex *fft_result;

    double *p;
    double *fft_abs_result;

    int edgelength;
    int minlength;

    double *peaks;
    int *index;

    double *left_peaks;
    int *left_index;

    double *upeaks;
    int *uindex;
    int *upeaksnum;

    int peaksnum;
    double maxpeak;

    float filterratio=0.2;


    edgelength=(int)ceil(5000*N*1.0/sampleRate);
    minlength=(int)fmin((float)(N/2+1),(float)edgelength);

    hann_data=hann(framelength);
    hann_sum=sum(hann_data,framelength);

    peaks=(double *)malloc(sizeof(double)*MAX_PEAK_NUM);
    index=(int *)malloc(sizeof(int)*MAX_PEAK_NUM);

    left_peaks=(double *)malloc(sizeof(double)*PEAK_NUM);
    left_index=(int *)malloc(sizeof(int)*PEAK_NUM);

    upeaks=(double *)malloc(sizeof(double)*HALF_PEAK_NUM*framenumber);
    uindex=(int *)malloc(sizeof(int)*HALF_PEAK_NUM*framenumber);
    upeaksnum=(int*)malloc(sizeof(int)*framenumber);

    frame=(double*)malloc(sizeof(double)*N);
    memset(frame,0,sizeof(double)*N);
    fft_result=fftw_alloc_complex(N/2+1);
    fft_abs_result=(double*)malloc(sizeof(double)*(N/2+1));

    plan=fftw_plan_dft_r2c_1d(N,frame,fft_result,FFTW_ESTIMATE);//

    for(int i=0;i<100;i++)
    {
        printf("%lf\n",wavdata[i]);
    }
    p=wavdata;
    for(int i=0;i<framenumber;i++)
    {
        memcpy(frame,p,framelength*sizeof(double));//!!!don't forget multiple sizeof()!!!

        dot_product(frame,hann_data,framelength);

        fftw_execute(plan);

        fft_abs(fft_result,fft_abs_result,N/2+1);

        divide(fft_abs_result,hann_sum,N/2+1);

        peaksnum=findpeaks(fft_abs_result,minlength,peaks,index,MAX_PEAK_NUM);

        if(i==199)
        {
            FILE *fp=fopen("./fft_result","w");
            for(int j=0;j<framelength;j++)
            {
                fprintf(fp,"%lf\n",hann_data[j]);
            }
            fclose(fp);
            for(int j=0;j<MAX_PEAK_NUM;j++)
            {
                printf("%d(%lf)\n",index[j],peaks[j]);
            }
        }

        maxpeak=max(peaks,peaksnum);

        peaksnum=biggerfilter(peaks,index,peaksnum,left_peaks,left_index,PEAK_NUM,maxpeak*filterratio);

        morefilter(left_peaks,left_index,peaksnum,i,upeaks,uindex,upeaksnum,HALF_PEAK_NUM,maxpeak*filterratio);

        p+=hoplength;
    }

    fftw_destroy_plan(plan);
    fftw_free(fft_result);

    free(hann_data);
    free(frame);
    free(fft_abs_result);
    free(peaks);
    free(index);
    free(left_peaks);
    free(left_index);

    *peaks_p=upeaks;
    *index_p=uindex;
    *peaksnum_p=upeaksnum;

}

void dot_product(double *frame,double *data,int length)
{
    for(int i=0;i<length;i++)
    {
        frame[i]*=data[i];
    }
}

void fft_abs(fftw_complex *data,double *fft_abs_result,int length)
{
    for(int i=0;i<length;i++)
    {
        fft_abs_result[i]=sqrt(data[i][0]*data[i][0]+data[i][1]*data[i][1]);
    }
}

void divide(double *data,double ws,int length)
{
    for(int i=0;i<length;i++)
    {
        data[i]/=(ws/2);
    }
}

void printresult(double *data,int length,int frame)
{
    printf("frame %d:\n",frame);
    for(int i=0;i<length;i++)
    {
        printf("frame %d,%d: %lf\n",frame,i,data[i]);
    }
}

int biggerfilter(double *peaks,int *index,int length,double *left_peaks,int *left_index,int maxnum,double threshold)
{
    int peaksnum=0;
    for(int i=0;i<length;i++)
    {
        if(peaks[i]>threshold&&peaksnum<maxnum)
        {
            left_peaks[peaksnum]=peaks[i];
            left_index[peaksnum++]=index[i];
        }
        if(peaksnum>=maxnum)break;
    }

    return peaksnum;

}

void morefilter(double *peaks,int *index,int length,int frame,double *left_peaks,int *left_index,int *left_peaksnum,int maxnum,double threshold)
{
    if(length>16)
    {
        int peaksnum=0;
        threshold*=(length*1.0/16);
        for(int i=0;i<length;i++)
        {
            if(peaks[i]>threshold&&peaksnum<maxnum)
            {
                left_peaks[frame*HALF_PEAK_NUM+peaksnum]=peaks[i];
                left_index[frame*HALF_PEAK_NUM+peaksnum++]=index[i];
            }
            if(peaksnum>=maxnum)break;
        }
        left_peaksnum[frame]=peaksnum;

    }
    else
    {
        for(int i=0;i<length;i++)
        {
            left_peaks[frame*HALF_PEAK_NUM+i]=peaks[i];
            left_index[frame*HALF_PEAK_NUM+i]=index[i];
        }
        left_peaksnum[frame]=length;
    }
}

