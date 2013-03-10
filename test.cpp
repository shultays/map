#include <stdio.h>
#include "Array2D.h"
int main(){
  Array2D<int> a(4, 5);
  
  a[0][0] = 1;
  for(int i=0; i<4; i++){
    for(int j=0; j<5; j++){
      a.data[i*5+j] = i*10+j;
      
    }
  }
  
  
  FILE *f = fopen("z.txt", "w");
  for(int i=0; i<4; i++){
    for(int j=0; j<5; j++){
    fprintf(f, "%d ", a[i][j]);
  }
  fprintf(f, "\n");
}
  fprintf(f, "\n");
  fprintf(f, "\n");
  a.setAll(42);
    for(int i=0; i<4; i++){
    for(int j=0; j<5; j++){
    fprintf(f, "%d ", a[i][j]);
  }
  fprintf(f, "\n");
}
  fprintf(f, "\n");
  fprintf(f, "\n");
  fclose(f);
  
  return 0;
}

