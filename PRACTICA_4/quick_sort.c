#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

char *pivote_mode = 'r';

void quicksort(int*,int,int);
int partition(int*,int,int);

void quicksort(int*A,int p,int r){
    if(p<r){
        int q = partition(A,p,r);
        quicksort(A,p,q-1);
        quicksort(A,q,r);
    }
}
int partition(int* A,int p,int r){
    
}

void rellenarArrayRandom(int* A, int n){
    for(int i = 0; i < n;i++){
        *(A + i) = -100000 + rand() % (100000 - (-100000) + 1);
    }
}
void rellenarArrayOrdenado(int* A, int n){
    double min_val = -100000;
    double max_val = 100000;
    if(n == 1){
        *A = min_val;
        return;
    }
    double paso;
    paso = (max_val - min_val) / (n - 1);
    for(int i = 0; i < n;i++){ 
        *(A + i) = (double)(min_val + i*paso);
    }
}

void print_array(char* A, int tam){
    int i = 0;
    while(i < tam && i < 100){
        printf("%d", *(A + i));
        i++;
    }
}
void main(){
    int* array;
    double tam,tiempo_segundos;
    char* array_mode;
    clock_t inicio,fin;

    //crear array de numeros
    tam = scanf("\n Ingrese tamaño del arreglo: %d",&tam);
    array = malloc(sizeof(int)*tam);
    
    //establecer donde se elegira el pivote, al inicio o final.
    do{
        printf("\n Ingrese posicion del Pivote: \n Random: r \n Ultimo: f");
        pivote_mode = fgetc(stdin);
        tolower(pivote_mode);
        if(pivote_mode != 'r' && pivote_mode != 'f') printf("\n Solo puede seleccionar 'r' o 'f'.");
    }while(pivote_mode != 'r' && pivote_mode != 'f');
    
    //definir si el arreglo estara ordenado o desordenado
    do{
        printf("\n Ingrese Estado del Arreglo: \n Ordenado: o \n Desordenado: r");
        array_mode = fgetc(stdin);
        tolower(array_mode);
        if(array_mode != 'o' && array_mode != 'r') printf("\n Solo puede seleccionar 'r' u 'o'.");
    }while(array_mode != 'o' && array_mode != 'r');
    
    //rellenar arreglo
    if(array_mode == 'o'){
        inicio = clock();
        rellenarArrayOrdenado(array,tam);
        fin = clock();
        tiempo_segundos = (double) (fin - inicio) / CLOCKS_PER_SEC;
        printf("\n Tiempo en llenar arreglo ordenado: %d",tiempo_segundos);
    }else if (array_mode == 'r'){
        inicio = clock();
        rellenarArrayRandom(array,tam);
        fin = clock();
        tiempo_segundos = (double) (fin - inicio) / CLOCKS_PER_SEC;
        printf("\n Tiempo en llenar arreglo random: %d",tiempo_segundos);
    }
    printf("\n Arreglo desordenado(solo hasta 100 elementos):");
    print_array(array,tam);
    
    //ordenamiento
    inicio = clock();
    quicksort(array,0,tam-1);
    fin = clock();
    tiempo_segundos = (double) (fin - inicio) / CLOCKS_PER_SEC;
    
    printf("\n Arreglo ordenado(solo hasta 100 elementos):");
    print_array(array,tam);

    if(pivote_mode == "f" && array_mode == "o") 
        printf("Tiempo de ordenamiento para un arreglo Ordenado y el pivote al en la ultima posicion %d", tiempo_segundos);
    
    if(pivote_mode == "r" && array_mode == "o") 
        printf("Tiempo de ordenamiento para un arreglo Ordenado y el pivote en una posicion random: %d", tiempo_segundos);
    
    if(pivote_mode == "f" && array_mode == "r") 
        printf("Tiempo de ordenamiento para un arreglo desordenado y el pivote al en la ultima posicion %d", tiempo_segundos);
    
    if(pivote_mode == "r" && array_mode == "r") 
        printf("Tiempo de ordenamiento para un arreglo desordenado y el pivote en una posicion random: %d", tiempo_segundos);
    free(array);
}