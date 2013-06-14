#include "util.h"

double difftime_ms(struct timeval finish,struct timeval start)
{
    return (double)(finish.tv_sec-start.tv_sec)*1000+(double)(finish.tv_usec-start.tv_usec)*1.0/1000;
}

double sum(double *data,int length)
{
    double s=0;
    for(int i=0;i<length;i++)
    {
        s+=data[i];
    }
    return s;
}

double * hann(int length)
{
    double * data;
    data=(double*)malloc(sizeof(double)*length);
    for(int i=0;i<length;i++)
    {
        data[i]=(double)0.5*(1-cos(6.283186*i/(length-1)));
    }
    return data;
}

double max(double *data,int length)
{
    double m=0;
    for(int i=0;i<length;i++)
    {
        if(m<data[i])
        {
            m=data[i];
        }
    }

    return m;
}

void max(double *data,int *length,int framelength,int batch,double *m)
{
    for(int i=0;i<batch;i++)
    {
        double mm=0;
        for(int j=0;j<length[i];j++)
        {
            if(mm<data[i*framelength+j])
            {
                mm=data[i*framelength+j];
            }
        }
        m[i]=mm;
    }
}

