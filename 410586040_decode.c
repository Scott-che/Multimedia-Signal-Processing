#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(int argc,char *argv[])
{
    FILE *fp=fopen(argv[1],"rb");
    FILE *fp2=fopen(argv[2],"rb");
    FILE *fp3=fopen(argv[3],"wb");
    int a,n;
    char UTF8_Head[]={"\xef\xbb\xbf"};
    int temp,i=0,j=0,k=0,e;
    struct Huffman{
    int number[5];
    int code[30];
    int codelen;
    };
    struct Huffman huf[102];
    for(i=0;i<102;i++){
        huf[i].number[0]=0;
        huf[i].number[1]=0;
        huf[i].number[2]=0;
        huf[i].number[3]=0;
        huf[i].number[4]=0;
    }
    fwrite(UTF8_Head,1,3,fp3);
    for(i=0;i<102;i++){
    while(!feof(fp2)){/**讀取codebook**/
            a=fgetc(fp2);
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
    while(!feof(fp2)){
            a=fgetc(fp2);
            temp=a;
            if(temp!=13){
                huf[i].code[k]=temp;
                ++k;
            }
            else if(temp==13){
                a=fgetc(fp2);
                break;
            }

            }
            huf[i].codelen=k;
            k=0;
    }
    int s[102],test;

    for(i=0;i<102;++i){/**把codebook的資料由ascii轉成0~9之間的整數**/
            test=huf[i].number[2]-48;
            if(test>=0){
                s[i]=100*(huf[i].number[0]-48)+10*(huf[i].number[1]-48)+(huf[i].number[2]-48);
            }
            else
                s[i]=10*(huf[i].number[0]-48)+(huf[i].number[1]-48);
            }
      for(i=0;i<102;++i){
        for(k=0;k<huf[i].codelen;++k){
            huf[i].code[k]=huf[i].code[k]-48;
        }
      }

     int buffer[24]={0},q=0,t,p=0,v=0,last=0,buf_len=0,size;
     size=0;
     while(1){
        fread(&a,1,1,fp);
        //k=a;
        if(feof(fp)){
            for(k=8-buf_len;k<8;++k){/**最後一個byte在前面補零輸出**/
                buffer[k]=buffer[k-(8-buf_len)];
            }
            for(k=0;k<8-buf_len;++k){
                buffer[k]=0;
            }

            for(q=0;q<=101;q++){
                t=0;
             while((buffer[t]==huf[q].code[t])&&(buf_len>=huf[q].codelen)){
                ++t;
                if(t==huf[q].codelen){
                    n=s[q];
                    fprintf(fp3,"%c",n);
                    q=102;
                    break;
                }
             }
            }
            break;
        }
        j=1;
        for(i=0;i<8;++i){
              k=a%(j*2)/j;
              buffer[buf_len+7-i]=k;/**把讀進來的數字轉成二進位**/
              j*=2;
           }
           buf_len=buf_len+i;


        for(q=0;q<=101;q++){/**一次先讀一個byte進來，然後比對，如果沒找到相同的則在讀進一個byte**/
                t=0;
             while((buffer[t]==huf[q].code[t])&&(buf_len>=huf[q].codelen)){
                ++t;
                if(t==huf[q].codelen){
                    n=s[q];
                    size=size+1;
                    if(size<=510391){
                    fprintf(fp3,"%c",n);}


                    for(k=0;k<buf_len-t;++k){/**把未輸出的往前shift，後面則歸零**/
                        buffer[k]=buffer[k+t];
                    }
                    for(k=buf_len-t;k<buf_len;++k){
                        buffer[k]=0;
                    }
                    buf_len=buf_len-t;
                    q=0;

                    break;

                }

             }

        }

     }
        printf("end");

    return 0;
}
