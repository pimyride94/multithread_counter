#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define MAX_WORKERS 4

typedef struct {
    int ID;
    uint64_t nprimes;
    uint64_t *val;
    int numval;
} thread_args;

int n_workers = 0;
pthread_t workers[MAX_WORKERS];
int worker_status[MAX_WORKERS];
thread_args worker_data[MAX_WORKERS];


int isprime(uint64_t value)
{
    if(value == 0) return 0;
    if(value == 1) return 0;
    for (uint64_t i=2; i< value; i++) {
        if(value%i == 0) return 0;
    }
    return 1;
}

char * tokeniz(char *str,uint64_t *valores, int *numval){
    
    *numval = 0;
    char *token;
    int len, len2;
    int i;
    
    token = strtok(str," ");
    
    while( token != NULL ) {
        sscanf(token,"%lld",&valores[*numval]);
        (*numval)++;
        token = strtok(NULL, " ");
    }
    
    return token;
}



void* worker(void *arg) {
    thread_args *info = (thread_args *)arg;
    for (int i=info->ID; i<info->numval; i+=MAX_WORKERS) {
        int ispr = isprime(info->val[i]);
        if(ispr) info->nprimes++;
    }
    return NULL;
}

int main() {
    
    int i=0;
    uint64_t val[1024]; /*vetor de numeros a ser analisado */
    int num_val; /*numero de valores lidos do arquivo*/
    /* index do processo */
    
    char *s=malloc(sizeof(char)*10000);
    
    fgets (s, 10000, stdin);
    s[strlen(s)-1] = '\0';
    tokeniz(s,val,&num_val);
    
    for(int i = 0; i<MAX_WORKERS; i++)
    {
        worker_data[i].ID = i;
        worker_data[i].nprimes = 0;
        worker_data[i].val = val;
        worker_data[i].numval = num_val;
        pthread_create(& (workers[i]), NULL, worker, (void*) &worker_data[i]);
    }
    

    uint64_t nprimes = 0;
    for (int i=0; i<MAX_WORKERS; i++) {
        pthread_join(workers[i], NULL);
        nprimes += worker_data[i].nprimes;
    }
    printf("%lld\n",nprimes);
    
    return 0;
}
