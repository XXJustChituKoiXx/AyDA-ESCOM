#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITOS 10000

// Suma dos numeros binarios de n bits. El resultado tiene n+1 bits
int* sumar(int *x, int *y, int n) {
    int *R = calloc(n+1, sizeof(int));
    int acarreo = 0;

    for (int i = 0; i < n; i++) {
        int suma = *(x+i) + *(y+i) + acarreo;
        *(R+i) = suma % 2;
        acarreo = suma / 2;
    }

    *(R+n) = acarreo;
    return R;
}

// Resta dos numeros binarios (x >= y). x tiene nx bits, y tiene ny bits (ny <= nx).
// El resultado tiene nx bits
int* restar(int *x, int nx, int *y, int ny) {
    int *R = calloc(nx, sizeof(int));
    int prestamo = 0;

    for (int i = 0; i < nx; i++) {
        int bitY = i < ny ? *(y+i) : 0;
        int resta = *(x+i) - bitY - prestamo;

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
// Suma x (de nx bits) a R (de nR bits) desplazado 'desplazamiento' posiciones.
void sumar_desplazado(int *R, int nR, int *x, int nx, int desplazamiento) {
    int acarreo = 0;
    int posicion = desplazamiento;

    for (int i = 0; i < nx && posicion < nR; i++, posicion++) {
        int suma = *(R+posicion) + *(x+i) + acarreo;

        *(R+posicion) = suma % 2;
        acarreo = suma / 2;
    }

    // Propaga el acarreo restante
    while (acarreo && posicion < nR) {
        int suma = *(R+posicion) + acarreo;

        *(R+posicion) = suma % 2;
        acarreo = suma / 2;
        posicion++;
    }
}

// Multiplica dos numeros de n bits. El resultado tiene 2*n bits
int* multiplicar_base(int *x, int *y, int n) {
    int *R = calloc(2*n, sizeof(int));

    for (int i = 0; i < n; i++) {
        if (*(x+i)) {
            for (int j = 0; j < n; j++) {
                if (*(y+j)) {
                    int posicion = i+j;
                    int acarreo = 1;

                    while (acarreo && posicion < 2*n) {
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

// Multiplica dos numeros de n bits. El resultado tiene 2*n bits
int* karatsuba(int *x, int *y, int n) {
    if (n <= 3)
        return multiplicar_base(x, y, n);

    int m = n / 2;
    int alto = n - m;

    // Divide los numeros en mitades. La parte baja se rellena con ceros
    // hasta 'alto' bits para poder sumarla con la parte alta
    int *xBajo = calloc(alto, sizeof(int));
    int *yBajo = calloc(alto, sizeof(int));
    int *xAlto = x + m;
    int *yAlto = y + m;

    for (int i = 0; i < m; i++) {
        *(xBajo+i) = *(x+i);
        *(yBajo+i) = *(y+i);
    }

    int *z0 = karatsuba(xBajo, yBajo, m);       // 2*m bits
    int *z2 = karatsuba(xAlto, yAlto, alto);    // 2*alto bits

    int *sumaX = sumar(xAlto, xBajo, alto);     // alto+1 bits
    int *sumaY = sumar(yAlto, yBajo, alto);     // alto+1 bits

    int *z1 = karatsuba(sumaX, sumaY, alto+1);  // 2*(alto+1) bits

    // Obtiene el producto cruzado
    int *aux = restar(z1, 2*(alto+1), z0, 2*m);
    int *cruzado = restar(aux, 2*(alto+1), z2, 2*alto);

    int *R = calloc(2*n, sizeof(int));

    // Combina los resultados mediante desplazamientos
    sumar_desplazado(R, 2*n, z2, 2*alto, 2*m);
    sumar_desplazado(R, 2*n, cruzado, 2*(alto+1), m);
    sumar_desplazado(R, 2*n, z0, 2*m, 0);

    free(xBajo);
    free(yBajo);
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
    // Un numero de n bits tiene a lo mas n/3 + 1 digitos decimales
    int maxDigitos = n / 3 + 2;
    int *digitos = calloc(maxDigitos, sizeof(int));
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

    char *resultado = calloc(cantidad+1, sizeof(char));

    for (int i = 0; i < cantidad; i++)
        *(resultado+i) = '0' + *(digitos+cantidad-1-i);

    *(resultado+cantidad) = '\0';

    free(digitos);
    return resultado;
}

// Lee una linea de hasta MAX_DIGITOS caracteres. Regresa 0 si es demasiado larga
int leer_numero(char *buffer) {
    if (fgets(buffer, MAX_DIGITOS + 2, stdin) == NULL)
        return 0;

    char *salto = strchr(buffer, '\n');
    if (salto != NULL) {
        *salto = '\0';
    } else if (!feof(stdin)) {
        // La linea no cupo en el buffer: descarta el resto
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }

    // Elimina el retorno de carro en Windows
    char *retorno = strchr(buffer, '\r');
    if (retorno != NULL)
        *retorno = '\0';

    return 1;
}

int main() {
    char *xDecimal = calloc(MAX_DIGITOS + 2, sizeof(char));
    char *yDecimal = calloc(MAX_DIGITOS + 2, sizeof(char));

    // Lee los dos numeros decimales y verifica que no superen MAX_DIGITOS
    printf("Primer numero: ");
    if (!leer_numero(xDecimal)) {
        printf("Error: maximo %d digitos por numero.\n", MAX_DIGITOS);
        return 1;
    }

    printf("Segundo numero: ");
    if (!leer_numero(yDecimal)) {
        printf("Error: maximo %d digitos por numero.\n", MAX_DIGITOS);
        return 1;
    }

    // Verifica que las entradas sean numericas y no esten vacias
    if (*xDecimal == '\0' || *yDecimal == '\0') {
        printf("Entrada invalida.\n");
        return 1;
    }

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

    // Convierte las entradas a binario.
    // Un numero de d digitos decimales tiene a lo mas 4*d bits
    int *bitsX = calloc(4 * strlen(xDecimal) + 1, sizeof(int));
    int *bitsY = calloc(4 * strlen(yDecimal) + 1, sizeof(int));

    int nx = decimal_a_binario(xDecimal, bitsX);
    int ny = decimal_a_binario(yDecimal, bitsY);

    // Ajusta la longitud a una potencia de dos
    int n = nx > ny ? nx : ny;
    int potencia = 1;

    while (potencia < n)
        potencia *= 2;

    n = potencia;

    int *x = calloc(n, sizeof(int));
    int *y = calloc(n, sizeof(int));

    memcpy(x, bitsX, nx * sizeof(int));
    memcpy(y, bitsY, ny * sizeof(int));

    // Multiplica usando Karatsuba (el resultado tiene 2*n bits)
    int *R = karatsuba(x, y, n);

    // Convierte el resultado a decimal
    char *resultado = binario_a_decimal(R, 2*n);

    printf("Resultado: %s\n", resultado);

    free(xDecimal);
    free(yDecimal);
    free(bitsX);
    free(bitsY);
    free(x);
    free(y);
    free(R);
    free(resultado);

    return 0;
}
