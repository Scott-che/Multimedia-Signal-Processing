#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc,char *argv[])
{   FILE *fp=fopen(argv[2],"rb");//argv[2]
    FILE *fp2=fopen(argv[1],"rb");//argv[1]
    FILE *fp3=fopen(argv[3],"wb");//argv[3]
    unsigned char a;
    int temp,i=0,j=0,k=0;
    struct Huffman{
    char number[5];
    int code[30];
    int codelen;
    };
    struct Huffman huf[102];/**建立struct來存codebook的資料**/
    for(i=0;i<102;i++){
        huf[i].number[0]=0;
        huf[i].number[1]=0;
        huf[i].number[2]=0;
        huf[i].number[3]=0;
        huf[i].number[4]=0;
    }
    for(i=0;i<102;i++){/**讀取codebook**/
    while(!feof(fp)){
            a=fgetc(fp);
            temp=a;
            if(temp!=9){
                huf[i].number[j]=temp;
                ++j;
            }
            else if(temp==9){
                break;
                }
            }
            j=0;
    while(!feof(fp)){
            a=fgetc(fp);
            temp=a;
            if(temp!=13){
                huf[i].code[k]=temp;
                ++k;
            }
            else if(temp==13){
                a=fgetc(fp);
                break;
            }

            }
            huf[i].codelen=k;
            k=0;
    }
    int s[102],buffer[30]={0},v,r=0,n,test;

      for(i=0;i<102;++i){/**把codebook的資料由ascii轉成0~9之間的數字**/
          test=huf[i].number[2]-48;
            if(test>=0){
                s[i]=100*(huf[i].number[0]-48)+10*(huf[i].number[1]-48)+(huf[i].number[2]-48);
            }
            else{
                s[i]=10*(huf[i].number[0]-48)+(huf[i].number[1]-48);}
            }
      for(i=0;i<102;++i){
        for(k=0;k<huf[i].codelen;++k){
            huf[i].code[k]=huf[i].code[k]-48;
        }
      }
     i=0;
     r=0;
     int z;
    while(!feof(fp2)){

            fread(&a,1,1,fp2);

            if(feof(fp2)){
                for(i=0;i<8;++i){
                    if(i<r){
                        buffer[8-r+i]=buffer[i];
                    }
                    else
                        buffer[i]=0;
                }
                n=buffer[0]*128+buffer[1]*64+buffer[2]*32+buffer[3]*16+buffer[4]*8+buffer[5]*4+buffer[6]*2+buffer[7];
                fprintf(fp3,"%c",n);
                break;
                }
            else {/**讀取input的資料，比對codebook，滿8個bit就輸出**/

            for(k=0;k<102;++k){
                if(a==s[k]){
                    v=k;
                }
            }
            for(k=0;k<huf[v].codelen;++k){
                buffer[r+k]=huf[v].code[k];
            }
            r=(r+k);

            if(r>7&&r<16&&r!=8){/**分成了不同區間，分別輸出1or2or3組**/
              n=buffer[0]*128+buffer[1]*64+buffer[2]*32+buffer[3]*16+buffer[4]*8+buffer[5]*4+buffer[6]*2+buffer[7];
              fprintf(fp3,"%c",n);
              for(j=0;j<r-8;++j){
                buffer[j]=buffer[8+j];
              }
              r=j;
              for(i=r;i<=7;++i){
                buffer[i]=0;
              }
            }

              else if(r>15&&r<24&&r!=16){
                n=buffer[0]*128+buffer[1]*64+buffer[2]*32+buffer[3]*16+buffer[4]*8+buffer[5]*4+buffer[6]*2+buffer[7];
                fprintf(fp3,"%c",n);
                n=buffer[8]*128+buffer[9]*64+buffer[10]*32+buffer[11]*16+buffer[12]*8+buffer[13]*4+buffer[14]*2+buffer[15];
                fprintf(fp3,"%c",n);
                for(j=0;j<r-16;++j){
                buffer[j]=buffer[16+j];
              }
              r=j;
              for(i=r;i<=7;++i){
                buffer[i]=0;
              }
            }

             else if(r==24){
                n=buffer[0]*128+buffer[1]*64+buffer[2]*32+buffer[3]*16+buffer[4]*8+buffer[5]*4+buffer[6]*2+buffer[7];
                fprintf(fp3,"%c",n);
                n=buffer[8]*128+buffer[9]*64+buffer[10]*32+buffer[11]*16+buffer[12]*8+buffer[13]*4+buffer[14]*2+buffer[15];
                fprintf(fp3,"%c",n);
                n=buffer[16]*128+buffer[17]*64+buffer[18]*32+buffer[19]*16+buffer[20]*8+buffer[21]*4+buffer[22]*2+buffer[23];
                fprintf(fp3,"%c",n);
                r=0;
                for(i=0;i<=7;++i){
                    buffer[i]=0;
                }
             }

             else if(r==8){
                n=buffer[0]*128+buffer[1]*64+buffer[2]*32+buffer[3]*16+buffer[4]*8+buffer[5]*4+buffer[6]*2+buffer[7];
                fprintf(fp3,"%c",n);
                r=0;
                for(i=0;i<=7;++i){
                    buffer[i]=0;
                }
            }

            else if(r==16){
                n=buffer[0]*128+buffer[1]*64+buffer[2]*32+buffer[3]*16+buffer[4]*8+buffer[5]*4+buffer[6]*2+buffer[7];
                fprintf(fp3,"%c",n);
                n=buffer[8]*128+buffer[9]*64+buffer[10]*32+buffer[11]*16+buffer[12]*8+buffer[13]*4+buffer[14]*2+buffer[15];
                fprintf(fp3,"%c",n);
                r=0;
                for(i=0;i<=7;++i){
                    buffer[i]=0;
                }
            }
           }
    }
    printf("END");

    return 0;
}
