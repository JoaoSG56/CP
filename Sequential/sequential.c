#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>

// PAPI events to monitor
// #define NUM_EVENTS 4
// int Events[NUM_EVENTS] = { PAPI_TOT_CYC, PAPI_TOT_INS , PAPI_L1_DCM, PAPI_L2_DCM};
// // PAPI counters' values
// long long values[NUM_EVENTS], min_values[NUM_EVENTS];
// int retval, EventSet=PAPI_NULL;


#define limit 10000


#define dim 500000 // tamanho do array

#define num_buckets 500 // numero de buckets
#define tam_bucket dim/num_buckets + (dim/num_buckets)/3 // tamanho de cada bucket originalmente

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
    //bucket b[num_buckets];
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
    for(i=0;i<num_buckets;i++)                     //ordena os baldes
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
    long long start_usec, end_usec, elapsed_usec, min_usec=0L;
    printf("tam_bucket: %d\n",tam_bucket);
    int i;

    int *vetor = (int *) malloc(sizeof(int)*dim);

    int max = -1;

    for(i=0;i<dim;i++) {
        vetor[i] = rand() % limit;
        if(max < vetor[i]) max = vetor[i];
    }



    // // Calculate the time taken by take_enter()
    // retval = PAPI_library_init(PAPI_VER_CURRENT);
    // if (retval != PAPI_VER_CURRENT) {
    //     fprintf(stderr,"PAPI library init error!\n");
    //     return 0;
    // }

    // /* create event set */
    // if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
    //     fprintf(stderr,"PAPI create event set error\n");
    //     return 0;
    // }


    // /* Get the number of hardware counters available */
    // if ((num_hwcntrs = PAPI_num_hwctrs()) <= PAPI_OK)  {
    //     fprintf (stderr, "PAPI error getting number of available hardware counters!\n");
    //     return 0;
    // }
    // fprintf(stdout, "done!\nThis system has %d available counters.\n\n", num_hwcntrs);

    // // We will be using at most NUM_EVENTS counters
    // if (num_hwcntrs >= NUM_EVENTS) {
    //     num_hwcntrs = NUM_EVENTS;
    // } else {
    //     fprintf (stderr, "Error: there aren't enough counters to monitor %d events!\n", NUM_EVENTS);
    //     return 0;
    // }

    // if (PAPI_add_events(EventSet,Events,NUM_EVENTS) != PAPI_OK)  {
    //     fprintf(stderr,"PAPI library add events error!\n");
    //     return 0;
    // }
  
    


    // // use PAPI timer (usecs) - note that this is wall clock time
    // // for process time running in user mode -> PAPI_get_virt_usec()
    // // real and virtual clock cycles can also be read using the equivalent
    // // PAPI_get[real|virt]_cyc()
    // start_usec = PAPI_get_real_usec();

    // /* Start counting events */
    // if (PAPI_start(EventSet) != PAPI_OK) {
    //     fprintf (stderr, "PAPI error starting counters!\n");
    //     return 0;
    // }

    bucket_sort(vetor,max);

    
    // /* Stop counting events */
    // if (PAPI_stop(EventSet,values) != PAPI_OK) {
    //     fprintf (stderr, "PAPI error stoping counters!\n");
    //     return 0;
    // }

    // end_usec = PAPI_get_real_usec();
    // fprintf (stdout, "done!\n");

    // elapsed_usec = end_usec - start_usec;

    // if ((run==0) || (elapsed_usec < min_usec)) {
    //     min_usec = elapsed_usec;
    //     for (i=0 ; i< NUM_EVENTS ; i++) min_values[i] = values [i];
    // }

    // fprintf (stdout,"\nWall clock time: %lld usecs\n", min_usec);

    // // output PAPI counters' values
    // for (i=0 ; i< NUM_EVENTS ; i++) {
    //     char EventCodeStr[PAPI_MAX_STR_LEN];

    //     if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
    //         fprintf (stdout, "%s = %lld\n", EventCodeStr, min_values[i]);
    //     } else {
    //         fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", min_values[i]);
    //     }
    // }

    
   


    // for(i=0;i<dim;i++){
    //     printf("%d, ",vetor[i]);
    // }
    //puts("\n"); 
    return 0;
}
