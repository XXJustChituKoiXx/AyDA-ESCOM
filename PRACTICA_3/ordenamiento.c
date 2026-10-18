#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define INFINITO 999999999

void gen_arr(int*, int);
void print_arr(int*, int);
void bubble_sort(int*, int);
void selection_sort(int*, int);
void insertion_sort(int*, int);
void merge_sort(int*, int, int);
void merge(int*, int, int, int);
void swap(int*, int*);

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
            bubble_sort(datos,n);
            break;
        case 'S':
            selection_sort(datos,n);
            break;
        case 'I':
            insertion_sort(datos,n);
            break;
        case 'M':
            merge_sort(datos, 0, n - 1);
            break;
        default:
            printf("Error: Opcion no valida\n");
            c = '\0';
    }
    clock_t fin = clock();
    double tiempo_usado = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    printf("Ordenamiento finalizado despues de %f segundos\n", tiempo_usado);
    
    if(n < 50 && c != '\0')
        print_arr(datos,n);

    printf("\n");
    free(datos);

    return 0;
}

void swap(int*i, int*inx_min){
        int temp = *i;
        *i = *inx_min;
        *inx_min = temp;
}

void gen_arr(int* arr, int n){
    int min = -10000;
    int max = 10000;

    for (int i = 0; i < n; i++) 
        arr[i] = min + rand() % (max - min + 1);
}

void print_arr(int* arr, int n){
    for(int i = 0;i < n; i++)
        printf("%d  ",*(arr+i));
    
    printf("\n");
}

void bubble_sort(int* arr, int n){
    //f(n) = n(n+1)/2
    //O(n) = O(n^2)
    int flag;
    for(int i = 0; i<n-1 ; ++i){
        flag = 0;
        for(int j = 0; j < n - i - 1; ++j){
            if(*(arr + j) > *(arr + j +1)){
                swap((arr + j) , (arr + j + 1));
                flag = 1;
            }
        }
        if(flag == 0) break;
    }
}

void selection_sort(int *A, int n){
    //f(n) = 5n - 3 + [(n^2 + n -2) + (3n^2 - 3n)]/2
    //O(n) = O(n^2)
    for(int i = 0; i < n - 1; ++i){
        int inx_min = i;
        for(int j = i + 1; j < n ; ++j){
            if(*(A+j) < *(A + inx_min)) 
                inx_min = j;
        }
        if(inx_min != i) swap((A + i),(A + inx_min));        
    }
}

void insertion_sort(int* arr, int n){
    //f(n) = n(n+1) / 2
    //O(n) = O(n^2)
    for(int i=1;i<n;i++){
        int key = arr[i];
        int j=i;
        while(j>0 && key<arr[j-1]){
            arr[j] = arr[j-1];
            j--;
        }
        arr[j] = key;
    }
}

void merge_sort(int *A, int p, int r) {
    //D(n) = θ(1) , C(n) = θ(n), cantidad de subroblemas es  T(n) = 2T(n/2) = nlog(n)  -> nlog(n) + n   
    if (p < r) {
        int q = p + (r - p) / 2; //1
        merge_sort(A, p, q);     
        merge_sort(A, q + 1, r);
        merge(A, p, q, r);   
    }
}
// _ _ _ _ _ _ _ _ _ _ 
// p       q         r
void merge(int *A, int p, int q, int r){
    //f(n) = 5n+2
    //C(n) = θ(n)
    int n1 = q - p + 1;
    int n2 = r - q;
    int *L = malloc((n1 + 1)*sizeof(int));
    int *R = malloc((n2 + 1)*sizeof(int));
    int i = 0;
    int j = 0; 
    for(i = 0; i < n1; ++i){
        *(L+i) = *(A + p + i);
    }
    for(j = 0; j < n2; ++j){
        *(R+j) = *(A + q + 1 + j);
    }
    *(L+i) = INFINITO;
    *(R+j) = INFINITO;
    i = 0;
    j = 0;
    
    for(int k = p; k <= r; ++k){
        if(*(L + i) <= *(R + j)){
            *(A + k) = *(L + i);
            i++;
        }else{
            *(A + k) = *(R + j);
            j++;
        }
    }
    free(L);
    free(R);
}