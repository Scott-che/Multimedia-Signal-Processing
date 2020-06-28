#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#define double_pi 6.283185
struct waveheader{///這個struct是用來寫入cos波的header檔
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
};
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
    short Data;
};///�鱈堨腠truct來存wav header

int main(){
       FILE *fp;
       FILE *fp2;
       FILE *fp3;
       FILE *fp4;
       FILE *fp5;
       FILE *cos_50Hz_8k;
       FILE *cos_200Hz_8k;
       FILE *cos_55Hz_8k;
       FILE *cos_220Hz_8k;
       FILE *cos_50Hz_16k;
       FILE *cos_200Hz_16k;
       FILE *cos_55Hz_16k;
       FILE *cos_220Hz_16k;

       struct waveheader_ header;
       struct waveheader header1;
       ///這邊是在建立cos弦波的wav檔，寫入header檔
       cos_50Hz_8k=fopen("cos_50Hz_8k.wav","wb");
       cos_200Hz_8k=fopen("cos_200Hz_8k.wav","wb");
       cos_55Hz_8k=fopen("cos_55Hz_8k.wav","wb");
       cos_220Hz_8k=fopen("cos_220Hz_8k.wav","wb");
       cos_50Hz_16k=fopen("cos_50Hz_16k.wav","wb");
       cos_200Hz_16k=fopen("cos_200Hz_16k.wav","wb");
       cos_55Hz_16k=fopen("cos_55Hz_16k.wav","wb");
       cos_220Hz_16k=fopen("cos_220Hz_16k.wav","wb");
       header1.riff[0]='R';
       header1.riff[1]='I';
       header1.riff[2]='F';
       header1.riff[3]='F';
       header1.file_size=16036;
       header1.wave[0]='W';
       header1.wave[1]='A';
       header1.wave[2]='V';
       header1.wave[3]='E';
       header1.fmt[0]='f';
       header1.fmt[1]='m';
       header1.fmt[2]='t';
       header1.fmt[3]=' ';
       header1.length_data=16;
       header1.wave_type=1;
       header1.channel=1;
       header1.sample_rate=8000;
       header1.Byte_Rate=16000;
       header1.Bit_Size_Channels_8=2;
       header1.bit_per_sample=16;
       header1.data[0]='d';
       header1.data[1]='a';
       header1.data[2]='t';
       header1.data[3]='a';
       header1.datd_size=16000;
       fwrite(&header1,sizeof(header1),1,cos_50Hz_8k);
       fwrite(&header1,sizeof(header1),1,cos_200Hz_8k);
       fwrite(&header1,sizeof(header1),1,cos_55Hz_8k);
       fwrite(&header1,sizeof(header1),1,cos_220Hz_8k);
       int n,f=50,f2=200,f3=55,f4=220;///這邊分別是寫入音訊資料
       short K[8000];
       for(n=0;n<8000;++n){
           K[n]=10000*cos(double_pi*f*n/8000);
           fwrite(&K[n],2,1,cos_50Hz_8k);
           K[n]=10000*cos(double_pi*f2*n/8000);
           fwrite(&K[n],2,1,cos_200Hz_8k);
           K[n]=10000*cos(double_pi*f3*n/8000);
           fwrite(&K[n],2,1,cos_55Hz_8k);
           K[n]=10000*cos(double_pi*f4*n/8000);
           fwrite(&K[n],2,1,cos_220Hz_8k);
       }
       header1.file_size=32036;
       header1.sample_rate=16000;
       header1.Byte_Rate=32000;
       header1.datd_size=32000;
       fwrite(&header1,sizeof(header1),1,cos_50Hz_16k);
       fwrite(&header1,sizeof(header1),1,cos_200Hz_16k);
       fwrite(&header1,sizeof(header1),1,cos_55Hz_16k);
       fwrite(&header1,sizeof(header1),1,cos_220Hz_16k);
       short Y[16000];
       for(n=0;n<16000;++n){
           Y[n]=10000*cos(double_pi*f*n/16000);
           fwrite(&Y[n],2,1,cos_50Hz_16k);
           Y[n]=10000*cos(double_pi*f2*n/16000);
           fwrite(&Y[n],2,1,cos_200Hz_16k);
           Y[n]=10000*cos(double_pi*f3*n/16000);
           fwrite(&Y[n],2,1,cos_55Hz_16k);
           Y[n]=10000*cos(double_pi*f4*n/16000);
           fwrite(&Y[n],2,1,cos_220Hz_16k);
       }
       fclose(cos_50Hz_8k);
       fclose(cos_200Hz_8k);
       fclose(cos_55Hz_8k);
       fclose(cos_220Hz_8k);
       fclose(cos_50Hz_16k);
       fclose(cos_200Hz_16k);
       fclose(cos_55Hz_16k);
       fclose(cos_220Hz_16k);

       int v;
       int fre1[4]={50,200,55,220};
       int fre2;
       char g[20];
       char s1[30],s2[30],s3[30],s4[30];

       for(v=0;v<10;++v){///這個大迴圈跑十次，每跑一次代表一個音檔去做frame, 做4個不同的window,dft，然後依序跑十次
            if(v<4){///這邊是用sprintf去改檔案名稱裡的變數，這樣就可以依序開啟wav檔，同時也用sprintf去改待會要輸出的txt檔的名稱
                fre2=8;
                sprintf(g,"cos_%dHz_%dk.wav",fre1[v],fre2);
                sprintf(s1,"cos_%dHz_%dk.Set1.txt",fre1[v],fre2);
                sprintf(s2,"cos_%dHz_%dk.Set2.txt",fre1[v],fre2);
                sprintf(s3,"cos_%dHz_%dk.Set3.txt",fre1[v],fre2);
                sprintf(s4,"cos_%dHz_%dk.Set4.txt",fre1[v],fre2);
            }
            else if(v>=4&&v<8) {fre2=16;
                sprintf(g,"cos_%dHz_%dk.wav",fre1[v-4],fre2);
                sprintf(s1,"cos_%dHz_%dk.Set1.txt",fre1[v-4],fre2);
                sprintf(s2,"cos_%dHz_%dk.Set2.txt",fre1[v-4],fre2);
                sprintf(s3,"cos_%dHz_%dk.Set3.txt",fre1[v-4],fre2);
                sprintf(s4,"cos_%dHz_%dk.Set4.txt",fre1[v-4],fre2);
            }
            else if(v>=8&&v<9){
                fre2=8;
                sprintf(g,"vowel-%dk.wav",fre2);
                sprintf(s1,"vowel-%dk.Set1.txt",fre2);
                sprintf(s2,"vowel-%dk.Set2.txt",fre2);
                sprintf(s3,"vowel-%dk.Set3.txt",fre2);
                sprintf(s4,"vowel-%dk.Set4.txt",fre2);
            }
            else {
                fre2=16;
                sprintf(g,"vowel-%dk.wav",fre2);
                sprintf(s1,"vowel-%dk.Set1.txt",fre2);
                sprintf(s2,"vowel-%dk.Set2.txt",fre2);
                sprintf(s3,"vowel-%dk.Set3.txt",fre2);
                sprintf(s4,"vowel-%dk.Set4.txt",fre2);
            }
        fp=fopen(g,"rb");///這邊就照剛剛sprintf改的名稱開啟檔案
        fp2=fopen(s1,"wb");
        fp3=fopen(s2,"wb");
        fp4=fopen(s3,"wb");
        fp5=fopen(s4,"wb");
       ///讀取wav header，並寫出header
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
       printf("%d\n",v);
       int sample=header.datd_size/2;
       short x[sample];
       int k;
       int p;///analysis window size
       int m;///frame interval
       int dftws;///DFT window size
       int q;
       int j;
       int t;
       int u;

       for(k=0;k<sample;++k){
            fread(&header.Data,2,1,fp);
            short intdata=header.Data;
            x[k]=intdata;
            }
            if(header.sample_rate==16000){///Set1
            p=80;
            m=80;
            dftws=128;
            }
            else {
                p=40;
                m=40;
                dftws=64;
            }
            float a[dftws];
            float x_[dftws];
            float X_re[dftws];
            float X_im[dftws];
            t=sample/m;

            for(q=0;q<t;++q){
                for(k=0;k<dftws;++k){

                        if(k<p){
                           x_[k]=x[q*m+k];
                    }
                        else x_[k]=0;
                }
                   for(j=0;j<dftws;++j){
                        X_re[j]=0;
                        X_im[j]=0;
                        for(u=0;u<dftws;++u){
                           X_re[j]+=x_[u]*cos(u*j*double_pi/dftws);
                           X_im[j]-=x_[u]*sin(u*j*double_pi/dftws);
                           }
                        a[j]=20*log10(sqrt(X_re[j]*X_re[j]+X_im[j]*X_im[j]));
                        fprintf(fp2,"%f ",a[j]);
                    }
                    fprintf(fp2,"\n");
            }
            if(header.sample_rate==16000){///Set2
            p=80;
            m=80;
            dftws=128;
            }
            else {
                p=40;
                m=40;
                dftws=64;
            }
            float b[dftws];
            float x1_[dftws];
            float X1_re[dftws];
            float X1_im[dftws];
            t=sample/m;

            for(q=0;q<t;++q){
                for(k=0;k<dftws;++k){

                        if(k<p){
                           x1_[k]=x[q*m+k]*(0.54-0.46*cos(double_pi*k/(p-1)));
                    }
                        else x1_[k]=0;
                    }
                   for(j=0;j<dftws;++j){
                        X1_re[j]=0;
                        X1_im[j]=0;
                        for(u=0;u<dftws;++u){
                           X1_re[j]+=x1_[u]*cos(u*j*double_pi/dftws);
                           X1_im[j]-=x1_[u]*sin(u*j*double_pi/dftws);
                        }
                        b[j]=20*log10(sqrt(X1_re[j]*X1_re[j]+X1_im[j]*X1_im[j]));
                        fprintf(fp3,"%f ",b[j]);
                    }
                    fprintf(fp3,"\n");
            }
            if(header.sample_rate==16000){///Set3
            p=320;
            m=160;
            dftws=512;
            }
            else {
                p=160;
                m=80;
                dftws=256;
            }
            float c[dftws];
            float x2_[dftws];
            float X2_re[dftws];
            float X2_im[dftws];
            t=sample/m;

            for(q=0;q<t;++q){
                for(k=0;k<dftws;++k){

                        if(k<p){
                           x2_[k]=x[q*m+k];
                    }
                        else x2_[k]=0;
                    }

                    for(j=0;j<dftws;++j){
                        X2_re[j]=0;
                        X2_im[j]=0;
                        for(u=0;u<dftws;++u){
                           X2_re[j]+=x2_[u]*cos(u*j*double_pi/dftws);
                           X2_im[j]-=x2_[u]*sin(u*j*double_pi/dftws);
                        }
                        c[j]=20*log10(sqrt(X2_re[j]*X2_re[j]+X2_im[j]*X2_im[j]));
                        fprintf(fp4,"%f ",c[j]);
                    }
                    fprintf(fp4,"\n");
            }
            if(header.sample_rate==16000){///Set4
            p=320;
            m=160;
            dftws=512;
            }
            else {
                p=160;
                m=80;
                dftws=256;
            }
            float d[dftws];
            float x3_[dftws];
            float X3_re[dftws];
            float X3_im[dftws];
            t=sample/m;

            for(q=0;q<t;++q){
                for(k=0;k<dftws;++k){

                        if(k<p){
                           x3_[k]=x[q*m+k]*(0.54-0.46*cos(double_pi*k/(p-1)));
                    }
                        else x3_[k]=0;
                    }
                    for(j=0;j<dftws;++j){
                        X3_re[j]=0;
                        X3_im[j]=0;
                        for(u=0;u<dftws;++u){
                           X3_re[j]+=x3_[u]*cos(u*j*double_pi/dftws);
                           X3_im[j]-=x3_[u]*sin(u*j*double_pi/dftws);
                        }
                        d[j]=20*log10(sqrt(X3_re[j]*X3_re[j]+X3_im[j]*X3_im[j]));
                        fprintf(fp5,"%f ",d[j]);
                    }
                    fprintf(fp5,"\n");
            }
       }
       return 0;
}
