#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

#define limit 10000


#define dim 500000 // tamanho do array

#define num_buckets 20 // numero de buckets
#define tam_bucket dim/num_buckets + (dim/num_buckets)/3 // tamanho de cada bucket originalmente
//#define tam_bucket 300
struct timeval start, end;

struct bucket{
    int tam;
    int topo;   
    int *balde;
};
typedef struct bucket *Bucket;

/* 
nº buckets : pre-definido
range bucket - nº max do array / nº buckets
*/

void bucket_sort(int v[],int tam);                   //cabeçalho das funções
void bubble(int v[],int tam);                                                 
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}     
void bucket_sort(int v[],int max){
    int range = max/num_buckets + 1;
    printf("range: %d\n",range);
    Bucket *b = malloc(num_buckets * sizeof(Bucket));
    int i,j,k;
    /* 1 */ 
    
    for(i=0;i<num_buckets;i++){                  //inicializa todos os "topo"
        b[i] = malloc(sizeof(struct bucket));
        b[i]->tam = tam_bucket;
        b[i]->topo=0;
        b[i]->balde = (int *) malloc(sizeof(int)*tam_bucket);
    }



    /* 2 */ 
    for(i=0;i<dim;i++){                          //verifica em que balde o elemento deve ficar
        j = v[i] / range;
        if(b[j]->topo == b[j]->tam){
            // realloc
            b[j]->tam *= 2;
            b[j]->balde = (int *) realloc(b[j]->balde, b[j]->tam * sizeof(int));
        }

        b[j]->balde[b[j]->topo]=v[i];

        (b[j]->topo)++;
    }
    /* 3 */
    int n_threads = 4;
    //int n_threads = num_buckets/5;
    printf("numero de threads: %d\n",n_threads);
#pragma omp parallel num_threads(n_threads)
#pragma omp for
    for(i=0;i<num_buckets;i++){                     //ordena os baldes
        if(b[i]->topo){
            //bubble(b[i]->balde,b[i]->topo);
            qsort(b[i]->balde,b[i]->topo,sizeof(int),cmpfunc);
            //printf("\n\n----------------------\n");
            //int j;
            //printf("balde numero: %d\n",i);
            //for(j = 0; j< b[i]->topo; j++){
            //    printf("%d, ", b[i]->balde[j]);
            //}
            //printf("----------------------\n\n");
        }
    }

    i=0;
    /* 4 */ 
    for(j=0;j<num_buckets;j++){                    //põe os elementos dos baldes de volta no vetor
        for(k=0;k<b[j]->topo;k++){
            v[i]=b[j]->balde[k];
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
    printf("tam_bucket: %d\n",tam_bucket);
    int i;

    int *vetor = (int *) malloc(sizeof(int)*dim);

    int max = -1;

    for(i=0;i<dim;i++) {
        vetor[i] = rand() % limit;
        if(max < vetor[i]) max = vetor[i];
    }



    // Calculate the time taken by take_enter()
    gettimeofday(&start, NULL);

    bucket_sort(vetor,max);

    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("The elapsed time is %ld seconds and %ld micros\n", seconds, micros);


    /* for(i=0;i<dim;i++){
        printf("%d, ",vetor[i]);
    }
    puts("\n"); */
    return 0;
}
