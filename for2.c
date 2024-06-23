#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CSIZE 4096
#define BSIZE 16
#define DSIZE 4
#define BASEA 16416
#define BASEB 67584
#define N 32
#define GA 2

typedef struct entrada {
    int tag0;
    int valor0;
    int valido0;
    int tag1;
    int valor1;
    int valido1;
    int m;
} * tEntrada;

typedef struct cache {
    tEntrada * datos;
    int accesos;
    int misses;
} * tCache;

int posToAddress(int i, int j, int BASE) {
    return BASE + (i * N + j ) * DSIZE;
}

int cache_access(tCache cache, int address) {
    int offsetbits = (int) log2(BSIZE);
    int setbits = (int) log2(CSIZE / BSIZE / GA);

    int set = (address >> offsetbits) & (int) (pow(2, setbits) - 1); 
    int tag = address >> (offsetbits + setbits);
   
    cache->accesos += 1;
    tEntrada entrada = cache->datos[set];

    int res = 1;

    if ((entrada->tag0 != tag || !entrada->valido0) && (entrada->tag1 != tag || !entrada->valido1)) {
        if (!entrada->valido0 || (!entrada->m && entrada->valido1)) {
            entrada->tag0 = tag;
            entrada->valido0 = 1; 
            entrada->m = 1;
        } else if (!entrada->valido1 || (entrada->m && entrada->valido0)) {
            entrada->tag1 = tag;
            entrada->valido1 = 1;
            entrada->m = 0;
        }
        
        cache->misses += 1;
        res = 0;
    } else {
        if (entrada->tag0 == tag) entrada->m = 1;
        else entrada->m = 0;
    }

    cache->datos[set] = entrada;
    return res;
}

void recorrido(tCache cache) {
    // macro para LibreOffice 
    printf("Sub ColorearCeldasRojo()\n\tDim oDoc As Object\n\tDim oSheet As Object\n\tDim oCell As Object\n\tDim i As Integer\n\n\t' Obtener el documento actual\n\toDoc = ThisComponent\n\n\t' Obtener la hoja de cálculo activa\n\toSheet = oDoc.CurrentController.ActiveSheet\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!cache_access(cache, posToAddress(i, j, BASEA))) ;//printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            if (!cache_access(cache, posToAddress(j, i, BASEB))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", i, j);
            if (!cache_access(cache, posToAddress(i, j, BASEA))) ;//printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            if (!cache_access(cache, posToAddress(j, i, BASEB))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", i, j);
        }
    }

    printf("End sub\n");
}

int main() {
    tCache cache = (tCache) malloc(sizeof(struct cache));
    cache->accesos = 0;
    cache->misses = 0;
    cache->datos = (tEntrada *) malloc(sizeof(tEntrada) * (CSIZE / BSIZE / GA));

    for (int i = 0; i < (CSIZE / BSIZE / GA); i++) {
        tEntrada entrada = (tEntrada) malloc(sizeof(struct entrada));
        entrada->valido0 = 0;
        entrada->valido1 = 0;
        entrada->m = 0;
        cache->datos[i] = entrada;
    } 
    
    recorrido(cache);
    printf("accesos: %d, misses: %d\n", cache->accesos, cache->misses);
    
    for (int i = 0; i < (CSIZE / BSIZE / GA); i++) free(cache->datos[i]);
    free(cache->datos);
    free(cache);

    return 0;
}
