#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gen_arr(int*,int);
void print_arr(int*,int);
void bubble(int*,int);
void selection(int*,int);
void insertion(int*,int);
void merge(int*,int);


int main(){
    srand(time(NULL));
    int n;
    char c;

    printf("Ingrese la longitud del arreglo: ");
    scanf("%d",&n);

    int* datos = (int*)malloc(n * sizeof(int));
    if (datos == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    gen_arr(datos,n);
    printf("Arreglo generado\n");

    if(n < 50)
        print_arr(datos,n);

    printf("Ingrese el tipo de ordenamiento (B/S/I/M): ");
    scanf(" %c",&c);
    clock_t inicio = clock();
    switch(c){
        case 'B':
            bubble(datos,n);
            break;
        case 'S':
            selection(datos,n);
            break;
        case 'I':
            insertion(datos,n);
            break;
        case 'M':
            merge(datos,n);
            break;
        default:
            printf("Error: Opcion no valida\n");
    }
    clock_t fin = clock();
    double tiempo_usado = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    printf("Ordenamiento finalizado despues de %f segundos\n", tiempo_usado);
    
    if(n < 50)
        print_arr(datos,n);

    printf("\n");
    free(datos);

    return 0;
}

void gen_arr(int* arr, int n){

    int min = -10000;
    int max = 10000;


    for (int i = 0; i < n; i++) 
        arr[i] = min + rand() % (max - min + 1);

    return;
}

void print_arr(int* arr,int n){
    for(int i = 0;i < n; i++)
        printf("%d  ",*(arr+i));
    
    printf("\n");

    return;
}

void bubble(int* arr, int n){
    int fin = n - 1;

    while(fin > 0){
        int nuevo_fin = 0;
        for(int i=0;i<fin;i++){
            if(arr[i] > arr[i+1]){
                int temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                nuevo_fin = i;
            }
        }
        fin = nuevo_fin;
    }
    return;
}

void selection(int* arr, int n){

    return;
}

void insertion(int* arr, int n){
    for(int i=1;i<n;i++){
        int key = arr[i];
        int j=i;
        while(j>0 && key<arr[j-1]){
            arr[j] = arr[j-1];
            j--;
        }
        arr[j] = key;
    }

    return;
}

void merge(int* arr, int n){

    return;
}