#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CSIZE 512
#define BSIZE 32
#define DSIZE 4
#define BASE 1040
#define N 32

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

int posToAddress(int i, int j) {
    return BASE + (i * N + j ) * DSIZE;
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

void cuadrados(tCache cache) {
    int matriz[N][N];
    
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matriz[i][j] = i * N + j;        
    
    int i = 0;
    int j = 0;
    
    int s = 0;
    int c = 1;
    
    // macro para LibreOffice 
    printf("Sub ColorearCeldasRojo()\n\tDim oDoc As Object\n\tDim oSheet As Object\n\tDim oCell As Object\n\tDim i As Integer\n\n\t' Obtener el documento actual\n\toDoc = ThisComponent\n\n\t' Obtener la hoja de cálculo activa\n\toSheet = oDoc.CurrentController.ActiveSheet\n");
    
    if(!cache_access(cache, posToAddress(i, j))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
    
    while ((s != 0 || i+1 < N) && (s != 1 || j+1 < N)) {
        if (s == 0) {
            if(!cache_access(cache, posToAddress(++i, j))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            
            for (int k = 0; k < c; k++)
                if(!cache_access(cache, posToAddress(i, ++j))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            for (int k = 0; k < c; k++)
                if(!cache_access(cache, posToAddress(--i, j))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            
            s = 1;    
        } else {
            if(!cache_access(cache, posToAddress(i, ++j))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            
            for (int k = 0; k < c; k++)
                if(!cache_access(cache, posToAddress(++i, j))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            for (int k = 0; k < c; k++)
                if(!cache_access(cache, posToAddress(i, --j))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", j, i);
            
            s = 0;    
        }
        
        c++;
    }

    printf("End sub\n");
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
    
    cuadrados(cache);
    printf("accesos: %d, misses: %d\n", cache->accesos, cache->misses);
    
    for (int i = 0; i < (CSIZE / BSIZE); i++) free(cache->datos[i]);
    free(cache->datos);
    free(cache);

    return 0;
}
