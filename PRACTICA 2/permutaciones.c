#include <stdio.h>
#include <string.h>
#define TAM 15
void swap(char *a,char *b){
    //a: primer caracter b:seugundo caracter
    char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void permutar(char *cadena, int i,int len, int *counter){
    int j;
    if(i == len){
        (*counter)++;
        printf("\n%d.- %s",*counter,cadena);
    }else{
        for(j = i; j < len; j++){              //recorre el resto de la cadena
            swap((cadena + i),(cadena + j)); //hacer swap
            permutar(cadena,i+1,len,counter);  //llamada recursiva donde se hace swap  hasta llegar al final de la rama, si ya se llego al final de la cadena -> imprime la cadena y decsase los swaps que hizo para buscar otro camino 
            swap((cadena + i),(cadena + j)); //revierte el swap (bactrak) una vez se llega al final de la rama para poder buscar otra aun disponible                    
        }
    }
}
int main(){
    int counter;
    char cadena[TAM];
    counter = 0;
    /*
    fgets agrega un salto de linea al final de la cadena("\n") aumentando 1 al largo de la cadena y, al ser un salto de linea, 
    rompiendo las permutaciones, por lo que hay que cambiarlo por un caracter nulo ("\0")(que indica el fin de la cadena y no se cuena en el len).
    */
    fgets(cadena,TAM,stdin); 
    cadena[strlen(cadena)-1] = '\0'; //el salto de linea siempre estara al final
    permutar(cadena,0,strlen(cadena),&counter);
    return 0;
}