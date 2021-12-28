#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#define limit 10000


#define dim 50000 // tamanho do array
#define tam_bucket 300 // tamanho de cada bucket originalmente
#define num_buckets 20 // numero de buckets

struct timeval start, end;

typedef struct {
    int tam;
    int topo;   
    int *balde;
}bucket;

/* 
nº buckets : pre-definido
range bucket - nº max do array / nº buckets
*/

void bucket_sort(int v[],int tam);                   //cabeçalho das funções
void bubble(int v[],int tam);                                                 
        
void bucket_sort(int v[],int max){
    int range = max/num_buckets + 1;
    bucket b[num_buckets];                                      
    int i,j,k;                                                 
    /* 1 */ 
    
    for(i=0;i<num_buckets;i++){                  //inicializa todos os "topo"
        b[i].tam = tam_bucket;
        b[i].topo=0;
        b[i].balde = (int *) malloc(sizeof(int)*tam_bucket);
    }

    
/*     10

    100

    range = 10 // 100 / 10

    0 - 9
    10 - 19
    20 -29
    30 - 39
    40 - 49
    50 - 59
    60 - 69
    70 - 79
    80 - 89
    90 - 99

    v[i] = 78 / n_buckets ?? */


    /* 2 */ 
    for(i=0;i<dim;i++){                          //verifica em que balde o elemento deve ficar
        j = v[i] / range;
        if(b[j].topo == b[j].tam){
            // realloc
            b[j].tam *= 2;
            b[j].balde = (int *) realloc(b[j].balde, b[j].tam * sizeof(int));
        }

        b[j].balde[b[j].topo]=v[i];

        (b[j].topo)++;
    }
    /* 3 */ 
    for(i=0;i<num_buckets;i++)                     //ordena os baldes
        if(b[i].topo)
            bubble(b[i].balde,b[i].topo);

    i=0;
    /* 4 */ 
    for(j=0;j<num_buckets;j++){                    //põe os elementos dos baldes de volta no vetor
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

    int *vetor = (int *) malloc(sizeof(int)*dim);

    int max = -1;

    for(i=0;i<dim;i++) {
        vetor[i] = random() % limit;
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
}
