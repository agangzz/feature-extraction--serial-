

#include "ReadWav.h"
#include "spectraltransform.h"
#include "saliencefunction.h"
#include "saliencefilter.h"
#include "generatecontour.h"
#include "contour.h"
#include "characteristic.h"
#include "filtercontour.h"
#include "util.h"


#define N 8192
#define BLOCK_NUM 60
#define THREAD_NUM 512
#define BIN_NUM 480
#define HALF_PEAK_NUM 32
#define QTR_PEAK_NUM 16
#define FRAME_NUM 29


void melodycontour(char* wavfile,int kind)
{
    struct timeval start,finish,first,end;
    float frametime=46.44;
    float hoptime=10.0;
    float v=0.2;
   
    //wav data
    int framelength;//length of every frame
    int hoplength;
    int framenumber;//how many FFT frames in all,one frame will get a pitch
    int datalength;//=sampleRate*wavDuration. length of all frames(has stride)
    unsigned long sampleRate;

    //spectral transform result in GPU
    double *peaks;
    int *index;
    int *peaks_num;

    double* filteredpeaks;
    int* filteredindex;
    int* filteredpeaks_num;

    double* leftpeaks;
    int* leftindex;
    int* leftpeaks_num;

    FILE *fp1;

    //read wav file to get data
    CReadWav wavFile;


    gettimeofday(&first,NULL);
    gettimeofday(&start,NULL);

    wavFile.readData(wavfile);
    sampleRate=wavFile.pWav.nSamplesPerSec;
    datalength=wavFile.framelen;
    gettimeofday(&finish,NULL);
    printf("time of read wav is %f\n",difftime_ms(finish,start));

    //calculate variables about frame data
    framelength=(int)floor(sampleRate*frametime/1000);
    hoplength=(int)floor(sampleRate*hoptime/1000);
    framenumber=(datalength-framelength)/hoplength;

    printf("number is %d,frame length is %d,data length is %d,sample rate is %ld\n",framenumber,framelength,datalength,sampleRate); 
 
    gettimeofday(&start,NULL);
    spectraltransform(wavFile.data,datalength,sampleRate,N,framenumber,framelength,hoplength,&peaks,&index,&peaks_num);
    gettimeofday(&finish,NULL);
    printf("time of spectral transform is %f\n",difftime_ms(finish,start));

    fp1=fopen("./temp2","w");
    for(int i=0;i<framenumber;i++)
    {
        fprintf(fp1,"frame %d has %d peaks:\n",i+1,peaks_num[i]);
        for(int j=0;j<peaks_num[i];j++)
        {
            fprintf(fp1,"%d(%lf)\n",index[i*HALF_PEAK_NUM+j],peaks[i*HALF_PEAK_NUM+j]);
        }
    }
    fclose(fp1);
    
    gettimeofday(&start,NULL);
    Saliencefunc(peaks,index,peaks_num,N,sampleRate,framenumber,&leftpeaks,&leftindex,&leftpeaks_num,&filteredpeaks,&filteredindex,&filteredpeaks_num);
    gettimeofday(&finish,NULL);
    printf("time of salience is %f\n",difftime_ms(finish,start));

    gettimeofday(&start,NULL);
    saliencefilter(leftpeaks,leftindex,leftpeaks_num,filteredpeaks,filteredindex,filteredpeaks_num,framenumber);
    gettimeofday(&finish,NULL);
    printf("time of salience filter is %f\n",difftime_ms(finish,start));


    int contourgap=(int)(50/hoptime)+1;
    contours *cons;

    gettimeofday(&start,NULL);
    cons=generateContours(filteredpeaks,filteredindex,filteredpeaks_num,leftpeaks,leftindex,leftpeaks_num,framenumber,contourgap);
    printf("have %d contours in all\n",cons->length);

    free(filteredpeaks);
    free(filteredindex);
    free(filteredpeaks_num);
    free(leftpeaks);
    free(leftindex);
    free(leftpeaks_num);

    deleteusinglength(cons,6);
    printf("after delete using length, have %d contours in all\n",cons->length);

    gettimeofday(&finish,NULL);
    printf("time of generate contours is %f\n",difftime_ms(finish,start));

    float contours_salience_mean;
    float contours_salience_std;

    float contours_pitch_mean;
    float contours_pitch_std;

    gettimeofday(&start,NULL);
    Pitchmeans(cons);
    Pitchstds(cons);
    Saliencesums(cons);
    Saliencemeans(cons);
    Saliencestds(cons);
    gettimeofday(&finish,NULL);
    printf("time of characteristic is %f\n",difftime_ms(finish,start));



    gettimeofday(&start,NULL);
    if(kind==1)
    {
        deleteusingpitchstdandlength(cons,10,20);
        printf("after delete using pitch std and length, have %d contours in all\n",cons->length);
    }

    contours_salience_mean=Contoursaliencemean(cons);
    contours_salience_std=Contoursaliencestd(cons,contours_salience_mean);
    printf("salience mean is %f, salience std is %f,salience threshold is %f\n",contours_salience_mean,contours_salience_std,contours_salience_mean-v*(contours_salience_std));

    deleteusingsaliencemean(cons,contours_salience_mean-v*(contours_salience_std));
    printf("after delete using salience mean, have %d contours in all\n",cons->length);

    if(kind==1)
    {
        contours_pitch_mean=Contourpitchmean(cons);
        contours_pitch_std=Contourpitchstd(cons,contours_pitch_mean);
        printf("contours pitch mean is %f,contours pitch std is %f\n",contours_pitch_mean,contours_pitch_std);

        deleteusingpitchmean(cons,contours_pitch_mean-1.2*contours_pitch_std);
        printf("after delete using pitch mean, have %d contours in all\n",cons->length);
    }

    //printcontours(cons);

    int s,e;
    float *melodypitchmean;
    for(int i=0;i<3;i++)
    {
        melodypitchmean=pitchmeancontour(cons,250,&s,&e);
        deletepitchoutlier(cons,melodypitchmean,s,0);
    }
    printf("after delete pitch outlier, have %d contours in all\n",cons->length);

    //gettimeofday(&start,NULL);
    melodypitchmean=pitchmeancontour(cons,250,&s,&e);
    //gettimeofday(&finish,NULL);
    //printf("time of pitch mean contour is %f\n",difftime_ms(finish,start));
    deleteoctave(cons,melodypitchmean,s,0);
    printf("after delete octave, have %d contours in all\n",cons->length);


    //printcontours(cons);


    melodypitchmean=pitchmeancontour(cons,250,&s,&e);
    int *ultimatemelodypitch;
    //ultimatemelodypitch=ultimatemelodycontour(cons,framenumber);
    ultimatemelodypitch=ultimatemelodycontour(cons,melodypitchmean,s,framenumber);

    int i=0;
    
    //continuation naturally
    while(i<framenumber)
    {
        if(ultimatemelodypitch[i]==0)
        {
            int j=i+1;
            while(j<framenumber&&ultimatemelodypitch[j]==0)
            {
                j++;
            }
            if(j-i<6&&i>0)
            {
                for(int k=i;k<=j-1;k++)
                {
                    ultimatemelodypitch[k]=ultimatemelodypitch[i-1];
                }
            }
            i=j+1;
        }
        else
        {
            i++;
        }
    }

    gettimeofday(&finish,NULL);
    printf("time of voice detection is %f\n",difftime_ms(finish,start));

    char *wav=strrchr(wavfile,(int)'.');
    strncpy(wav,".txt",4);

    FILE *fp;

    fp=fopen(wavfile,"wb");
    if(fp!=NULL)
    {
        fprintf(fp,"%d\r\n",framenumber);
        for(int i=0;i<framenumber;i++)
        {
            fprintf(fp,"%d\r\n",ultimatemelodypitch[i]);
        }

        fclose(fp);
    }
    else
    {
        fprintf(stderr,"open file failed!\n");
    }
    gettimeofday(&end,NULL);
    printf("time  is %f\n",difftime_ms(end,first));

}

