#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#define max 10
#define limit 100000


int dim;
int tam_bucket;
int num_bucket;

struct timeval start, end;

typedef struct {
        int topo;
        int *balde;
}bucket;
 
void bucket_sort(int v[],int tam);                   //cabeçalho das funções
void bubble(int v[],int tam);                                                 
                                                          
void bucket_sort(int v[],int tam){                                     
        bucket b[num_bucket];                                      
        int i,j,k;                                                 
/* 1 */ for(i=0;i<num_bucket;i++){                  //inicializa todos os "topo"
                b[i].topo=0;
                b[i].balde = (int *) malloc(sizeof(int)*tam_bucket);
        }
 
/* 2 */ for(i=0;i<tam;i++){                          //verifica em que balde o elemento deve ficar
                j=(num_bucket)-1;
                while(1){
                         if(j<0)
                                 break;
                         if(v[i]>=j*10){
                                 b[j].balde[b[j].topo]=v[i];
                                 (b[j].topo)++;
                                 break;
                         }
                         j--;
                }
 }
         
 /* 3 */ for(i=0;i<num_bucket;i++)                     //ordena os baldes
                 if(b[i].topo)
                         bubble(b[i].balde,b[i].topo);
         
         i=0;
 /* 4 */ for(j=0;j<num_bucket;j++){                    //põe os elementos dos baldes de volta no vetor
                 for(k=0;k<b[j].topo;k++){
                         v[i]=b[j].balde[k];
                         i++;
                 }
         }
 }
 
void bubble(int v[],int tam){
        int i,j,temp,flag;
        if(tam)
        for(j=0;j<tam-1;j++){
                flag=0;
                for(i=0;i<tam-1;i++){
                        if(v[i+1]<v[i]){
                                 temp=v[i];
                                 v[i]=v[i+1];
                                 v[i+1]=temp;
                                 flag=1;
                        }
                }
                if(!flag)
                break;
        }
}

int main(){

        int i;
       
        printf("Give length of array to sort \n");
        if (scanf("%d", &dim) != 1){
                printf("error\n");
                return -1;
        }
        printf("Give number of buckets \n");
        if (scanf("%d", &num_bucket) != 1){
                printf("error\n");
                return -1;
        }

        int *vetor = (int *) malloc(sizeof(int)*dim);
        tam_bucket = (int)limit/num_bucket;

        for(i=0;i<dim;i++) {
                vetor[i] = random() % limit;
        }



        // Calculate the time taken by take_enter()
        gettimeofday(&start, NULL);
        bucket_sort(vetor,dim);
        gettimeofday(&end, NULL);
        long seconds = (end.tv_sec - start.tv_sec);
        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
        printf("The elapsed time is %ld seconds and %ld micros\n", seconds, micros);


}
