#include<stdio.h>
#include<string.h>

int glob_cont = 1;

int permutar(char*,char*,int);
void swap(char*,char*);

int main(){
    char cad[17];
    char resp[17];
  
    printf("Ingrese la cadena para permutar: ");
    fgets(cad,17,stdin);
    strcpy(resp,"");
    permutar(cad,resp,0);

  return 0;
}

int permutar(char* str,char* aux,int aux_ind){
  char respaldo[17];
  int len = strlen(str)-1;
  int str_ind = 0;
  
  strcpy(respaldo,str);
  
  
  if(len==0){
    printf("Cadena vacía");
    return -1;
  }
  if(len==1){
      aux[aux_ind] = str[0];
      aux[aux_ind+1] = '\0';
      //str[str_ind];
      printf("%d - %s\n",glob_cont++,aux);
    
      return 0;
  }
  
  while(str_ind<len){
    swap(&str[0],&str[str_ind]);
  
    aux[aux_ind] = str[0];
    permutar(str+1,aux,aux_ind+1);
    str_ind++;
  }
  strcpy(str,respaldo);
  
  return 0;
}

void swap(char* a, char* b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}