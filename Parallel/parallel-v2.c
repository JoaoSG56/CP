#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

#include <math.h>

#include "papi.h"


#define limit 50000000
// #define dim 500000000 // tamanho do array

#define num_buckets 100 // numero de buckets
// #define tam_bucket dim/num_buckets + (dim/num_buckets)/3 // tamanho de cada bucket originalmente
//#define tam_bucket 300
// #define n_threads 4

typedef struct bucket{
    int tam;
    int topo;   
    int *balde;
}*Bucket;


/* 
nº buckets : pre-definido
range bucket - nº max do array / nº buckets
*/


void bubble(int v[],int tam);                                                 
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}     
void bucket_sort(int v[],int max,int dim,int num_buckets,int n_threads){

    int tam_bucket = dim/num_buckets + (dim/num_buckets)/3; // tamanho de cada bucket originalmente
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
    
    //int n_threads = num_buckets/5;
    printf("numero de threads: %d\n",n_threads);
#pragma omp parallel for schedule(static) num_threads(n_threads)
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

int main(int argc, char **argv){
    double start_usec, end_usec, elapsed_usec;
    int dim = atoi(argv[1]);
    int n_threads = atoi(argv[2]);

    printf("[PARALELO]------------ dim: %d ---------- n_threads: %d",dim,n_threads);
    printf("num_buckets: %d\n",num_buckets);
    int i;

    int *vetor = (int *) malloc(sizeof(int)*dim);

    int max = -1;

    for(i=0;i<dim;i++) {
        vetor[i] = rand() % limit;
        if(max < vetor[i]) max = vetor[i];
    }

    

    start_usec = omp_get_wtime();



    bucket_sort(vetor,max,dim,num_buckets,n_threads);


    end_usec = omp_get_wtime();
    fprintf (stdout, "done!\n");

    elapsed_usec = end_usec - start_usec;



    fprintf (stdout,"\nWall clock time: %f usecs\n", elapsed_usec);



    int flag = 1;
    int first = vetor[0];
    for(i=1;i<dim;i++){
        //printf("%d, ",vetor[i]);
        if(first>vetor[i]){
            flag = 0;
            break;
        }
        first = vetor[i];
    }

    printf("Ordenado : %d\n",flag);
    printf("-----------------------------\n");
    /* for(i=0;i<dim;i++){
        printf("%d, ",vetor[i]);
    }
    puts("\n"); */
    return 0;
}
