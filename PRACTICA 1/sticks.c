#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int *encontrarDuplicados(int *size, int **pointerA, int *duplicatedSize){
    int *duplicatedSticks = malloc((*size) * sizeof(int));
    int *result = duplicatedSticks;
    int *pointer = *pointerA; 
    *duplicatedSize = 0;

    for(int i = 0; i < *size; ++i){
        for(int j = 0; j < i; ++j){
            //busca si hay duplicados
            if(*(pointer + i) == *(pointer + j)){
                
                bool alreadyAdded = false;
                //busca si este elemento duplicado ya esta en el el arrego activa una flag
                for(int k = 0; k < *duplicatedSize; ++k){
                    if(*(duplicatedSticks + k) == *(pointer + i)){
                        alreadyAdded = true;
                        break;
                    }
                }
                //si la flag esta activa entonces no lo agrega
                if(!alreadyAdded){
                    *result = *(pointer + i);
                    result++;
                    (*duplicatedSize)++;
                }
                break;
            }
        }
    }

    return duplicatedSticks;
}
void maxNumber(int **duplicated,int sizeDuplicated,int *max1, int *max2){
    int *pointer = *duplicated;
    for(int i = 0; i  < sizeDuplicated;++i){
        if(*(pointer + i)  > *max1) *max1 = *(pointer + i);
    }
    for(int i = 0; i  < sizeDuplicated;++i){
        if(*(pointer + i) > *max2 && *(pointer + i) < *max1)*max2 = *(pointer + i);
    }
}
int calculateArea(int *size, int **pointerA){
    if(*size < 4) return -1;
    int sizeDuplicated = 0;
    int *duplicated = encontrarDuplicados(size, pointerA, &sizeDuplicated);
    if(sizeDuplicated < 2){
        free(duplicated);
        return -1;
    }
    int max1 = 0;
    int max2 = 0;
    maxNumber(&duplicated,sizeDuplicated,&max1,&max2);
    int area;
    area = (max1) * (max2);
    free(duplicated);
    return area;
}

int main(){
    int sticks;
    int *space_separated;
    int *pointerA;

    scanf("%d", &sticks);

    space_separated = malloc(sticks * sizeof(int));

    for(int i = 0; i < sticks; ++i){
        scanf("%d", space_separated + i);
    }

    pointerA = space_separated;

    printf("%d\n", calculateArea(&sticks, &pointerA));

    free(space_separated);

    return 0;
}