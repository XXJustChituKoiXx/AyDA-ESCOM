#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

char pivote_mode = 'r';
void swap(int* a, int* b);
int partition(int* A, int p, int r);
void quicksort(int* A, int p, int r);

void quicksort(int* A, int p, int r){
    while(p < r){
        int q = partition(A, p, r);
        if (q - p < r - q) {
            quicksort(A, p, q - 1);
            p = q + 1;             
        } else {
            quicksort(A, q + 1, r);
            r = q - 1;
        }
    }
}

int partition(int* A,int p,int r){
    int x,i;
    if(pivote_mode == 'r'){
        x =  p + rand() % (r - p + 1);
        swap(A + r, A + x);
    }
    x = r;
    i = p - 1;
    for(int j = p; j < r; ++j){
        if(*(A + j) < *(A + x)){
            i++;
            swap(A + i, A + j);
        }
    }
    swap(A + i + 1, A + x);
    return i + 1;
}

void rellenarArrayRandom(int* A, int n){
    for(int i = 0; i < n;i++){
        *(A + i) = -100000 + rand() % (200001);
    }
}
void rellenarArrayOrdenado(int* A, int n){
    /*
    double min_val = -100000;
    double max_val = 100000;
    if(n == 1){
        *A = (int) min_val;
        return;
    }
    double paso;
    paso = (max_val - min_val) / (n - 1);
    for(int i = 0; i < n;i++){ 
        *(A + i) = (int)(min_val + i*paso);
    }
    */
    for(int i = 0; i < n;i++){ 
        *(A + i) = i;
    }
}

void print_array(int* A, int tam){
    int i = 0;
    while(i < tam && i < 100){
        printf("%d ", *(A + i));
        i++;
    }
}

void swap(int* a, int*b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


void main(){
    int* array;
    int tam;
    double tiempo_segundos;
    char array_mode;
    clock_t inicio,fin;
    srand(time(NULL));
    //crear array de numeros
    printf("\n Ingrese la longitud del arreglo: ");
    scanf("%d",&tam);
    while (getchar() != '\n');
    array = malloc(sizeof(int)*tam);
    if(array == NULL){
        printf("\n Error al asignar memoria para el arreglo.\n");
        return 1;
    }
    
    //establecer donde se elegira el pivote, al inicio o final.
    do{
        printf("\n Ingrese posicion del Pivote: \n Random: r \n Ultimo: f \n");
        pivote_mode = fgetc(stdin);
        pivote_mode = tolower(pivote_mode);
        while (getchar() != '\n');
        if(pivote_mode != 'r' && pivote_mode != 'f') printf("\n Solo puede seleccionar 'r' o 'f'.\n");
    }while(pivote_mode != 'r' && pivote_mode != 'f');
    
    //definir si el arreglo estara ordenado o desordenado
    do{
        printf("\n Ingrese Estado del Arreglo: \n Ordenado: o \n Desordenado: r\n");
        array_mode = fgetc(stdin);
        array_mode = tolower(array_mode);
        while (getchar() != '\n');
        if(array_mode != 'o' && array_mode != 'r') printf("\n Solo puede seleccionar 'r' u 'o'.\n");
    }while(array_mode != 'o' && array_mode != 'r');
    
    //rellenar arreglo
    if(array_mode == 'o'){
        inicio = clock();
        rellenarArrayOrdenado(array,tam);
        fin = clock();
        tiempo_segundos = (double) (fin - inicio) / CLOCKS_PER_SEC;
        printf("\n Tiempo en llenar arreglo ordenado: %f s.",tiempo_segundos);
    }else if (array_mode == 'r'){
        inicio = clock();
        rellenarArrayRandom(array,tam);
        fin = clock();
        tiempo_segundos = (double) (fin - inicio) / CLOCKS_PER_SEC;
        printf("\n Tiempo en llenar arreglo random: %f s.",tiempo_segundos);
    }
    printf("\n Arreglo generado antes de ordenadar(Primeros 100 elementos): \n");
    print_array(array,tam);
    
    //ordenamiento
    inicio = clock();
    quicksort(array,0,tam-1);
    fin = clock();
    tiempo_segundos = (double) (fin - inicio) / CLOCKS_PER_SEC;
    
    printf("\n Arreglo despues de ordenar(Primeros 100 elementos): \n");
    print_array(array,tam);

    if(pivote_mode == 'f' && array_mode == 'o') 
        printf("\nTiempo de ordenamiento para un arreglo ordenado de %d elementos y el pivote al en la ultima posicion: %f s.", tam, tiempo_segundos);
    
    if(pivote_mode == 'r' && array_mode == 'o') 
        printf("\nTiempo de ordenamiento para un arreglo ordenado de %d elementos y el pivote en una posicion aleatoria: %f s.", tam, tiempo_segundos);
    
    if(pivote_mode == 'f' && array_mode == 'r') 
        printf("\nTiempo de ordenamiento para un arreglo desordenado de %d elementos y el pivote al en la ultima posicion: %f s.", tam, tiempo_segundos);
    
    if(pivote_mode == 'r' && array_mode == 'r') 
        printf("\nTiempo de ordenamiento para un arreglo desordenado de %d elementos y el pivote en una posicion aleatoria: %f s.", tam, tiempo_segundos);

    printf("\n");
    free(array);
}