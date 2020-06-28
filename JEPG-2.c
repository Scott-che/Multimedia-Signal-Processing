#include <stdio.h>
#include <stdlib.h>
#include <math.h>
float PI=3.1415927;
//int ac[181104]={0};
int P=0;
float q0[8][8]={
      { 16, 11, 10, 16, 24, 40, 51, 61},/**亮度量化表**/
      { 12, 12, 14, 19, 26, 58, 60, 55},
      { 14, 13, 16, 24, 40, 57, 69, 56},
      { 14, 17, 22, 29, 51, 87, 80, 82},
      { 18, 22, 37, 56, 68, 109, 103, 77},
      { 24, 35, 55, 64, 81, 104, 113, 92},
      { 49, 64, 78, 87, 103, 121, 120, 101},
      { 72, 92, 95, 98, 112, 100, 103, 99}
};
float q1[8][8]={
     { 17, 18, 24, 47, 99, 99, 99, 99},/**彩度量化表**/
     { 18, 21, 26, 66, 99, 99, 99, 99},
     { 24, 26, 56, 99, 99, 99, 99, 99},
     { 47, 66, 99, 99, 99, 99, 99, 99},
     { 99, 99, 99, 99, 99, 99, 99, 99},
     { 99, 99, 99, 99, 99, 99, 99, 99},
     { 99, 99, 99, 99, 99, 99, 99, 99},
     { 99, 99, 99, 99, 99, 99, 99, 99}
};

/*construct a structure of BMP header*/
typedef struct Bmpheader{
    unsigned short identifier; // 0x0000
    unsigned int filesize; // 0x0002
    unsigned short reserved; // 0x0006
    unsigned short reserved2;
    unsigned int bitmap_dataoffset; // 0x000A
    unsigned int bitmap_headersize; // 0x000E
    unsigned int width; // 0x0012
    unsigned int height; // 0x0016
    unsigned short planes; // 0x001A
    unsigned short bits_perpixel; // 0x001C
    unsigned int compression; // 0x001E
    unsigned int bitmap_datasize; // 0x0022
    unsigned int hresolution; // 0x0026
    unsigned int vresolution; // 0x002A
    unsigned int usedcolors; // 0x002E
    unsigned int importantcolors; // 0x0032
    unsigned int palette; // 0x0036
} Bitmap;

/*construct a structure of RGB*/
typedef struct RGB{
    int R;
    int G;
    int B;
} ImgRGB;
struct a{
    int r;
    int g;
    int b;
};
struct b{
    int R_y[30];
    int R_cb[30];
    int R_cr[30];
    int L_y[30];
    int L_cb[30];
    int L_cr[30];
    int Zigzag1[64];
    int Zigzag2[64];
    int Zigzag3[64];
};
int K=0,T=0;
struct c{
   int symbol[145];
   int times[145];

};
struct d{
   int s[300];
   //int s1[260];
   int t[300];
};
Bitmap readheader(FILE* fp);
ImgRGB** malloc_2D(int row, int col);
struct b rl[25715];
struct c huf[3];
struct d Huf[3];
void quicksort(int A[],int B[],int first,int last){
    int i,j,pivot,temp,temp2;
    if(first<last){
        pivot=first;
        i=first;
        j=last;
        while(i<j){
            while(A[i]<=A[pivot]&&i<last)
                i++;
            while(A[j]>A[pivot])
                j--;

            if(i<j){
                temp=A[i];
                temp2=B[i];
                A[i]=A[j];
                B[i]=B[j];
                A[j]=temp;
                B[j]=temp2;
            }
        }
        temp=A[pivot];
        temp2=B[pivot];
        A[pivot]=A[j];
        B[pivot]=B[j];
        A[j]=temp;
        B[j]=temp2;
        quicksort(A,B,first,j-1);
        quicksort(A,B,j+1,last);
    }
}

void InputData(FILE* fp,ImgRGB **array,int H,int W);
void output_bmp(ImgRGB **RGB,FILE* outfile,Bitmap bmpheader);
void RGB2YCbCr(int rr,int gg,int bb,float *y,float *cb,float *cr);
void dct(float *pic_in,float *enc_out);
void quantize(float *dctb,float *qb,int n,int t);
void dequantize(float *qb,float *dctb,int n,int c);
void idct(float *enc_in,float *rec_out);
void YCbCr2RGB(float *y,float *cb,float *cr,float *r,float *g,float *b);
void zigzag(float quant1[8][8],float quant2[8][8],float quant3[8][8],int t);
void izigzag(float *quant1,float *quant2,float *quant3,int c);
void dpcm(float *dc,float *dc_out);
void idpcm(float *dc,float *dc_out,float *idc);
void RLE(int t,int *q,int s);
void iRLE(int t,int *q,int s);
void dc_count(float *dif,int o);
void ac_count(int *q,int s);
void huf_dc(FILE *fp,int o);
void huf_ac(FILE *fp,int e);
int main(/*int argc,char *argv[]*/){

    FILE* fp=fopen("input.bmp","rb");//argv[1]
    FILE *fw=fopen("out.bmp","wb");//argv[2]
    FILE *fp_y=fopen("codebook_y.txt","wb");
    FILE *fp_cb=fopen("codebook_cb.txt","wb");
    FILE *fp_cr=fopen("codebook_cr.txt","wb");
    FILE *fp_ac_y=fopen("codebook_ac_y.txt","wb");
    FILE *fp_ac_cb=fopen("codebook_ac_cb.txt","wb");
    FILE *fp_ac_cr=fopen("codebook_ac_cr.txt","wb");
    //read header
    Bitmap bmpheader=readheader(fp);

    ImgRGB **Data_RGB=malloc_2D(bmpheader.height, bmpheader.width);

	//read data
    InputData(fp,Data_RGB,bmpheader.height,bmpheader.width);
    printf("%d\n%d\n",bmpheader.width,bmpheader.height);
    struct a rgb[8][8];

    /*put other else function here*/
    int x,y,u=0,v=0,m=8,n=8,R1,G1,B1,k=0,t=0,p,z,q1[25715]={0},q2[25715]={0},q3[25715]={0},c=0;
    int M,N,U=0,V=0;
    float Y,Cb,Cr;
    float buf_y1[8][8],buf_cb1[8][8],buf_cr1[8][8],DCT1[8][8],DCT2[8][8],DCT3[8][8],IDCT1[8][8],IDCT2[8][8],IDCT3[8][8];
    float quant1[8][8],quant2[8][8],quant3[8][8],dquant1[8][8],dquant2[8][8],dquant3[8][8];
    float a[8][8],b[8][8],cc[8][8];
    float dc_y[25715]={0},dc_cb[25715]={0},dc_cr[25715]={0},DC_y[25715]={0},DC_cb[25715]={0},DC_cr[25715]={0},IDC_y[25715]={0},IDC_cb[25715]={0},IDC_cr[25715]={0};
    while(1){
       m=8;n=8;
       if(v>184){
        break;
       }
       for(x=0;x<8;++x){/**把各個陣列出使為零**/
        for(y=0;y<8;++y){
            rgb[x][y].r=0;rgb[x][y].g=0;rgb[x][y].b=0;
            buf_y1[x][y]=0;buf_cb1[x][y]=0;buf_cr1[x][y]=0;
            DCT1[x][y]=0;DCT2[x][y]=0;DCT3[x][y]=0;
            IDCT1[x][y]=0;IDCT2[x][y]=0;IDCT3[x][y]=0;
            dquant1[x][y]=0;dquant2[x][y]=0;dquant3[x][y]=0;
        }
       }
       if(u==138){
        m=4;
       }
       if(v==184){
        n=6;
       }
       for(x=0;x<n;++x){/**blocking，不滿把個補零**/
           for(y=0;y<m;++y){
               rgb[x][y].r=Data_RGB[x+8*v][y+8*u].R;
               rgb[x][y].g=Data_RGB[x+8*v][y+8*u].G;
               rgb[x][y].b=Data_RGB[x+8*v][y+8*u].B;
               R1=rgb[x][y].r;G1=rgb[x][y].g;B1=rgb[x][y].b;
               RGB2YCbCr(R1,G1,B1,&Y,&Cb,&Cr);/**轉成YCbCr**/
               buf_y1[x][y]=Y-128;
               buf_cb1[x][y]=Cb-128;
               buf_cr1[x][y]=Cr-128;
               //printf("%f\n",buf_cb1[x][y]);
           }
       }
       dct(&buf_y1[0][0],&DCT1[0][0]);/**把每個8*8block去做dct，quantize，dequantize，idct，最後轉回RGB**/
       dct(&buf_cb1[0][0],&DCT2[0][0]);
       dct(&buf_cr1[0][0],&DCT3[0][0]);
       //printf("%f\n",buf_y1[0][0]);
       quantize(&DCT1[0][0],&quant1[0][0],0,t);
       quantize(&DCT2[0][0],&quant2[0][0],1,t);
       quantize(&DCT3[0][0],&quant3[0][0],2,t);
       //printf("%f\n",quant1[0][0]);
       zigzag(quant1,quant2,quant3,t);
       dc_y[t]=quant1[0][0];
       dc_cb[t]=quant2[0][0];
       dc_cr[t]=quant3[0][0];
       RLE(t,&q1[0],1);/**做RLE**/
       RLE(t,&q2[0],2);
       RLE(t,&q3[0],3);
       iRLE(t,&q1[0],1);
       iRLE(t,&q2[0],2);
       iRLE(t,&q3[0],3);
       /*for(x=0;x<64;x++){
        printf("%f\t",rl[t].Zigzag1[x]);
       }
       printf("%d\n",t);**/
       /*for(x=0;x<=q;++x){
        printf("%d %d %d\n",rl[t].R[x],rl[t].L[x],t);
       }*/
       //printf("%d\n",t);
       ++t;
       if(t==25715){

        dpcm(&dc_y[0],&DC_y[0]);
        dc_count(&DC_y[0],0);/**分別計算dc、ac出現的次數，並產生codebook**/
        huf_dc(fp_y,0);
        ac_count(&q1[0],0);
        huf_ac(fp_ac_y,0);

        dpcm(&dc_cb[0],&DC_cb[0]);
        dc_count(&DC_cb[0],1);
        huf_dc(fp_cb,1);
        ac_count(&q2[0],1);
        huf_ac(fp_ac_cb,1);

        dpcm(&dc_cr[0],&DC_cr[0]);
        dc_count(&DC_cr[0],2);
        huf_dc(fp_cr,2);
        ac_count(&q3[0],2);
        huf_ac(fp_ac_cr,2);

        idpcm(&dc_y[0],&DC_y[0],&IDC_y[0]);
        idpcm(&dc_cb[0],&DC_cb[0],&IDC_cb[0]);
        idpcm(&dc_cr[0],&DC_cr[0],&IDC_cr[0]);
        for(x=0;x<25715;x++){
        rl[x].Zigzag1[0]=IDC_y[x];
        rl[x].Zigzag2[0]=IDC_cb[x];
        rl[x].Zigzag3[0]=IDC_cr[x];
        //printf("%d\t%d\t%d\n",dc_cb[x],rl[x].Zigzag3[0],x);
       }
       while(1){
        for(x=0;x<8;x++){
            for(y=0;y<8;y++){
                IDCT1[x][y]=0;quant1[x][y]=0;dquant1[x][y]=0;buf_y1[x][y]=0;
                IDCT2[x][y]=0;quant2[x][y]=0;dquant2[x][y]=0;buf_cb1[x][y]=0;
                IDCT3[x][y]=0;quant3[x][y]=0;dquant3[x][y]=0;buf_cr1[x][y]=0;
            }
        }
       if(c>25714){
        break;
       }
       M=8,N=8;
       if(U==138)
        M=4;
       if(V==184)
        N=6;
       izigzag(&quant1[0][0],&quant2[0][0],&quant3[0][0],c);
       //printf("%f\n",quant1[0][0]);
       dequantize(&quant1[0][0],&dquant1[0][0],0,c);
       dequantize(&quant2[0][0],&dquant2[0][0],1,c);
       dequantize(&quant3[0][0],&dquant3[0][0],2,c);
       //printf("%f\n",dquant1[0][0]);
       idct(&dquant1[0][0],&IDCT1[0][0]);
       idct(&dquant2[0][0],&IDCT2[0][0]);
       idct(&dquant3[0][0],&IDCT3[0][0]);
       //printf("%f\n",IDCT1[0][0]);
       YCbCr2RGB(&IDCT1[0][0],&IDCT2[0][0],&IDCT3[0][0],&buf_y1[0][0],&buf_cb1[0][0],&buf_cr1[0][0]);
       //printf("%f\n",buf_y1[0][0]);

       for(x=0;x<N;++x){///把轉換回來的資料寫入
        for(y=0;y<M;++y){
            Data_RGB[x+8*V][y+8*U].R=buf_y1[x][y];
            Data_RGB[x+8*V][y+8*U].G=buf_cb1[x][y];
            Data_RGB[x+8*V][y+8*U].B=buf_cr1[x][y];
        }
       }
       c++;
       U=U+1;
       if(U>138){
        U=0;
        V=V+1;
       }
       }
    }











       u=u+1;
       if(u>138){
        u=0;
        v=v+1;
       }
    }

    //output bmp
    output_bmp(Data_RGB,fw,bmpheader);
    printf("END\n");
    return 0;
}


/*read header*/
Bitmap readheader(FILE* fp){
    Bitmap x;
    fread(&x.identifier,sizeof(x.identifier),1,fp);
    fread(&x.filesize,sizeof(x.filesize),1,fp);
    fread(&x.reserved,sizeof(x.reserved),1,fp);
    fread(&x.reserved2,sizeof(x.reserved2),1,fp);
    fread(&x.bitmap_dataoffset,sizeof(x.bitmap_dataoffset),1,fp);
    fread(&x.bitmap_headersize,sizeof(x.bitmap_headersize),1,fp);
    fread(&x.width,sizeof(x.width),1,fp);
    fread(&x.height,sizeof(x.height),1,fp);
    fread(&x.planes,sizeof(x.planes),1,fp);
    fread(&x.bits_perpixel,sizeof(x.bits_perpixel),1,fp);
    fread(&x.compression,sizeof(x.compression),1,fp);
    fread(&x.bitmap_datasize,sizeof(x.bitmap_datasize),1,fp);
    fread(&x.hresolution,sizeof(x.hresolution),1,fp);
    fread(&x.vresolution,sizeof(x.vresolution),1,fp);
    fread(&x.usedcolors,sizeof(x.usedcolors),1,fp);
    fread(&x.importantcolors,sizeof(x.importantcolors),1,fp);
    return x;
}

/*input data without header into RGB structure*/
void InputData(FILE* fp,ImgRGB **array,int H, int W){
    int temp,i,j;
    for(i=0;i<H;i++){
        for(j=0;j<W;j++){
            temp=fgetc(fp);
            array[i][j].B=temp;
            temp=fgetc(fp);
            array[i][j].G=temp;
            temp=fgetc(fp);
            array[i][j].R=temp;
        }
    }
}

/* A function of making two dimensions memory locate array*/
ImgRGB** malloc_2D(int row, int col){
    ImgRGB **Array, *Data;
    int i;
    Array=(ImgRGB**)malloc(row*sizeof(ImgRGB *));
    Data=(ImgRGB*)malloc(row*col*sizeof(ImgRGB));
    for(i=0; i<row; i++,Data+=col){
        Array[i] = Data;
    }
    return Array;
}

/*output header and data*/
void output_bmp(ImgRGB **RGB,FILE* outfile,Bitmap bmpheader){
    int x,y;
    fwrite(&bmpheader.identifier, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.filesize, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.reserved, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.reserved2, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.bitmap_dataoffset, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.bitmap_headersize, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.width, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.height, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.planes, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.bits_perpixel, sizeof(short), 1 , outfile);
    fwrite(&bmpheader.compression, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.bitmap_datasize, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.hresolution, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.vresolution, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.usedcolors, sizeof(int), 1 , outfile);
    fwrite(&bmpheader.importantcolors, sizeof(int), 1 , outfile);

    for (x=0; x<bmpheader.height; x++){
        for(y=0; y<bmpheader.width; y++){
            fwrite(&RGB[x][y].B, sizeof(char),1,outfile);
            fwrite(&RGB[x][y].G, sizeof(char),1,outfile);
            fwrite(&RGB[x][y].R, sizeof(char),1,outfile);
        }
    }
}
void RGB2YCbCr(int rr,int gg,int bb,float *y,float *cb,float *cr){
    *y=0.299*rr+0.587*gg+0.114*bb;
    *cb=-0.1687*rr-0.3313*gg+0.5*bb+128;
    *cr=0.5*rr-0.4187*gg-0.0813*bb+128;
}
void YCbCr2RGB(float *y,float *cb,float *cr,float *r,float *g,float *b){
    int i,j;
    for(i=0;i<8;i++){
        for(j=0;j<8;j++){
            r[i*8+j]=128+y[i*8+j]+1.402*cr[i*8+j];
            g[i*8+j]=128+y[i*8+j]-0.34414*cb[i*8+j]-0.71414*cr[i*8+j];
            b[i*8+j]=128+y[i*8+j]+1.772*cb[i*8+j];
            if(r[i*8+j]>255){
                r[i*8+j]=255;
            }
            if(r[i*8+j]<0){
                r[i*8+j]=0;
            }
            if(g[i*8+j]>255){
                g[i*8+j]=255;
            }
            if(g[i*8+j]<0){
                g[i*8+j]=0;
            }
            if(b[i*8+j]>255){
                b[i*8+j]=255;
            }
            if(b[i*8+j]<0){
                b[i*8+j]=0;
            }
        }

    }
}
void dct(float *pic_in,float *enc_out){
    int u,v,x,y;
    float u_cs,v_cs;
    for(u=0;u<8;u++){
        for(v=0;v<8;v++){
            for(x=0;x<8;x++){
                for(y=0;y<8;y++){
                    u_cs=cos(((2*x+1)*u*PI)/16);
                    if(u==0)
                        u_cs=(1/(sqrt(2)));
                    v_cs=cos(((2*y+1)*v*PI)/16);
                    if(v==0)
                        v_cs=(1/(sqrt(2)));
                    enc_out[u+8*v]+=0.25*pic_in[x+8*y]*u_cs*v_cs;
                }
            }
        }
    }
}
void idct(float *enc_in,float *rec_out){
    int u,v,x,y;
    float u_cs,v_cs;
    for(x=0;x<8;x++){
        for(y=0;y<8;y++){
            for(u=0;u<8;u++){
                for(v=0;v<8;v++){
                    u_cs=cos(((2*x+1)*u*PI)/16);
                    if(u==0)
                        u_cs=(1/(sqrt(2)));
                    v_cs=cos(((2*y+1)*v*PI)/16);
                    if(v==0)
                        v_cs=(1/(sqrt(2)));
                    rec_out[x+8*y]+=0.25*enc_in[u+8*v]*u_cs*v_cs;
                }
            }
            //printf("%f\n",rec_out[x+8*y]);
        }
    }
}
void quantize(float *dctb,float *qb,int n,int t){
    int u,v;
    for(v=0;v<8;v++){
        for(u=0;u<8;u++){
            if(n==0){
                qb[v*8+u]=dctb[v*8+u]/q0[v][u];
                rl[t].Zigzag1[v*8+u]=qb[v*8+u];
                }
            else{
                qb[v*8+u]=dctb[v*8+u]/q1[v][u];
                if(n==1){
                    rl[t].Zigzag2[v*8+u]=qb[v*8+u];
                }
                if(n==2){
                    rl[t].Zigzag3[v*8+u]=qb[v*8+u];
                }
            }
        }
    }
}
void dequantize(float *qb,float *dctb,int n,int c){
    int u,v;
    for(v=0;v<8;v++){
        for(u=0;u<8;u++){
            if(n==0)
                dctb[v*8+u]=qb[v*8+u]*q0[v][u];
            else{
                dctb[v*8+u]=qb[v*8+u]*q1[v][u];
                }
        }
    }
}
void zigzag(float quant1[8][8],float quant2[8][8],float quant3[8][8],int t){
    int i,j;
    int u[64]={0,0,1,2,1,0,0,1,2,3,4,3,2,1,0,0,1,2,3,4,5,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7,7,6,5,4,3,2,1,2,3,4,5,6,7,7,6,5,4,3,4,5,6,7,7,6,5,6,7,7};
    int v[64]={0,1,0,0,1,2,3,2,1,0,0,1,2,3,4,5,4,3,2,1,0,0,1,2,3,4,5,6,7,6,5,4,3,2,1,0,1,2,3,4,5,6,7,7,6,5,4,3,2,3,4,5,6,7,7,6,5,4,5,6,7,7,6,7};
    for(i=0;i<64;i++){
        rl[t].Zigzag1[i]=(int)(quant1[u[i]][v[i]]);
        rl[t].Zigzag2[i]=(int)(quant2[u[i]][v[i]]);
        rl[t].Zigzag3[i]=(int)(quant3[u[i]][v[i]]);
        //printf("%d\t",rl[t].Zigzag1[i]);
    }
    //printf("\n");
}
void izigzag(float *Quant1,float *Quant2,float *Quant3,int c){
    int i,j;
    int u[64]={0,0,1,2,1,0,0,1,2,3,4,3,2,1,0,0,1,2,3,4,5,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7,7,6,5,4,3,2,1,2,3,4,5,6,7,7,6,5,4,3,4,5,6,7,7,6,5,6,7,7};
    int v[64]={0,1,0,0,1,2,3,2,1,0,0,1,2,3,4,5,4,3,2,1,0,0,1,2,3,4,5,6,7,6,5,4,3,2,1,0,1,2,3,4,5,6,7,7,6,5,4,3,2,3,4,5,6,7,7,6,5,4,5,6,7,7,6,7};
    for(i=0;i<64;i++){
        Quant1[u[i]*8+v[i]]=(rl[c].Zigzag1[i]);
        Quant2[u[i]*8+v[i]]=(rl[c].Zigzag2[i]);
        Quant3[u[i]*8+v[i]]=(rl[c].Zigzag3[i]);
        //printf("%f\n",Quant1[u[i]*8+v[i]]);
    }
}
void dpcm(float *dc,float *dc_out){
    int i;
    for(i=0;i<25715;i++){
        if(i==0)
            dc_out[i]=dc[i];
        else
            dc_out[i]=dc[i]-dc[i-1];
    }
}
void idpcm(float *dc,float *dc_out,float *idc){
    int i;
    for(i=0;i<25715;i++){
        if(i==0)
            idc[i]=dc_out[i];
        else
            idc[i]=dc_out[i]+dc[i-1];
    }
}
void RLE(int t,int *q,int s){
    int i=63,last,p=0,k=0,w,e;
    if(s==1){
        //printf("asasa\n");
        while(i>=0){
        e=rl[t].Zigzag1[i];
        if(e==0)
            i=i-1;
        else if(e!=0){
            last=i;
            break;
        }
        }
        if(i>0){
         p=0;
         for(w=0;w<=last;w++){
             e=rl[t].Zigzag1[w];
             if(e!=0&&w!=0){
                rl[t].R_y[k]=w-p-1;
                rl[t].L_y[k]=e;
                k++;
                p=w;
             }
         }
        rl[t].R_y[k]=0;
        rl[t].L_y[k]=0;
        q[t]=k;
        }
        else{
            q[t]=0;
            rl[t].R_y[0]=0;
            rl[t].L_y[0]=0;
        }
        //printf("%d\t%d\n",t,q[t]);
    }
    if(s==2){
        while(i>=0){
                e=rl[t].Zigzag2[i];
        if(e==0)
            i=i-1;
        else if(e!=0){
            last=i;
            break;
        }
    }
    if(i>0){
    for(i=0;i<=last;i++){
            e=rl[t].Zigzag2[i];
        if(e!=0&&i!=0){
            rl[t].R_cb[k]=i-p-1;
            rl[t].L_cb[k]=rl[t].Zigzag2[i];
            k++;
            p=i;
        }
    }

    rl[t].R_cb[k]=0;
    rl[t].L_cb[k]=0;
    q[t]=k;
    }
    else{
        q[t]=0;
        rl[t].R_cb[0]=0;
        rl[t].L_cb[0]=0;
    }
    }
    if(s==3){
        while(i>=0){
                e=rl[t].Zigzag3[i];
        if(e==0)
            i=i-1;
        else if(e!=0){
            last=i;
            break;
        }
    }
    if(i>0){
    for(i=0;i<=last;i++){
            e=rl[t].Zigzag3[i];
        if(e!=0&&i!=0){
            rl[t].R_cr[k]=i-p-1;
            rl[t].L_cr[k]=rl[t].Zigzag3[i];
            k++;
            p=i;
        }
    }

    rl[t].R_cr[k]=0;
    rl[t].L_cr[k]=0;
    q[t]=k;
    }
    else{
        q[t]=0;
        rl[t].R_cr[0]=0;
        rl[t].L_cr[0]=0;
    }
    }

}
void iRLE(int t,int *q,int s){
    int i,p=0,v;
    if(s==1){
    if(q[t]!=0){
    for(i=0;i<q[t];i++){
        v=rl[t].R_y[i];
        rl[t].Zigzag1[p+v+1]=rl[t].L_y[i];
        p=p+v+1;
    }
    for(i=p+1;i<64;i++){
        rl[t].Zigzag1[i]=0;
    }
    }
    else if(q[t]==0){
    for(i=1;i<64;++i){
        rl[t].Zigzag1[i]=0;
    }
    }
    }
    if(s==2){
    if(q[t]!=0){
    for(i=0;i<q[t];i++){
        v=rl[t].R_cb[i];
        rl[t].Zigzag2[p+v+1]=rl[t].L_cb[i];
        p=p+v+1;
    }
    for(i=p+1;i<64;i++){
        rl[t].Zigzag2[i]=0;
    }
    }
    else if(q[t]==0){
    for(i=1;i<64;++i){
        rl[t].Zigzag2[i]=0;
    }
    }
    }
    if(s==3){
    if(q[t]!=0){
    for(i=0;i<q[t];i++){
        v=rl[t].R_cr[i];
        rl[t].Zigzag3[p+v+1]=rl[t].L_cr[i];
        p=p+v+1;
    }
    for(i=p+1;i<64;i++){
        rl[t].Zigzag3[i]=0;
    }
    }
    else if(q[t]==0){
    for(i=1;i<64;++i){
        rl[t].Zigzag3[i]=0;
    }
    }
    }
}
void dc_count(float *dif,int o){
    int x,y,temp1,temp2,i=0,t,k,a=0,g=0;
    for(x=0;x<25715;x++){
        if(x==0){
        temp1=(int)(dif[x]);
        t=0;
        a=1;
        }
        if(x>0){
            k=0;
            while(1){

                if((int)(dif[x])==huf[o].symbol[k]){
                   a=0;
                   break;
                }
                else{
                    ++k;
                    if(k>=i){
                    a=1;
                    temp1=(int)(dif[x]);
                    t=0;
                    break;
                   }
                }
            }
      }
      if(a==1){
           for(y=0;y<25715;y++){
               temp2=(int)(dif[y]);
               if(temp1==temp2){
                  ++t;
               }
           }
           huf[o].symbol[i]=temp1;
           huf[o].times[i]=t;
           g+=t;
           //printf("%d\t%d\t%d\t%d\n",symbol[i],times[i],i,g);
           ++i;
        }
    }
    K=i;
}
void ac_count(int *q,int s){
    int x,y,z=0,temp1=0,temp2=0,t,a=0,k,m,n,Z=0,i=0;
    if(s==0){
    for(x=0;x<25715;x++){
        for(y=0;y<=q[x];y++){
            if(rl[x].L_y[y]>=0){
                z=1000*rl[x].L_y[y]+rl[x].R_y[y];/**先判斷L值的正負，然後把L值乘上1000，再加或減R值，得到一個symbol**/
            }
            else
                z=1000*rl[x].L_y[y]-rl[x].R_y[y];
            if(x==0&&y==0){
                temp1=z;
                t=0;
                a=1;
            }
            if((x==0&&y>0)||(x!=0&&y>=0)){
            k=0;
            while(1){

                if(z==Huf[s].s[k]){
                   a=0;
                   break;
                }
                else{
                    ++k;
                    if(k>=i){
                    a=1;
                    temp1=z;
                    t=0;
                    break;
                   }
                }
            }
           }
      if(a==1){
           for(m=0;m<25715;m++){
            for(n=0;n<=q[m];n++){
               if(rl[m].L_y[n]>=0){
                Z=1000*rl[m].L_y[n]+rl[m].R_y[n];
            }
               else{
                Z=1000*rl[m].L_y[n]-rl[m].R_y[n];
               }
               temp2=Z;
               if(temp1==temp2){
                  ++t;
               }
            }
           }
           Huf[s].s[i]=temp1;
           Huf[s].t[i]=t;
           //g+=t;
           //printf("%d\t%d\t%d\n",Huf[s].s[i],Huf[s].t[i],i);
           ++i;
        }
        }
    }
    T=i;
    }
    if(s==1){
    for(x=0;x<25715;x++){
        for(y=0;y<=q[x];y++){
            if(rl[x].L_cb[y]>=0){
                z=10*rl[x].L_cb[y]+rl[x].R_cb[y];
            }
            else
                z=10*rl[x].L_cb[y]-rl[x].R_cb[y];
            if(x==0&&y==0){
                temp1=z;
                t=0;
                a=1;
            }
            if((x==0&&y>0)||(x!=0&&y>=0)){
            k=0;
            while(1){

                if(z==Huf[s].s[k]){
                   a=0;
                   break;
                }
                else{
                    ++k;
                    if(k>=i){
                    a=1;
                    temp1=z;
                    t=0;
                    break;
                   }
                }
            }
           }
      if(a==1){
           for(m=0;m<25715;m++){
            for(n=0;n<=q[m];n++){
               if(rl[m].L_cb[n]>=0){
                Z=10*rl[m].L_cb[n]+rl[m].R_cb[n];
            }
               else{
                Z=10*rl[m].L_cb[n]-rl[m].R_cb[n];
               }
               temp2=Z;
               if(temp1==temp2){
                  ++t;
               }
            }
           }
           Huf[s].s[i]=temp1;
           Huf[s].t[i]=t;
           //g+=t;
           //printf("%d\t%d\t%d\n",Huf[s].s[i],Huf[s].t[i],i);
           ++i;
        }
        }
    }
    T=i;
    }
    if(s==2){
    for(x=0;x<25715;x++){
        for(y=0;y<=q[x];y++){
            if(rl[x].L_cr[y]>=0){
                z=10*rl[x].L_cr[y]+rl[x].R_cr[y];
            }
            else
                z=10*rl[x].L_cr[y]-rl[x].R_cr[y];
            if(x==0&&y==0){
                temp1=z;
                t=0;
                a=1;
            }
            if((x==0&&y>0)||(x!=0&&y>=0)){
            k=0;
            while(1){

                if(z==Huf[s].s[k]){
                   a=0;
                   break;
                }
                else{
                    ++k;
                    if(k>=i){
                    a=1;
                    temp1=z;
                    t=0;
                    break;
                   }
                }
            }
           }
      if(a==1){
           for(m=0;m<25715;m++){
            for(n=0;n<=q[m];n++){
               if(rl[m].L_cr[n]>=0){
                Z=10*rl[m].L_cr[n]+rl[m].R_cr[n];
            }
               else{
                Z=10*rl[m].L_cr[n]-rl[m].R_cr[n];
               }
               temp2=Z;
               if(temp1==temp2){
                  ++t;
               }
            }
           }
           Huf[s].s[i]=temp1;
           Huf[s].t[i]=t;
           //g+=t;
           //printf("%d\t%d\t%d\n",Huf[s].s[i],Huf[s].t[i],i);
           ++i;
        }
        }
    }
    T=i;
    }



}
void huf_dc(FILE *fp,int o){
    int k,y,s[K],c[K];
    printf("%d\n",K);

    for(k=0;k<K;++k){
        c[k]=huf[o].times[k];
        s[k]=huf[o].symbol[k];
        //printf("%d\t%d\t%d\n",k,symbol[k],times[k]);
    }
    quicksort(c,s,0,K-1);
struct data{
    int f[30];
    int C;
    int L;
};
    struct data da[K];
    for(k=0;k<K;++k){
     da[k].C=s[K-1-k];
    }
    int t,Temp,X,r,R[K-1],v=K-1;
    for(k=0;k<K-1;++k){
        if(k!=K-2){
        c[0]=c[0]+c[1];
        for(y=1;y<v;++y){
            c[y]=c[y+1];
            }

        X=c[0];
        t=1;
        while(X>c[t]){
            Temp=c[t];
            c[t]=X;
            c[t-1]=Temp;
            r=t;
            ++t;
            if(t>v-1)
                break;
        }
        R[k]=r;
        v=v-1;
        }
        else R[k]=0;
        }
    int n[30]={0},u[30]={0},N[K];
    int p,h,m,l,q;
    for(k=0;k<K;++k){
                h=0;
                l=0;
                p=k;
            for(y=0;y<K-1;++y){/**藉由編碼序來推出每個的編碼，只要相加一次位置就先往前shift1，然後判斷新位置如果小於等於編碼序則位置再往前shift1**/
                if((p==0)||(p==1)){
                    n[h]=p;
                    p=R[y];
                    ++h;
                }
                else{
                    p=p-1;
                    if(p<=R[y]){
                        p=p-1;
                    }
                }
            }
            for(m=h-1;m>=0;--m){
                u[l]=n[m];
                da[K-1-k].f[l]=u[l];
                ++l;
            }
            N[K-1-k]=l-1;
        }
        for(k=0;k<K;++k){
            p=N[k];
            da[k].L=p+1;
            //printf("%d\t%d\t",da[k].C,da[k].L);
            fprintf(fp,"%d\t",da[k].C);
            for(y=0;y<=p;++y){
                //printf("%d",da[k].f[y]);
                fprintf(fp,"%d",da[k].f[y]);
            }
            //printf("\n");
            fprintf(fp,"\r\n");
        }
        fclose(fp);

}
void huf_ac(FILE *fp,int e){
    int k,y,s[T],c[T];
    printf("%d\n",T);

    for(k=0;k<T;++k){
        c[k]=Huf[e].t[k];
        s[k]=Huf[e].s[k];
        //printf("%d\t%d\t%d\n",k,symbol[k],times[k]);
    }
    quicksort(c,s,0,T-1);

struct data{
    int f[30];
    int C;
    int L;
};
    struct data da[T];
    for(k=0;k<T;++k){
     da[k].C=s[T-1-k];
    }
    int t,Temp,X,r,R[T-1],v=T-1;
    for(k=0;k<T-1;++k){
        if(k!=T-2){
        c[0]=c[0]+c[1];
        for(y=1;y<v;++y){
            c[y]=c[y+1];
            }

        X=c[0];
        t=1;
        while(X>c[t]){
            Temp=c[t];
            c[t]=X;
            c[t-1]=Temp;
            r=t;
            ++t;
            if(t>v-1)
                break;
        }
        R[k]=r;
        v=v-1;
        }
        else R[k]=0;
        }
    int n[30]={0},u[30]={0},N[T];
    int p,h,m,l,q;
    for(k=0;k<T;++k){
                h=0;
                l=0;
                p=k;
            for(y=0;y<T-1;++y){/**藉由編碼序來推出每個的編碼，只要相加一次位置就先往前shift1，然後判斷新位置如果小於等於編碼序則位置再往前shift1**/
                if((p==0)||(p==1)){
                    n[h]=p;
                    p=R[y];
                    ++h;
                }
                else{
                    p=p-1;
                    if(p<=R[y]){
                        p=p-1;
                    }
                }
            }
            for(m=h-1;m>=0;--m){
                u[l]=n[m];
                da[T-1-k].f[l]=u[l];
                ++l;
            }
            N[T-1-k]=l-1;
        }
        for(k=0;k<T;++k){
            p=N[k];
            da[k].L=p+1;
            //printf("%d\t%d\t",da[k].C,da[k].L);
            fprintf(fp,"%d\t",da[k].C);
            for(y=0;y<=p;++y){
                //printf("%d",da[k].f[y]);
                fprintf(fp,"%d",da[k].f[y]);
            }
            //printf("\n");
            fprintf(fp,"\r\n");
        }
        fclose(fp);

}
