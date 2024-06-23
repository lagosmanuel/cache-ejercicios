#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CSIZE 4096
#define BSIZE 16
#define DSIZE 4
#define BASEA 1024
#define BASEB 67584
#define N 1024

typedef struct entrada {
    int tag;
    int valor;
    int valido;
} * tEntrada;

typedef struct cache {
    tEntrada * datos;
    int accesos;
    int misses;
} * tCache;

int posToAddress(int i, int BASE) {
    return BASE + i * DSIZE;
}

int cache_access(tCache cache, int address) {
    int offsetbits = (int) log2(BSIZE);
    int setbits = (int) log2(CSIZE / BSIZE);

    int set = (address >> offsetbits) & (int) (pow(2, setbits) - 1); 
    int tag = address >> (offsetbits + setbits);
   
    cache->accesos += 1;
    tEntrada entrada = cache->datos[set];

    int res = 1;

    if (entrada->tag != tag || !entrada->valido) {
        entrada->tag = tag;
        entrada->valido = 1; 
        cache->datos[set] = entrada;
        cache->misses += 1;
        res = 0;
    } 
    
    return res;
}

void recorrido(tCache cache) {
    for (int i = 0; i < N; i++) {
       cache_access(cache, posToAddress(i, BASEA));
       cache_access(cache, posToAddress(i, BASEA));
       cache_access(cache, posToAddress(i, BASEB));
    }
}

int main() {
    tCache cache = (tCache) malloc(sizeof(struct cache));
    cache->accesos = 0;
    cache->misses = 0;
    cache->datos = (tEntrada *) malloc(sizeof(tEntrada) * (CSIZE / BSIZE));

    for (int i = 0; i < (CSIZE / BSIZE); i++) {
        tEntrada entrada = (tEntrada) malloc(sizeof(struct entrada));
        entrada->valido = 0;
        cache->datos[i] = entrada;
    } 
    
    recorrido(cache);
    printf("accesos: %d, misses: %d\n", cache->accesos, cache->misses);
    
    for (int i = 0; i < (CSIZE / BSIZE); i++) free(cache->datos[i]);
    free(cache->datos);
    free(cache);

    return 0;
}
