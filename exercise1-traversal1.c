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

void espiral(tCache cache) {
    int topRow = 0;
    int bottomRow = N - 1;
    int leftCol = 0;
    int rightCol = N - 1;
    
    // macro para LibreOffice 
    printf("Sub ColorearCeldasRojo()\n\tDim oDoc As Object\n\tDim oSheet As Object\n\tDim oCell As Object\n\tDim i As Integer\n\n\t' Obtener el documento actual\n\toDoc = ThisComponent\n\n\t' Obtener la hoja de cálculo activa\n\toSheet = oDoc.CurrentController.ActiveSheet\n");

    while (topRow <= bottomRow && leftCol <= rightCol) {
        // fila superior de izquierda a derecha
        for (int col = leftCol; col <= rightCol; col++) {
            if (!cache_access(cache, posToAddress(topRow, col))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", col, topRow);
        }
        topRow++;

        // columna derecha de arriba hacia abajo
        for (int row = topRow; row <= bottomRow; row++) {
            if (!cache_access(cache, posToAddress(row, rightCol))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", rightCol, row);
        }
        rightCol--;

        // fila inferior de derecha a izquierda
        if (topRow <= bottomRow) {
            for (int col = rightCol; col >= leftCol; col--) {
                if(!cache_access(cache, posToAddress(bottomRow, col))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", col, bottomRow);
            }
            bottomRow--;
        }

        // columna izquierda de abajo hacia arriba
        if (leftCol <= rightCol) {
            for (int row = bottomRow; row >= topRow; row--) {
                if(!cache_access(cache, posToAddress(row, leftCol))) printf("\toCell = oSheet.getCellByPosition(%d, %d)\n\toCell.CellBackColor = RGB(255, 0, 0)\n", leftCol, row);
            }
            leftCol++;
        }
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
    
    espiral(cache);
    printf("accesos: %d, misses: %d\n", cache->accesos, cache->misses);
    
    for (int i = 0; i < (CSIZE / BSIZE); i++) free(cache->datos[i]);
    free(cache->datos);
    free(cache);

    return 0;
}
