#include <stdio.h>

#define TAM 3

int verificarProgression(int* aptA,float *dif1,float *dif2){
    
    *dif1 = *(aptA + 1) - *aptA;
    *dif2 = *(aptA + 2) - *(aptA + 1);
    if(*dif1 == *dif2) return 0;//0 para PA,1 para PG     

    if(*aptA == 0 || *(aptA + 1) == 0 || *(aptA + 2) == 0) return -1; //si algun numero es 0, no puede haber serie geometrica 
    
    *dif1 = (float)*(aptA + 1) / *aptA;
    *dif2 = (float)*(aptA + 2) / *(aptA + 1);
    if(*dif1 == *dif2)return 1;//0 para PA,1 para PG     
    
    return -1; //-1 si no es ninguna
}
float nextNumber(int *aptA,float *dif1,float *dif2, int*serie){
    float lastNumber;
    *serie = verificarProgression(aptA,dif1,dif2);
    if(*serie == 0){
        lastNumber = *(aptA + TAM - 1) + *dif1;
    }else if(*serie == 1){
        lastNumber = *(aptA + TAM - 1) * *dif1;
    }else if(*serie == -1){
        return -1;
    }
    return lastNumber;
}
int main(){
    int nums[TAM],serie;
    float dif1,dif2,lastNum;
    int *aptA;
    
    aptA = nums;

    for (int i = 0; i < TAM; i++) {
        scanf("%d", (aptA+i));
    }
    lastNum = nextNumber(aptA,&dif1,&dif2,&serie);
    if(serie != -1 && lastNum != -1){
        if(serie == 0){
            printf("PA %f",lastNum);
        }else{
            printf("PG %f",lastNum);
        }
    }else{
        printf("No es PA ni PG.");
    }
    
    return 0;
}