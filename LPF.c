#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define double_pi 6.283185

struct waveheader_{
    char riff[4];
    int file_size;
    char wave[4];
    char fmt[4];
    int length_data;
    short wave_type;
    short channel;
    int sample_rate;
    int Byte_Rate;
    short Bit_Size_Channels_8;
    short bit_per_sample;
    char data[4];
    int datd_size;
    uint16_t Data;
    short Data1;
};//建立struct來存wav header

int main(int argc,char *argv[]){
       FILE *fp;
       FILE *fp2;

       struct waveheader_ header;
       fp=fopen(argv[1],"rb");
       fp2=fopen(argv[2],"wb");

       //讀取wav header，並寫出header
       fread(header.riff,sizeof(char),4,fp);
       fwrite(&header.riff,sizeof(char),4,fp2);
       printf("%.4s\n",header.riff);

       fread(&header.file_size,sizeof(int),1,fp);
       fwrite(&header.file_size,sizeof(int),1,fp2);
       printf("%d\n",header.file_size);

       fread(header.wave,sizeof(char),4,fp);
       fwrite(&header.wave,sizeof(char),4,fp2);
       printf("%.4s\n",header.wave);

       fread(header.fmt,sizeof(char),4,fp);
       fwrite(&header.fmt,sizeof(char),4,fp2);
       printf("%.4s\n",header.fmt);

       fread(&header.length_data,sizeof(int),1,fp);
       fwrite(&header.length_data,sizeof(int),1,fp2);
       printf("%d\n",header.length_data);

       fread(&header.wave_type,sizeof(short),1,fp);
       fwrite(&header.wave_type,sizeof(short),1,fp2);
       printf("%d\n",header.wave_type);

       fread(&header.channel,sizeof(short),1,fp);
       fwrite(&header.channel,sizeof(short),1,fp2);
       printf("%d\n",header.channel);

       fread(&header.sample_rate,sizeof(int),1,fp);
       fwrite(&header.sample_rate,sizeof(int),1,fp2);
       printf("%d\n",header.sample_rate);

       fread(&header.Byte_Rate,sizeof(int),1,fp);
       fwrite(&header.Byte_Rate,sizeof(int),1,fp2);
       printf("%d\n",header.Byte_Rate);

       fread(&header.Bit_Size_Channels_8,sizeof(short),1,fp);
       fwrite(&header.Bit_Size_Channels_8,sizeof(short),1,fp2);
       printf("%d\n",header.Bit_Size_Channels_8);

       fread(&header.bit_per_sample,sizeof(short),1,fp);
       fwrite(&header.bit_per_sample,sizeof(short),1,fp2);
       printf("%d\n",header.bit_per_sample);

       fread(header.data,sizeof(char),4,fp);
       fwrite(&header.data,sizeof(char),4,fp2);
       printf("%.4s\n",header.data);

       fread(&header.datd_size,sizeof(int),1,fp);
       fwrite(&header.datd_size,sizeof(int),1,fp2);
       printf("%d\n",header.datd_size);
       printf("run\n");
       //每次取樣2bytes，所以有56000/2個sample
       float x[28000];
       int k;
       int y[28000];
       for(k=0;k<28000;++k){
            fread(&header.Data,2,1,fp);

            unsigned long intdata=header.Data;

            long complement=0;

            if(intdata>32767){//從32767後，接為負數
                complement=intdata-65536;
            }
            else{//其餘皆為正數
                complement=intdata;
            }
            float float_data=(float)(complement/(float)32768);
            x[k]=float_data;
            if(x[k]>=0){//保留原先的正負號
               y[k]=1;
            }
            else{
                y[k]=-1;
            }
//printf("%d",y[k]);

       }
       fclose(fp);

       float X_re[28000];
       float X_im[28000];
       int j;
       int n;


       for(j=0;j<28000;++j){
           X_re[j]=0;
           X_im[j]=0;
           for(n=0;n<28000;++n){//DFT公式
            X_re[j]+=x[n]*cos(n*j*double_pi/28000);
            X_im[j]-=x[n]*sin(n*j*double_pi/28000);
           }

          if(j<=12250){//以下是濾波
            X_re[j]=(X_re[j])/2;
            X_im[j]=(X_im[j])/2;
          }
          else if(j>12250&&j<15750){
            X_re[j]=0;
            X_im[j]=0;
          }
          else{
            X_re[j]=(X_re[j])/2;
            X_im[j]=(X_im[j])/2;
          }




       }

       ///IDFT公式
       float idft_re[28000];
       float idft_im[28000];
       float idft[28000];
       for(j=0;j<28000;++j){
            idft_re[j]=0;
            idft_im[j]=0;
           for(n=0;n<28000;++n){
               idft_re[j]+=X_re[n]*cos(n*j*double_pi/28000)-X_im[n]*sin(n*j*double_pi/28000);
               idft_im[j]+=X_im[n]*cos(n*j*double_pi/28000)+X_re[n]*sin(n*j*double_pi/28000);

           }
           idft_re[j]/=28000;
           idft_im[j]/=28000;
           idft[j]=idft_re[j]*idft_re[j]+idft_im[j]*idft_im[j];

           float float_data2=sqrt(idft[j]);//平方相加開根號取大小
           //printf("%f\n",float_data2);
           //long complement=0;
           short intdata2;
           intdata2=(short)(float_data2*65536);
           //printf("%d\n",complement);
           /*if(complement>32767){
           indata2=complement-65536;
           header.Data1=indata2;
           }
           else{
              indata2=complement;
              header.Data1=indata2;
               }*/
          // printf("%d\n",header.Data1);
           header.Data1=intdata2*y[j];//將data乘上正負號
           fwrite(&header.Data1,2,1,fp2);//把資料寫入wav的data裡

       }

       fclose(fp2);

       printf("end\n");

    return 0;
}
