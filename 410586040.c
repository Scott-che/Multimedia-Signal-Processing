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
};//建構struct來存wav的資料

int main(int argc,char *argv[]){
       FILE *fp;
       FILE *output_magnitude;
       FILE *output_phase;
       struct waveheader_ header;
       fp=fopen(argv[1],"rb");
       //以下是用來讀取header檔
       fread(header.riff,sizeof(char),4,fp);
       printf("%.4s\n",header.riff);

       fread(&header.file_size,sizeof(int),1,fp);
       printf("%d\n",header.file_size);

       fread(header.wave,sizeof(char),4,fp);
       printf("%.4s\n",header.wave);

       fread(header.fmt,sizeof(char),4,fp);
       printf("%.4s\n",header.fmt);

       fread(&header.length_data,sizeof(int),1,fp);
       printf("%d\n",header.length_data);

       fread(&header.wave_type,sizeof(short),1,fp);
       printf("%d\n",header.wave_type);

       fread(&header.channel,sizeof(short),1,fp);
       printf("%d\n",header.channel);

       fread(&header.sample_rate,sizeof(int),1,fp);
       printf("%d\n",header.sample_rate);

       fread(&header.Byte_Rate,sizeof(int),1,fp);
       printf("%d\n",header.Byte_Rate);

       fread(&header.Bit_Size_Channels_8,sizeof(short),1,fp);
       printf("%d\n",header.Bit_Size_Channels_8);

       fread(&header.bit_per_sample,sizeof(short),1,fp);
       printf("%d\n",header.bit_per_sample);

       fread(header.data,sizeof(char),4,fp);
       printf("%.4s\n",header.data);

       fread(&header.datd_size,sizeof(int),1,fp);
       printf("%d\n",header.datd_size);
       printf("run\n");
       //讀取data，每次取樣2bytes所以有56000/2個sample
       float x[28000];
       int k;
       for(k=0;k<28000;++k){
            fread(&header.Data,2,1,fp);

            unsigned long intdata=header.Data;

            long complement=0;

            if(intdata>32767){//從32765之後開始為最小負數，所以將負數換回來
                complement=intdata-65536;
            }
            else{//其餘為正數
                complement=intdata;
            }
            float float_data=(float)(complement/(float)32768);
            x[k]=float_data;
//printf("%f\n",x[k]);

       }
       fclose(fp);
       //以下是DFT

       float X_re[28000];
       float X_im[28000];
       float p[28000];
       float d[28000];
       float a[28000];
       int j;
       int n;
       output_magnitude=fopen(argv[2],"wb");
       output_phase=fopen(argv[3],"wb");

       for(j=0;j<28000;++j){
           X_re[j]=0;
           X_im[j]=0;
           for(n=0;n<28000;++n){//DFT公式
            X_re[j]+=x[n]*cos(n*j*double_pi/28000);
            X_im[j]-=x[n]*sin(n*j*double_pi/28000);
           }
           p[j]=X_re[j]*X_re[j]+X_im[j]*X_im[j];
           printf("%f\n",p[j]);
           a[j]=20*log10(sqrt(p[j]));//算出振幅
           d[j]=atan(X_im[j]/X_re[j]);//算出phase

           fwrite(&a[j],sizeof(float),1,output_magnitude);
           fwrite(&d[j],sizeof(float),1,output_phase);


       }

       fclose(output_magnitude);
       fclose(output_phase);

       printf("end\n");

    return 0;
}
