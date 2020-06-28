#include <stdio.h>
#include <stdlib.h>
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
void quicksort2(int E[],int first2,int last2){
    int i2,j2,pivot2,temp3;
    if(first2<last2){
        pivot2=first2;
        i2=first2;
        j2=last2;
        while(i2<j2){
            while(E[i2]<=E[pivot2]&&i2<last2)
                i2++;
            while(E[j2]>E[pivot2])
                j2--;

            if(i2<j2){
                temp3=E[i2];
                E[i2]=E[j2];
                E[j2]=temp3;
            }
        }
        temp3=E[pivot2];
        E[pivot2]=E[j2];
        E[j2]=temp3;
        quicksort2(E,first2,j2-1);
        quicksort2(E,j2+1,last2);
    }
}
int main(int argc, char *argv[])
{   FILE *fp=fopen(argv[1],"rb");//argv[1]
    FILE *fp2=fopen(argv[2],"wb");//argv[2]
    int x,y,b[256];
    unsigned char a;
    for(x=0;x<256;++x){
        b[x]=0;
    }
    while(1){
        fread(&a,1,1,fp);/**這邊是讀檔順便計算次數**/

        if(feof(fp)){
            break;
            }
        b[a]+=1;
    }
    printf("finish read\n");

fclose(fp);
    int c[256]={0},d[102]={0};
    int k=0;
    for(y=0;y<256;y++){
        if(b[y]>0){
            c[k]=y;
            d[k]=b[y];
            ++k;
        }
    }
    quicksort(d,c,0,101);/**把參數輸入，然後藉由quicksort由小排到大**/
    /*for(k=0;k<102;++k){
      printf("%d\t%d\t%d\n",k,c[k],d[k]);
    }*/

struct data{
    int f[20];
    int C;
    int L;
};
    struct data da[102];

    for(k=0;k<102;++k){
     da[k].C=c[101-k];
    }
    int t,Temp,X,r,R[101],v=101;/**最小的兩項相加，然後再由小排到大，記下編碼序**/
    for(k=0;k<101;++k){
        if(k!=100){
        d[0]=d[0]+d[1];
        for(y=1;y<v;++y){
            d[y]=d[y+1];
            }

        X=d[0];
        t=1;
        while(X>d[t]){
            Temp=d[t];
            d[t]=X;
            d[t-1]=Temp;
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
int n[20],u[20],N[102],t1[20];
int p,h,m,l,q;
        for(k=0;k<102;++k){
                h=0;
                l=0;
                p=k;
            for(y=0;y<101;++y){/**藉由編碼序來推出每個的編碼，只要相加一次位置就先往前shift1，然後判斷新位置如果小於等於編碼序則位置再往前shift1**/
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
                da[101-k].f[l]=u[l];
                ++l;
            }
            N[101-k]=l-1;
        }
        for(k=0;k<102;++k){
            p=N[k];
            da[k].L=p+1;



        }
        int s[102],w=0;
        struct output{
        int K;
        int F[20];
        };
        struct output out[102];
        for(l=0;l<10;l++){/**把編碼長度相同的按照ascii大小來換位置**/
            q=1;
          while(da[w].L==da[w+q].L){
            ++q;
            if((w+q)>102){
                break;
            }
        }
        q=q-1;
        for(y=0;y<=q;++y){
            s[y]=da[y+w].C;
        }
        quicksort2(s,0,q);
        for(y=0;y<=q;++y){
            out[y+w].K=s[y];
            for(k=0;k<=q;++k){
                if(out[y+w].K==da[k+w].C){
                    for(h=0;h<da[k+w].L;++h){
                        out[y+w].F[h]=da[k+w].f[h];
                    }
                }
            }
        }
        w=w+q+1;
        if((w==79)||(w==76)||(w==91)){
            for(h=0;h<da[w].L;++h){
                        out[w].K=da[w].C;
                        out[w].F[h]=da[w].f[h];
                    }
            w=w+1;
        }
        }
        for(k=0;k<102;++k){/**把codebook輸出**/
            p=N[k];
            da[k].L=p+1;
            //printf("%d\t",out[k].K);
            fprintf(fp2,"%d\t",out[k].K);
                for(y=0;y<=p;++y){
                    //printf("%d",out[k].F[y]);
                    fprintf(fp2,"%d",out[k].F[y]);
            }
            //printf("\n");
            fprintf(fp2,"\r\n");
        }
    return 0;
}
