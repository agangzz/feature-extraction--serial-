#include "saliencefilter.h"

#define HALF_PEAK_NUM 32
#define QTR_PEAK_NUM 16


void saliencefilter(double *leftpeaks,int *leftindex,int *leftpeaksnum,double *filteredpeaks,int *filteredindex,int *filteredpeaksnum,int framenumber)
{
    int length=0;
    double sum=0;
    double mean=0;
    double std=0;

    double tdelta=0.9;
    double threshold;

    int *orignalleftpeaksnum;

    orignalleftpeaksnum=(int *)malloc(sizeof(int)*framenumber);
    memcpy(orignalleftpeaksnum,leftpeaksnum,sizeof(int)*framenumber);

    for(int i=0;i<framenumber;i++)//mean
    {
        for(int j=0;j<leftpeaksnum[i];j++)
        {
            sum+=leftpeaks[i*QTR_PEAK_NUM+j];
        }
        length+=leftpeaksnum[i];
    }

    mean=sum/length;

    for(int i=0;i<framenumber;i++)//std
    {
        for(int j=0;j<leftpeaksnum[i];j++)
        {
            std+=(leftpeaks[i*QTR_PEAK_NUM+j]-mean)*(leftpeaks[i*QTR_PEAK_NUM+j]-mean);
        }
    }

    std=sqrt(std/length);


    threshold=mean-tdelta*std;

    //printf("mean is %lf, length is %d,std is %lf,threshold is %lf\n",mean,length,std,threshold);

    for(int i=0;i<framenumber;i++)//filter the salience peaks in left part
    {
        int leftnum=leftpeaksnum[i];
        for(int j=0;j<leftpeaksnum[i];j++)
        {
            double peaks=leftpeaks[i*QTR_PEAK_NUM+j];
            int index=leftindex[i*QTR_PEAK_NUM+j];
            int filterednum=filteredpeaksnum[i];

            if(peaks<threshold)
            {
                filteredpeaks[i*HALF_PEAK_NUM+filterednum]=peaks;
                filteredindex[i*HALF_PEAK_NUM+filterednum++]=index;
                filteredpeaksnum[i]=filterednum;

                leftpeaks[i*QTR_PEAK_NUM+j]=0;
                leftindex[i*QTR_PEAK_NUM+j]=0;
                leftnum--;
                
            }
        }
        leftpeaksnum[i]=leftnum;
    }


    for(int i=0;i<framenumber;i++)//drop the zero bubble   
    {
        int j=0;
        int meeted=0;
        if(leftpeaksnum[i]==orignalleftpeaksnum[i])//no bubble
        {
            continue;
        }

        while(meeted<leftpeaksnum[i]&&j<orignalleftpeaksnum[i])
        {
            if(leftpeaks[i*QTR_PEAK_NUM+j]!=0)
            {
                if(j!=meeted)
                {
                    leftpeaks[i*QTR_PEAK_NUM+meeted]=leftpeaks[i*QTR_PEAK_NUM+j];
                    leftpeaks[i*QTR_PEAK_NUM+j]=0;

                    leftindex[i*QTR_PEAK_NUM+meeted++]=leftindex[i*QTR_PEAK_NUM+j];
                    leftindex[i*QTR_PEAK_NUM+j]=0;
                }
                else
                {
                    meeted++;
                }

            }
            
            j++;
        }
    }    

    free(orignalleftpeaksnum);

    /*
    FILE *salience_fp=fopen("./output/filteredsalience.txt","wb");
    if(salience_fp!=NULL)
    {
        for(int i=0;i<framenumber;i++)
        {
            fprintf(salience_fp,"frame %d left %d peaks,filtered is %d\n",i+1,leftpeaksnum[i],filteredpeaksnum[i]);
            for(int j=i*QTR_PEAK_NUM;j<i*QTR_PEAK_NUM+QTR_PEAK_NUM;j++)
            {
                fprintf(salience_fp,"%f\t(%d)\t: %f\t(%d)\n",leftpeaks[j],leftindex[j],filteredpeaks[j+i*QTR_PEAK_NUM],filteredindex[j+i*QTR_PEAK_NUM]);
            }
        }
    }
    fclose(salience_fp);
    */
}
