 #include "water.h"
 
 void riverFlow(){
  vector<struct RiverNode> nodes(1024*1024);
  int k=0;
  for(int i=0; i<1024; i++){
    for(int j=0; j<1024; j++){
      nodes[k].x  = i;
      nodes[k].y  = j;
     nodes[k].w  = map[i][j];
      k++;
    }
  }
  sort(nodes.begin(), nodes.end(), cmp);
    
  sup.setAll(100);
  for(int a=0; a<1024*1024; a++){
    int i=nodes[a].x;
    int j=nodes[a].y;
    float f = norms[i][j].z;
    if(map[i][j] < 100) continue;
    float over = sup[i][j];
    
   // sup[i][j] -= over;
    
    float t = 0;
    if(i>0 && map[i][j] > map[i-1][j] ) t+= map[i][j]-map[i-1][j];
    if(j>0 && map[i][j] > map[i][j-1] ) t+= map[i][j]-map[i][j-1];
    
    
    
    if(i<1023 && map[i][j] > map[i+1][j] ) t+= map[i][j]-map[i+1][j];
    if(j<1023 && map[i][j] > map[i][j+1] ) t+= map[i][j]-map[i][j+1];
    
    if(t==0) continue;
    
    if(i>0 && map[i][j] > map[i-1][j] ) sup[i-1][j] += over*(map[i][j]-map[i-1][j])/t;
    if(j>0 && map[i][j] > map[i][j-1] ) sup[i][j-1] += over*(map[i][j]-map[i][j-1])/t;
    
    if(i<1023 && map[i][j] > map[i+1][j] ) sup[i+1][j] += over*(map[i][j]-map[i+1][j])/t;
    if(j<1023 && map[i][j] > map[i][j+1] ) sup[i][j+1] += over*(map[i][j]-map[i][j+1])/t;
    
  }
  float max = -1;
  for(int i=0; i<1024; i++){
    for(int j=0; j<1024; j++){
      
      if(i%10==0 && j%10==0) fprintf(fout, "%.2f\t", sup[i][j]);
      
    if(map[i][j] < 100) continue;
      if(maxsup<sup[i][j]) maxsup = sup[i][j];
      if(minsup>sup[i][j]) minsup = sup[i][j];
    }
    if(i%10==0) fprintf(fout, "\n");
  }
  
  k=0;
  for(int i=0; i<1024; i++){
    for(int j=0; j<1024; j++){
      nodes[k].x  = i;
      nodes[k].y  = j;
      nodes[k].w  = sup[i][j];
      k++;
    }
  }
  
  sort(nodes.begin(), nodes.end(), cmp);
  
  
  sup.setAll(0);
  
 
  for(int a=0; a<10; a++){
    
    sup[nodes[a].x][nodes[a].y] = 1;
  }
}
