#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO_BITS 128 

void string_to_bin(int* A, const char* numero_texto);
void bin_to_string(int* R, int n, char* texto_decimal, int max_digitos);
void multiplicar_enteros_grandes(int* num1, int* num2, int* R, int n);
void sumar_bloques_bits(int* A, int* B, int* R, int n);
void copiar_bloque(int* destino, int* origen, int inicio, int n);

void string_to_bin(int* A, const char* numero_texto) {
    for(int i = 0; i < TAMANO_BITS; i++) *(A + i) = 0;
    int len = strlen(numero_texto);
    char *temp = malloc(len + 1); 
    if (temp == NULL) return; // Protección contra malloc nulo
    strcpy(temp, numero_texto);
    int j = TAMANO_BITS - 1;

    while (1) {
        int todos_ceros = 1; 
        int residuo = 0;
        for (int i = 0; i < len; i++) {
            int digito_actual = temp[i] - '0'; 
            if (digito_actual != 0) todos_ceros = 0;
            int valor_actual = residuo * 10 + digito_actual;
            temp[i] = (valor_actual / 2) + '0';
            residuo = valor_actual % 2;
        }
        if (todos_ceros) break; 
        if (j >= 0) {
            *(A + j) = residuo;
            j--;
        }
    }
    free(temp);
}

void bin_to_string(int* R, int n, char* texto_decimal, int max_digitos) {
    for(int i = 0; i < max_digitos - 1; i++) texto_decimal[i] = '0';
    texto_decimal[max_digitos - 1] = '\0';

    for (int i = 0; i < n; i++) {
        int acarreo = R[i]; 
        for (int j = max_digitos - 2; j >= 0; j--) {
            int valor_actual = (texto_decimal[j] - '0') * 2 + acarreo;
            texto_decimal[j] = (valor_actual % 10) + '0'; 
            acarreo = valor_actual / 10;                         
        }
    }
    int inicio_real = 0;
    while (texto_decimal[inicio_real] == '0' && texto_decimal[inicio_real + 1] != '\0') {
        inicio_real++;
    }
    memmove(texto_decimal, texto_decimal + inicio_real, strlen(texto_decimal + inicio_real) + 1);
}

void sumar_bloques_bits(int* A, int* B, int* R, int n) {
    int acarreo = 0;
    for (int i = n - 1; i >= 0; i--) {
        int suma = A[i] + B[i] + acarreo;
        R[i] = suma & 1;       
        acarreo = (suma >> 1); 
    }
}

void copiar_bloque(int* destino, int* origen, int inicio, int n) {
    for (int i = 0; i < n; i++) {
        *(destino + i) = *(origen + inicio + i);
    }
}

void multiplicar_enteros_grandes(int *num1, int *num2, int *R, int n){
    for(int i = 0; i < 2 * n; i++) R[i] = 0;

    if (n == 1) {
        R[1] = num1[0] & num2[0];
        return;
    }

    int mitad = n >> 1; 

    int *xi = malloc(mitad * sizeof(int));
    int *xd = malloc(mitad * sizeof(int));
    int *yi = malloc(mitad * sizeof(int));
    int *yd = malloc(mitad * sizeof(int));
    int *P1 = malloc(n * sizeof(int));
    int *P2 = malloc(n * sizeof(int));
    int *P3 = malloc(n * sizeof(int));
    int *sumaX = malloc(mitad * sizeof(int));
    int *sumaY = malloc(mitad * sizeof(int));

    // Validación de seguridad para prevenir fallos por memoria agotada
    if (!xi || !xd || !yi || !yd || !P1 || !P2 || !P3 || !sumaX || !sumaY) {
        free(xi); free(xd); free(yi); free(yd);
        free(P1); free(P2); free(P3);
        free(sumaX); free(sumaY);
        return;
    }

    copiar_bloque(xi, num1, 0, mitad);     
    copiar_bloque(xd, num1, mitad, mitad); 
    copiar_bloque(yi, num2, 0, mitad);     
    copiar_bloque(yd, num2, mitad, mitad); 

    multiplicar_enteros_grandes(xi, yi, P1, mitad);
    multiplicar_enteros_grandes(xd, yd, P2, mitad);

    sumar_bloques_bits(xi, xd, sumaX, mitad);
    sumar_bloques_bits(yi, yd, sumaY, mitad);

    multiplicar_enteros_grandes(sumaX, sumaY, P3, mitad);

    for (int i = 0; i < n; i++) R[i + n] += P2[i];
    for (int i = 0; i < n; i++) R[i] += P1[i];
    for (int i = 0; i < n; i++) {
        int termino_medio = P3[i] - P2[i] - P1[i];
        R[i + mitad] += termino_medio;
    }

    for (int i = (2 * n) - 1; i > 0; i--) {
        if (R[i] > 1) {
            R[i - 1] += (R[i] >> 1); 
            R[i] = R[i] & 1;
        } else if (R[i] < 0) {
            R[i - 1] -= 1;
            R[i] += 2;
        }
    }

    free(xi); free(xd); free(yi); free(yd);
    free(P1); free(P2); free(P3);
    free(sumaX); free(sumaY);
}

int main(){
    char* num1_texto = "999999999999999999999999";
    char* num2_texto = "999999999999999999999999";
    
    int max_dig_dec = 90; 
    char* resultado_dec = malloc(max_dig_dec * sizeof(char));
    int *A1, *A2, *resultado;

    A1 = (int*)malloc(TAMANO_BITS * sizeof(int));
    A2 = (int*)malloc(TAMANO_BITS * sizeof(int));
    resultado = (int*)malloc(2 * TAMANO_BITS * sizeof(int));

    if (A1 == NULL || A2 == NULL || resultado == NULL || resultado_dec == NULL) {
        printf("No se pudo asignar memoria.\n");
        free(A1); free(A2); free(resultado); free(resultado_dec);
        return 1;
    }

    string_to_bin(A1, num1_texto);
    string_to_bin(A2, num2_texto);
 
    multiplicar_enteros_grandes(A1, A2, resultado, TAMANO_BITS);
    
    bin_to_string(resultado, 2 * TAMANO_BITS, resultado_dec, max_dig_dec);
    printf("Resultado: %s\n", resultado_dec);

    free(A1);
    free(A2); 
    free(resultado);
    free(resultado_dec);
    return 0;
}