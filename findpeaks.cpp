
#include "findpeaks.h"



int findpeaks(double *data,int length,double *peaks,int *index,int maxnum)
{
    int peaksnum=0;
    for(int i=1;i<length-1;i++)
    {
        if(peaksnum<maxnum&&data[i]>data[i-1]&&data[i]>data[i+1])
        {
            peaks[peaksnum]=data[i];
            index[peaksnum++]=i;
        }
        if(peaksnum>=maxnum)break;
    }

    return peaksnum;
}

void findpeaks(double *data,int length,int batch,double *peaks,int *index,int *peaks_num,int maxnum)
{
    for(int i=0;i<batch;i++)
    {
        int peaksnum=0;
        for(int j=1;j<length-1;j++)
        {
            if(peaksnum<maxnum&&data[i*length+j]>data[i*length+j-1]&&data[i*length+j]>data[i*length+j+1])
            {
                peaks[i*maxnum+peaksnum]=data[i*length+j];
                index[i*maxnum+peaksnum++]=j+1;
            }
            if(peaksnum>=maxnum)break;
        }
        peaks_num[i]=peaksnum;
    }
}


