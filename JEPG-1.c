#include <stdio.h>
#include <stdlib.h>
#include <math.h>
float PI=3.1415927;
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
Bitmap readheader(FILE* fp);
ImgRGB** malloc_2D(int row, int col);
void InputData(FILE* fp,ImgRGB **array,int H,int W);
void output_bmp(ImgRGB **RGB,FILE* outfile,Bitmap bmpheader);
void RGB2YCbCr(int rr,int gg,int bb,float *y,float *cb,float *cr);
void dct(float *pic_in,float *enc_out);
void quantize(float *dctb,int *qb,int n);
void dequantize(int *qb,int *dctb,int n);
void idct(int *enc_in,float *rec_out);
void YCbCr2RGB(float *y,float *cb,float *cr,float *r,float *g,float *b);
int main(int argc,char *argv[]){

    FILE* fp=fopen(argv[1],"rb");//argv[1]
    FILE *fw=fopen(argv[2],"wb");//argv[2]

    //read header
    Bitmap bmpheader=readheader(fp);

    ImgRGB **Data_RGB=malloc_2D(bmpheader.height, bmpheader.width);

	//read data
    InputData(fp,Data_RGB,bmpheader.height,bmpheader.width);
    printf("%d\n%d\n",bmpheader.width,bmpheader.height);
    struct a rgb[8][8];
    //struct buffer buf[8][8];
    /*put other else function here*/
    int x,y,u=0,v=0,m=8,n=8,R1,G1,B1,k=0,t=0,p,z;
    float Y,Cb,Cr;
    float buf_y1[8][8],buf_cb1[8][8],buf_cr1[8][8],DCT1[8][8],DCT2[8][8],DCT3[8][8],IDCT1[8][8],IDCT2[8][8],IDCT3[8][8];
    int quant1[8][8],quant2[8][8],quant3[8][8],dquant1[8][8],dquant2[8][8],dquant3[8][8];
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
           }
       }
       dct(&buf_y1[0][0],&DCT1[0][0]);/**把每個8*8block去做dct，quantize，dequantize，idct，最後轉回RGB**/
       dct(&buf_cb1[0][0],&DCT2[0][0]);
       dct(&buf_cr1[0][0],&DCT3[0][0]);
       quantize(&DCT1[0][0],&quant1[0][0],0);
       quantize(&DCT2[0][0],&quant2[0][0],1);
       quantize(&DCT3[0][0],&quant3[0][0],1);
       dequantize(&quant1[0][0],&dquant1[0][0],0);
       dequantize(&quant2[0][0],&dquant2[0][0],1);
       dequantize(&quant3[0][0],&dquant3[0][0],1);
       idct(&dquant1[0][0],&IDCT1[0][0]);
       idct(&dquant2[0][0],&IDCT2[0][0]);
       idct(&dquant3[0][0],&IDCT3[0][0]);
       YCbCr2RGB(&IDCT1[0][0],&IDCT2[0][0],&IDCT3[0][0],&buf_y1[0][0],&buf_cb1[0][0],&buf_cr1[0][0]);

       for(x=0;x<n;++x){/**把轉換回來的資料寫入**/
        for(y=0;y<m;++y){
            Data_RGB[x+8*v][y+8*u].R=buf_y1[x][y];
            Data_RGB[x+8*v][y+8*u].G=buf_cb1[x][y];
            Data_RGB[x+8*v][y+8*u].B=buf_cr1[x][y];
        }
       }
       u=u+1;
       if(u>138){
        u=0;
        v=v+1;
       }
    }
    printf("END\n");
    /*
    RGB->YCbCr
    8*8 blocking
        DCT
        Quantization
        Inverse Quantization
        Inverse DCT

    YCbCr->RGB
    */



    //output bmp
    output_bmp(Data_RGB,fw,bmpheader);

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
void idct(int *enc_in,float *rec_out){
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
        }
    }
}
void quantize(float *dctb,int *qb,int n){
    int u,v;
    for(v=0;v<8;v++){
        for(u=0;u<8;u++){
            if(n==0)
                qb[v*8+u]=(int)(dctb[v*8+u]/q0[v][u]);
            else
                qb[v*8+u]=(int)(dctb[v*8+u]/q1[v][u]);
        }
    }
}
void dequantize(int *qb,int *dctb,int n){
    int u,v;
    for(v=0;v<8;v++){
        for(u=0;u<8;u++){
            if(n==0)
                dctb[v*8+u]=qb[v*8+u]*q0[v][u];
            else
                dctb[v*8+u]=qb[v*8+u]*q1[v][u];
        }
    }
}
