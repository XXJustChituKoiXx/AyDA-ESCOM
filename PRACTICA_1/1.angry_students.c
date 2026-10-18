#include<stdio.h>
#include<string.h>

char* act_cad(char*,int,int*);

int main(){
  int k;
   
  printf("======================== Angry Students ========================\n\n");
  printf("Ingrese la cantidad de estudiantes: ");
  scanf("%d",&k);
  if(k<=0 || k>100){
    printf("El valor de K dado no esta en los limites");
    return 1;
  }
  
 char s[k+1];
  printf("\nIngrese la cadena [A/P]: ");
  scanf("%s",s);
  strupr(s);

  int i;
  for(i=0;i<k;i++){
    if(s[i]!='A' && s[i]!='P'){
      printf("La cadena tiene un caracter invalido o esta incompleta");
      return 2;
    } 
  }
  printf("\n====================================================================\n");
  printf("\nEstado inicial: %s\n",s);
  i = 0;
  int terminado = 0;
  
  while(1){
    act_cad(s,k,&terminado);
    if(!terminado)
      printf("Estado despues de %d minutos: %s\n",++i,s);
    else
      break;
  }
  
  printf("\nYa no se registraron cambios en los estudiantes despues de %d minutos\n",i);
  
  return 0;
}

char* act_cad(char* s,int k,int* term){
  int cambios = 0;
  int i = 0;
  while(i<k-1){
    if(s[i]=='A' && s[i+1]=='P'){
      s[i+1]='A';
      cambios = 1;
      i += 2;
    }
    else
      i++;
  }
  
  *term = cambios == 0 ? 1 : 0;
  return s;
}