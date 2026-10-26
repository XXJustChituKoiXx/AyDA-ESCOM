#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAP 512

// Suma dos numeros binarios
int* sumar(int *x, int *y, int n) {
    int *R = calloc(CAP, sizeof(int));
    int acarreo = 0;

    for (int i = 0; i < n; i++) {
        int suma = *(x+i) + *(y+i) + acarreo;
        *(R+i) = suma % 2;
        acarreo = suma / 2;
    }

    *(R+n) = acarreo;
    return R;
}

// Resta dos numeros binarios
int* restar(int *x, int *y) {
    int *R = calloc(CAP, sizeof(int));
    int prestamo = 0;

    for (int i = 0; i < CAP; i++) {
        int resta = *(x+i) - *(y+i) - prestamo;

        if (resta < 0) {
            resta += 2;
            prestamo = 1;
        } else {
            prestamo = 0;
        }

        *(R+i) = resta;
    }

    return R;
}

// Combina los resultados parciales usando la formula:
// xy = z2 * 2^(2*m) + z1 * 2^m + z0.
// Desplaza z2 2*m posiciones, z1 m posiciones y suma z0 sin desplazar.
void sumar_desplazado(int *R, int *x, int desplazamiento) {
    int acarreo = 0;

    for (int i = 0; i + desplazamiento < CAP; i++) {
        int posicion = i + desplazamiento;
        int suma = *(R+posicion) + *(x+i) + acarreo;

        *(R+posicion) = suma % 2;
        acarreo = suma / 2;
    }
}

int* multiplicar_base(int *x, int *y, int n) {
    int *R = calloc(CAP, sizeof(int));

    for (int i = 0; i < n; i++) {
        if (*(x+i)) {
            for (int j = 0; j < n; j++) {
                if (*(y+j)) {
                    int posicion = i+j;
                    int acarreo = 1;

                    while (acarreo && posicion < CAP) {
                        int suma = *(R+posicion) + acarreo;
                        *(R+posicion) = suma % 2;
                        acarreo = suma / 2;
                        posicion++;
                    }
                }
            }
        }
    }

    return R;
}

int* karatsuba(int *x, int *y, int n) {
    if (n <= 3)
        return multiplicar_base(x, y, n);

    int m = n / 2;
    int alto = n - m;

    int *xBajo = calloc(CAP, sizeof(int));
    int *xAlto = calloc(CAP, sizeof(int));
    int *yBajo = calloc(CAP, sizeof(int));
    int *yAlto = calloc(CAP, sizeof(int));

    // Divide los numeros en mitades
    for (int i = 0; i < m; i++) {
        *(xBajo+i) = *(x+i);
        *(yBajo+i) = *(y+i);
    }

    for (int i = 0; i < alto; i++) {
        *(xAlto+i) = *(x+m+i);
        *(yAlto+i) = *(y+m+i);
    }

    int *z0 = karatsuba(xBajo, yBajo, m);
    int *z2 = karatsuba(xAlto, yAlto, alto);

    int *sumaX = sumar(xAlto, xBajo, alto);
    int *sumaY = sumar(yAlto, yBajo, alto);

    int *z1 = karatsuba(sumaX, sumaY, alto+1);

    // Obtiene el producto cruzado
    int *aux = restar(z1, z0);
    int *cruzado = restar(aux, z2);

    int *R = calloc(CAP, sizeof(int));

    // Combina los resultados mediante desplazamientos
    sumar_desplazado(R, z2, 2*m);
    sumar_desplazado(R, cruzado, m);
    sumar_desplazado(R, z0, 0);

    free(xBajo);
    free(xAlto);
    free(yBajo);
    free(yAlto);
    free(z0);
    free(z1);
    free(z2);
    free(sumaX);
    free(sumaY);
    free(aux);
    free(cruzado);

    return R;
}

// Convierte un numero decimal a binario
int decimal_a_binario(char *decimal, int *bits) {
    int longitud = strlen(decimal);
    int inicio = 0;
    int cantidad = 0;

    while (inicio < longitud && *(decimal+inicio) == '0')
        inicio++;

    if (inicio == longitud) {
        *(bits+0) = 0;
        return 1;
    }

    while (inicio < longitud) {
        int acarreo = 0;

        for (int i = inicio; i < longitud; i++) {
            int numero = acarreo * 10 + (*(decimal+i) - '0');

            *(decimal+i) = '0' + numero / 2;
            acarreo = numero % 2;
        }

        *(bits+cantidad) = acarreo;
        cantidad++;

        while (inicio < longitud && *(decimal+inicio) == '0')
            inicio++;
    }

    return cantidad;
}

// Convierte un numero binario a decimal
char* binario_a_decimal(int *bits, int n) {
    int *digitos = calloc(CAP, sizeof(int));
    char *resultado = calloc(CAP, sizeof(char));
    int cantidad = 1;

    for (int i = n-1; i >= 0; i--) {
        int acarreo = *(bits+i);

        for (int j = 0; j < cantidad; j++) {
            int valor = *(digitos+j) * 2 + acarreo;

            *(digitos+j) = valor % 10;
            acarreo = valor / 10;
        }

        while (acarreo > 0) {
            *(digitos+cantidad) = acarreo % 10;
            cantidad++;
            acarreo /= 10;
        }
    }

    for (int i = 0; i < cantidad; i++)
        *(resultado+i) = '0' + *(digitos+cantidad-1-i);

    *(resultado+cantidad) = '\0';

    free(digitos);
    return resultado;
}

int main() {
    char *xDecimal = calloc(1000, sizeof(char));
    char *yDecimal = calloc(1000, sizeof(char));

    int *x = calloc(CAP, sizeof(int));
    int *y = calloc(CAP, sizeof(int));

    // Lee los dos numeros decimales
    printf("Primer numero: ");
    if (fgets(xDecimal, 1000, stdin) == NULL)
        return 1;

    printf("Segundo numero: ");
    if (fgets(yDecimal, 1000, stdin) == NULL)
        return 1;

    // Elimina los saltos de linea
    char *saltoX = strchr(xDecimal, '\n');
    if (saltoX != NULL)
        *saltoX = '\0';

    char *saltoY = strchr(yDecimal, '\n');
    if (saltoY != NULL)
        *saltoY = '\0';


    // Verifica que las entradas sean numericas
    for (int i = 0; *(xDecimal+i) != '\0'; i++) {
        if (*(xDecimal+i) < '0' || *(xDecimal+i) > '9') {
            printf("Entrada invalida.\n");
            return 1;
        }
    }

    for (int i = 0; *(yDecimal+i) != '\0'; i++) {
        if (*(yDecimal+i) < '0' || *(yDecimal+i) > '9') {
            printf("Entrada invalida.\n");
            return 1;
        }
    }

    // Convierte las entradas a binario
    int nx = decimal_a_binario(xDecimal, x);
    int ny = decimal_a_binario(yDecimal, y);

    // Ajusta la longitud a una potencia de dos
    int n = nx > ny ? nx : ny;
    int potencia = 1;

    while (potencia < n)
        potencia *= 2;

    n = potencia;

    // Multiplica usando Karatsuba
    int *R = karatsuba(x, y, n);

    // Convierte el resultado a decimal
    char *resultado = binario_a_decimal(R, CAP);

    printf("Resultado: %s\n", resultado);

    free(xDecimal);
    free(yDecimal);
    free(x);
    free(y);
    free(R);
    free(resultado);

    return 0;
}
