#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
    Tamano a partir del cual se deja de dividir con strassen y se
    utiliza el algoritmo tradicional
 */
#define UMBRAL 32

/*
    Estructura de submatriz por calculo de indices:
    m = matriz completa
    f = fila donde empieza la submatriz
    c = columna donde empieza la submatriz
*/
typedef struct {
    long long **m;
    int f;
    int c;
} Sub;

/* Acceso al elemento (i, j) de una submatriz */
#define EL(X, i, j) ((X).m[(X).f + (i)][(X).c + (j)])

int es_valid(int);
long long** crear_matriz(int);
void liberar_matriz(long long**);
void imprimir_matriz(long long**, int);
void llenar_matriz(long long**, int);
void mult_trad(long long**, long long**, long long**, int);

Sub entera(long long**);
Sub cuadrante(Sub, int, int, int);
void suma_sub_mat(Sub, Sub, Sub, int);
void resta_sub_mat(Sub, Sub, Sub, int);
int strassen(Sub, Sub, Sub, int);


int main(){
    srand(time(NULL));
    clock_t inicio, fin;
    double tiempo;
    int n;

    printf("Ingrese el valor de n: ");
    if(scanf("%d", &n) != 1){
        printf("Error: Entrada invalida\n");
        return 1;
    }

    if(!es_valid(n))
        return 1;

    long long **mat_A = crear_matriz(n);
    long long **mat_B = crear_matriz(n);
    long long **mat_C = crear_matriz(n);   /* resultado de Strassen */
    long long **mat_D = crear_matriz(n);   /* resultado tradicional */

    if(mat_A == NULL || mat_B == NULL || mat_C == NULL || mat_D == NULL){
        printf("Error al asignar memoria\n");
        liberar_matriz(mat_A); liberar_matriz(mat_B);
        liberar_matriz(mat_C); liberar_matriz(mat_D);
        return 1;
    }

    llenar_matriz(mat_A, n);
    llenar_matriz(mat_B, n);

    printf("Matrices generadas exitosamente\n");
    if(n <= 32){
        printf("\n===================================================================");
        printf("\nMatriz A:\n");
        imprimir_matriz(mat_A, n);
        printf("\nMatriz B:\n");
        imprimir_matriz(mat_B, n);
        printf("===================================================================\n");
    }

    /* ---------- Strassen ---------- */
    inicio = clock();
    int resultado = strassen(entera(mat_A), entera(mat_B), entera(mat_C), n);
    fin = clock();
    tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    if(resultado != 0){
        printf("Error: Memoria insuficiente\n");
        liberar_matriz(mat_A); liberar_matriz(mat_B);
        liberar_matriz(mat_C); liberar_matriz(mat_D);
        return 1;
    }
    if(n <= 32){
        printf("\nResultado algoritmo de Strassen:\n");
        imprimir_matriz(mat_C, n);
    }
    printf("Ejecucion exitosa de Strassen con matriz %dx%d despues de %lf s\n", n, n, tiempo);

    /* ---------- Tradicional ---------- */
    inicio = clock();
    mult_trad(mat_A, mat_B, mat_D, n);
    fin = clock();
    tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    if(n <= 32){
        printf("\nResultado algoritmo tradicional:\n");
        imprimir_matriz(mat_D, n);
    }
    printf("Ejecucion exitosa de la multiplicacion tradicional con matriz %dx%d despues de %lf s\n", n, n, tiempo);

    liberar_matriz(mat_A);
    liberar_matriz(mat_B);
    liberar_matriz(mat_C);
    liberar_matriz(mat_D);

    return 0;
}


int es_valid(int n){
    if(n < 2){
        printf("Error: Dimension menor a 2\n");
        return 0;
    }
    if(n & (n - 1)){
        printf("Error: La dimension de la matriz no es potencia de 2\n");
        return 0;
    }
    return 1;
}


/* Reserva la matriz en un solo bloque contiguo:
   mat[0] apunta a todos los datos y mat[i] apunta al inicio de la fila i. */
long long** crear_matriz(int n){
    long long **mat = (long long**)malloc(n * sizeof(long long*));
    if(mat == NULL)
        return NULL;

    mat[0] = (long long*)calloc((size_t)n * n, sizeof(long long));
    if(mat[0] == NULL){
        free(mat);
        return NULL;
    }

    for(int i = 1; i < n; i++)
        mat[i] = mat[0] + (size_t)i * n;

    return mat;
}


void liberar_matriz(long long **mat){
    if(mat == NULL)
        return;
    free(mat[0]);
    free(mat);
}


void imprimir_matriz(long long **mat, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++)
            printf("%3lld ", mat[i][j]);
        printf("\n");
    }
}


void llenar_matriz(long long **mat, int n){
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            mat[i][j] = rand() % 3;
}


/* Metodo tradicional */
void mult_trad(long long **A, long long **B, long long **C, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            C[i][j] = 0;
            for(int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    }
}


/* Una matriz completa vista como submatriz que empieza en (0, 0) */
Sub entera(long long **m){
    Sub X = {m, 0, 0};
    return X;
}


/* 
    Cuadrante (i, j) de X, con i, j en {0, 1}:
    (0,0) = X11   (0,1) = X12
    (1,0) = X21   (1,1) = X22
 */
Sub cuadrante(Sub X, int i, int j, int mitad){
    Sub Y = {X.m, X.f + i * mitad, X.c + j * mitad};
    return Y;
}


/* Z = X + Y  (las tres de n x n) */
void suma_sub_mat(Sub X, Sub Y, Sub Z, int n){
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            EL(Z, i, j) = EL(X, i, j) + EL(Y, i, j);
}


/* Z = X - Y  (las tres de n x n) */
void resta_sub_mat(Sub X, Sub Y, Sub Z, int n){
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            EL(Z, i, j) = EL(X, i, j) - EL(Y, i, j);
}


/* Regresa 0 si todo salio bien, 1 si hubo error de memoria. */
int strassen(Sub A, Sub B, Sub C, int n){

    /* Caso base: con UMBRAL = 1 es solo c11 = a11 * b11 */
    if(n <= UMBRAL){
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++){
                long long s = 0;
                for(int k = 0; k < n; k++)
                    s += EL(A, i, k) * EL(B, k, j);
                EL(C, i, j) = s;
            }
        return 0;
    }

    int m = n / 2;

    /* ===== Paso 1: dividir A, B y ===== */
    Sub A11 = cuadrante(A, 0, 0, m), A12 = cuadrante(A, 0, 1, m);
    Sub A21 = cuadrante(A, 1, 0, m), A22 = cuadrante(A, 1, 1, m);

    Sub B11 = cuadrante(B, 0, 0, m), B12 = cuadrante(B, 0, 1, m);
    Sub B21 = cuadrante(B, 1, 0, m), B22 = cuadrante(B, 1, 1, m);

    Sub C11 = cuadrante(C, 0, 0, m), C12 = cuadrante(C, 0, 1, m);
    Sub C21 = cuadrante(C, 1, 0, m), C22 = cuadrante(C, 1, 1, m);


    long long **S[11] = {NULL};
    long long **P[8]  = {NULL};
    int err = 0;

    for(int k = 1; k <= 10; k++)
        if((S[k] = crear_matriz(m)) == NULL) err = 1;
    for(int k = 1; k <= 7; k++)
        if((P[k] = crear_matriz(m)) == NULL) err = 1;

    if(!err){
        /* ===== Paso 2: Calcular S1..S10 ===== */
        resta_sub_mat(B12, B22, entera(S[1]),  m);   /* S1  = B12 - B22 */
        suma_sub_mat (A11, A12, entera(S[2]),  m);   /* S2  = A11 + A12 */
        suma_sub_mat (A21, A22, entera(S[3]),  m);   /* S3  = A21 + A22 */
        resta_sub_mat(B21, B11, entera(S[4]),  m);   /* S4  = B21 - B11 */
        suma_sub_mat (A11, A22, entera(S[5]),  m);   /* S5  = A11 + A22 */
        suma_sub_mat (B11, B22, entera(S[6]),  m);   /* S6  = B11 + B22 */
        resta_sub_mat(A12, A22, entera(S[7]),  m);   /* S7  = A12 - A22 */
        suma_sub_mat (B21, B22, entera(S[8]),  m);   /* S8  = B21 + B22 */
        resta_sub_mat(A11, A21, entera(S[9]),  m);   /* S9  = A11 - A21 */
        suma_sub_mat (B11, B12, entera(S[10]), m);   /* S10 = B11 + B12 */

        /* ===== Paso 3: 7 productos recursivos ===== */
                  err = strassen(A11,          entera(S[1]),  entera(P[1]), m); /* P1 = A11 * S1  */
        if(!err)  err = strassen(entera(S[2]), B22,           entera(P[2]), m); /* P2 = S2 * B22  */
        if(!err)  err = strassen(entera(S[3]), B11,           entera(P[3]), m); /* P3 = S3 * B11  */
        if(!err)  err = strassen(A22,          entera(S[4]),  entera(P[4]), m); /* P4 = A22 * S4  */
        if(!err)  err = strassen(entera(S[5]), entera(S[6]),  entera(P[5]), m); /* P5 = S5 * S6   */
        if(!err)  err = strassen(entera(S[7]), entera(S[8]),  entera(P[6]), m); /* P6 = S7 * S8   */
        if(!err)  err = strassen(entera(S[9]), entera(S[10]), entera(P[7]), m); /* P7 = S9 * S10  */
    }

    if(!err){
        /* ===== Paso 4: armar C a partir de los Pi ===== */
        for(int i = 0; i < m; i++){
            for(int j = 0; j < m; j++){
                EL(C11, i, j) = P[5][i][j] + P[4][i][j] - P[2][i][j] + P[6][i][j];
                EL(C12, i, j) = P[1][i][j] + P[2][i][j];
                EL(C21, i, j) = P[3][i][j] + P[4][i][j];
                EL(C22, i, j) = P[5][i][j] + P[1][i][j] - P[3][i][j] - P[7][i][j];
            }
        }
    }

    /* Liberar auxiliares */
    for(int k = 1; k <= 10; k++) liberar_matriz(S[k]);
    for(int k = 1; k <= 7; k++)  liberar_matriz(P[k]);

    return err;
}