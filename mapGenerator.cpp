#include "mapGenerator.h"

#include <list>

#include <vector>
#include <algorithm>
#include "Perlin.h"


using namespace std;


void waterFix(){
  int i, j;
  int a, b;
  mw = 0;
  for(i=0; i<MAPSIZE; i++){
    for(j=0; j<MAPSIZE; j++){
      temp[i][j] = -1;
    }
  }
       
  for(i=1; i<MAPSIZE; i++){
    for(j=1; j<MAPSIZE; j++){
      if(map[i][j] < 100){
        int k;
        if(temp[i-1][j] >= 0 && temp[i][j-1] >= 0 && temp[i-1][j] == temp[i][j-1]){
          temp[i][j] = temp[i-1][j];
          water[temp[i][j]]++;
          k = temp[i][j];
        }else if(temp[i-1][j] >= 0 && temp[i][j-1] >= 0 && temp[i-1][j] != temp[i][j-1]){
          mw--;
          
          int r = temp[i-1][j]; 
          int d = temp[i][j-1];
          if(water[r] > water[d]){
            int t = r;
            r = d;
            d = t;            
          }
          if(d == mw){
            int t = r;
            r = d;
            d = t;            
          }
          
          temp[i][j] = d;
          water[d]++;
          int z=0;
          for(a=waterB[r][0]; a<=waterB[r][1]  ; a++){
            for(b=waterB[r][2]; b<=waterB[r][3]; b++){
              if(temp[a][b] == r){ z++; temp[a][b] = d;}
            }
          }
          
          for(a=waterB[mw][0]; a<=waterB[mw][1]  ; a++){
            for(b=waterB[mw][2]; b<=waterB[mw][3]; b++){
              if(temp[a][b] == mw) temp[a][b] = r;
            }
          }
          char bu[100];
          if(z != water[r]){
          sprintf(bu, "%d %d -- %d", z, water[r], mw);
//              MessageBox (HWND_DESKTOP, bu, "Error water size", MB_OK | MB_ICONEXCLAMATION);
           //   
           exit(0); 
          }
          
          water[d] += water[r];
          water[r] = 0;
          water[r] = water[mw];
          water[mw] = 0;
          
          
          if(waterB[d][0] > waterB[r][0]) waterB[d][0] = waterB[r][0];
          if(waterB[d][1] < waterB[r][1]) waterB[d][1] = waterB[r][1];
          if(waterB[d][2] > waterB[r][2]) waterB[d][2] = waterB[r][2];
          if(waterB[d][3] < waterB[r][3]) waterB[d][3] = waterB[r][3];
          
          waterB[r][0] = waterB[mw][0];
          waterB[r][1] = waterB[mw][1];
          waterB[r][2] = waterB[mw][2];
          waterB[r][3] = waterB[mw][3];
        
          k = d;
        }else if(temp[i-1][j] >= 0){
          temp[i][j] = temp[i-1][j];
          water[temp[i][j]]++;    
          
          k = temp[i][j];
          
        }else if(temp[i][j-1] >= 0){
          temp[i][j] = temp[i][j-1];
          water[temp[i][j]]++;   
          k = temp[i][j];
        }else{
          temp[i][j] = mw;
          water[mw]=1;
          waterB[mw][0] = i;
          waterB[mw][1] = i;
          waterB[mw][2] = j;
          waterB[mw][3] = j;
          k = mw;
          mw++;
        }
        
        if(waterB[k][0] > i) waterB[k][0] = i;
        if(waterB[k][1] < i) waterB[k][1] = i;
        if(waterB[k][2] > j) waterB[k][2] = j;
        if(waterB[k][3] < j) waterB[k][3] = j;
      }else{
        temp[i][j] = -2; 
      }
    }
  }
  

  for(a=0; a<mw; a++){
    if(water[a] < 100){
      water[a] = 0;
      mw--;
      water[a] = water[mw];
      for(i=waterB[a][0]; i<=waterB[a][1]; i++){
        for(j=waterB[a][2]; j<=waterB[a][3]; j++){
          if(temp[i][j] == a) temp[i][j] = -1;
        }
      }
      
      
      for(i=waterB[mw][0]; i<=waterB[mw][1]; i++){
        for(j=waterB[mw][2]; j<=waterB[mw][3]; j++){
          if(temp[i][j] == mw) temp[i][j] = a;
        }
      }
      
      waterB[a][0] = waterB[mw][0];
      waterB[a][1] = waterB[mw][1];
      waterB[a][2] = waterB[mw][2];
      waterB[a][3] = waterB[mw][3];
      a--;
    }
  }
  
  for(a=0; a<mw; a++){
    b=0;
      
    for(i=1; i<MAPSIZE; i++){
      for(j=1; j<MAPSIZE; j++){
        if(temp[i][j] == a) b++;
        
      }
    }
            
    if(b != water[a]){
//       MessageBox (HWND_DESKTOP, "11 Error water", "Error", MB_OK | MB_ICONEXCLAMATION);
       exit(0);
    }
  }
}



void landFix(){
  int i, j;
  int a, b;
  mw = 0;
  for(i=0; i<MAPSIZE; i++){
    for(j=0; j<MAPSIZE; j++){
      temp[i][j] = -1;
    }
  }
       
  for(i=1; i<MAPSIZE; i++){
    for(j=1; j<MAPSIZE; j++){
      if(map[i][j] >= 100){
        int k;
        if(temp[i-1][j] >= 0 && temp[i][j-1] >= 0 && temp[i-1][j] == temp[i][j-1]){
          temp[i][j] = temp[i-1][j];
          water[temp[i][j]]++;
          k = temp[i][j];
        }else if(temp[i-1][j] >= 0 && temp[i][j-1] >= 0 && temp[i-1][j] != temp[i][j-1]){
          mw--;
          
          int r = temp[i-1][j]; 
          int d = temp[i][j-1];
          if(water[r] > water[d]){
            int t = r;
            r = d;
            d = t;            
          }
          if(d == mw){
            int t = r;
            r = d;
            d = t;            
          }
          
          temp[i][j] = d;
          water[d]++;
          int z=0;
          for(a=waterB[r][0]; a<=waterB[r][1]  ; a++){
            for(b=waterB[r][2]; b<=waterB[r][3]; b++){
              if(temp[a][b] == r){ z++; temp[a][b] = d;}
            }
          }
          
          for(a=waterB[mw][0]; a<=waterB[mw][1]  ; a++){
            for(b=waterB[mw][2]; b<=waterB[mw][3]; b++){
              if(temp[a][b] == mw) temp[a][b] = r;
            }
          }
          char bu[100];
          if(z != water[r]){
          sprintf(bu, "%d %d -- %d", z, water[r], mw);
//              MessageBox (HWND_DESKTOP, bu, "Error water size", MB_OK | MB_ICONEXCLAMATION);
           //   
           exit(0); 
          }
          
          water[d] += water[r];
          water[r] = 0;
          water[r] = water[mw];
          water[mw] = 0;
          
          
          if(waterB[d][0] > waterB[r][0]) waterB[d][0] = waterB[r][0];
          if(waterB[d][1] < waterB[r][1]) waterB[d][1] = waterB[r][1];
          if(waterB[d][2] > waterB[r][2]) waterB[d][2] = waterB[r][2];
          if(waterB[d][3] < waterB[r][3]) waterB[d][3] = waterB[r][3];
          
          waterB[r][0] = waterB[mw][0];
          waterB[r][1] = waterB[mw][1];
          waterB[r][2] = waterB[mw][2];
          waterB[r][3] = waterB[mw][3];
        
          k = d;
        }else if(temp[i-1][j] >= 0){
          temp[i][j] = temp[i-1][j];
          water[temp[i][j]]++;    
          
          k = temp[i][j];
          
        }else if(temp[i][j-1] >= 0){
          temp[i][j] = temp[i][j-1];
          water[temp[i][j]]++;   
          k = temp[i][j];
        }else{
          temp[i][j] = mw;
          water[mw]=1;
          waterB[mw][0] = i;
          waterB[mw][1] = i;
          waterB[mw][2] = j;
          waterB[mw][3] = j;
          k = mw;
          mw++;
        }
        
        
  
        if(waterB[k][0] > i) waterB[k][0] = i;
        if(waterB[k][1] < i) waterB[k][1] = i;
        if(waterB[k][2] > j) waterB[k][2] = j;
        if(waterB[k][3] < j) waterB[k][3] = j;
      }else{
        temp[i][j] = -2; 
      }
    }
  }
  

  for(a=0; a<mw; a++){
    if(water[a] < 16){
      water[a] = 0;
      mw--;
      water[a] = water[mw];
      for(i=waterB[a][0]; i<=waterB[a][1]; i++){
        for(j=waterB[a][2]; j<=waterB[a][3]; j++){
          if(temp[i][j] == a){ temp[i][j] = -1; map[i][j] = 99-(rand()%10)/10.0f; }
        }
      }
      
      
      for(i=waterB[mw][0]; i<=waterB[mw][1]; i++){
        for(j=waterB[mw][2]; j<=waterB[mw][3]; j++){
          if(temp[i][j] == mw) temp[i][j] = a;
        }
      }
      
      waterB[a][0] = waterB[mw][0];
      waterB[a][1] = waterB[mw][1];
      waterB[a][2] = waterB[mw][2];
      waterB[a][3] = waterB[mw][3];
      a--;
    }
  }
  
  for(a=0; a<mw; a++){
    b=0;
      
    for(i=1; i<MAPSIZE; i++){
      for(j=1; j<MAPSIZE; j++){
        if(temp[i][j] == a) b++;
        
      }
    }
            
    if(b != water[a]){
//       MessageBox (HWND_DESKTOP, "11 Error water", "Error", MB_OK | MB_ICONEXCLAMATION);
       exit(0);
    }
  }
}

bool cmp(struct RiverNode a, struct RiverNode b){
   return (a.w > b.w); 
}
  
void riverErode(){
  for(int a=0; a<riverCount; a++){
    
    for(int b=0; b<rivers[a].size(); b++){
      
      rivers[riverCount][b];
    }
    
  } 
}


int createRiver(int beginX, int beginY){
  
  int i = beginX, j = beginY, k;
  int a, b;
  
  list<struct RiverNode> river;
  struct RiverNode n;
  int waterEndX = 1000000;
  int  waterEndY = 1000000;
  for(a=maxv(1, i-10); a<minv(MAPSIZE-1, i+10); a++){
    for(b=maxv(1, j-10); b<minv(MAPSIZE-1, j+10); b++){
      if(map[a][b] < 100){
        if(abs(waterEndX-i)+abs(waterEndY-j) > abs(a-i)+abs(b-j)){
            waterEndX = a;
            waterEndY = b;      
        }
      }
    }
  }


          
  a = i; b=j;
  while(a != waterEndX && b!=waterEndY){
    
    
    if(rand()&1){
      if(a<waterEndX) a++;
      else a--;
    }else{
      if(b<waterEndY) b++;
      else b--;
    }
    
    river.push_front( (struct RiverNode){a, b, 0} );
    
  }
  
  if(a<waterEndX) while(++a<waterEndX){ river.push_front( (struct RiverNode){a, b, 0} );}
  if(a>waterEndX) while(--a>waterEndX){ river.push_front( (struct RiverNode){a, b, 0} );}
  
  if(b<waterEndY) while(++b<waterEndY){ river.push_front( (struct RiverNode){a, b, 0} );}
  if(b>waterEndY) while(--b>waterEndY){ river.push_front( (struct RiverNode){a, b, 0} );}
  
    
  river.push_front( (struct RiverNode){a, b, 0} );
  
  i = beginX;
  j = beginY;
  int subr = 0;
  fprintf(fout, "1\n");
  
  while(1){
  fprintf(fout, ".");
    if(map[i][j] < 100 || map[i][j] > 190) break;
    if(i<1||j<1||i>MAPSIZE-2||j>MAPSIZE-2) break;
    if(sup2[i][j] == 1 || sup2[i][j] == 2){
       subr = 1;
       break;
    }
    
    n.x = i;
    n.y = j;
    n.w = 0;
    river.push_back(n);
    
    sup2[i][j] = 3;
    float max = -1000;
    int maxd=-1;
    for(k=0; k<4; k++){
      a = i+dir[k][0];
      b = j+dir[k][1];
      
      if(!sup2[a][b] && max < sup[a][b] && map[a][b] > map[i][j]-0.1 && map[a][b] < map[i][j]+2 ){
        max = sup[a][b];
        maxd = k;
      }
      
    }
    if(maxd != -1){
      i += dir[maxd][0];
      j += dir[maxd][1];
      
      
    }else{
      break;
    }
  }
  fprintf(fout, "2\n");
  if(river.size() < 20 && subr == 0){
    
  for (list<struct RiverNode>::iterator it = river.begin(); it != river.end(); it++)
      sup2[it->x][it->y] = 0;
    return 0;
  }
  fprintf(fout, "3\n");
  
  list<struct RiverNode>::iterator it = river.begin();
  sup2[it->x][it->y] = 1;
  rivers[riverCount].push_back((int3){it->x, it->y, 5});
  
  
  int oldx = it->x;
  int oldy = it->y;
  it++;
  for(; it != river.end(); it++){
    sup2[it->x][it->y] = 1;
    
    
    if(oldx < it->x)    k = 0;
    else if(oldx > it->x) k = 1;
    else if(oldy < it->y) k = 2;
    else if(oldy > it->y) k = 3;
    
    oldx = it->x;
    oldy = it->y;
    
    rivers[riverCount].push_back((int3){it->x, it->y, k});

    


  }
  /*
  k = 6<<8;
  if(it0->x < it1->x)      k |= 0;
  else if(it0->x > it1->x) k |= 1;
  else if(it0->y < it1->y) k |= 2;
  else if(it0->y > it1->y) k |= 3;
  rivers[riverCount].push_back((int3){it1->x, it1->y, k});
  */
  fprintf(fout, "4\n");
  riverCount++;
  
  sup2[beginX][beginY]  = 2;
  return 1;
}




void riverFlow(){
  riverCount = 0;
  
  fprintf(fout, "hmm5.3.1\n");
  float maxsup = 0;
  float minsup = 1000000000;

  int i, j, a, b;
  vector<struct RiverNode> nodes(MAPSIZE*MAPSIZE);
  int k=0;
  for(i=1; i<MAPSIZE-1; i++){
    for(j=1; j<MAPSIZE-1; j++){
      nodes[k].x  = i;
      nodes[k].y  = j;
      nodes[k].w  = map[i][j];
      k++;
    }
  }
  sort(nodes.begin(), nodes.end(), cmp);
    
  fprintf(fout, "hmm5.3.2\n");
    
  sup.setAll(100);
  for(a=0; a<(MAPSIZE-2)*(MAPSIZE-2); a++){
    int i=nodes[a].x;
    int j=nodes[a].y;
    float f = norms[i][j].z;
    if(map[i][j] < 100) continue;
    float over = sup[i][j];
    
   // sup[i][j] -= over;
    
    float t = 0;
    if(i>0 && map[i][j] > map[i-1][j] ) t+= map[i][j]-map[i-1][j];
    if(j>0 && map[i][j] > map[i][j-1] ) t+= map[i][j]-map[i][j-1];
    
    
    
    if(i<MAPSIZE && map[i][j] > map[i+1][j] ) t+= map[i][j]-map[i+1][j];
    if(j<MAPSIZE && map[i][j] > map[i][j+1] ) t+= map[i][j]-map[i][j+1];
    
    if(t==0) continue;
    
    if(i>0 && map[i][j] > map[i-1][j] ) sup[i-1][j] += over*(map[i][j]-map[i-1][j])/t;
    if(j>0 && map[i][j] > map[i][j-1] ) sup[i][j-1] += over*(map[i][j]-map[i][j-1])/t;
    
    if(i<MAPSIZE-1 && map[i][j] > map[i+1][j] ) sup[i+1][j] += over*(map[i][j]-map[i+1][j])/t;
    if(j<MAPSIZE-1 && map[i][j] > map[i][j+1] ) sup[i][j+1] += over*(map[i][j]-map[i][j+1])/t;
    
  }
  fprintf(fout, "hmm5.3.3\n");
  float max = -1;
  for(i=1; i<MAPSIZE-1; i++){
    for(j=1; j<MAPSIZE-1; j++){

      
    if(map[i][j] < 100) continue;
      if(maxsup<sup[i][j]) maxsup = sup[i][j];
      if(minsup>sup[i][j]) minsup = sup[i][j];
    }
   // if(i%10==0) fprintf(fout, "\n");
  }
  
  fprintf(fout, "hmm5.3.4\n");
  k=0;
  for(i=1; i<MAPSIZE; i++){
    for(j=1; j<MAPSIZE; j++){
      nodes[k].x  = i;
      nodes[k].y  = j;
      nodes[k].w  = sup[i][j];
      k++;
    }
  }
  
  sort(nodes.begin(), nodes.end(), cmp);
  
  sup2.setAll(0);
  
  int ibeg, jbeg;
  
  fprintf(fout, "hmm5.3.5\n");
  
  int ri=max_river;
  int mink=0;
  int waterEndX, waterEndY;
  while(ri){
    
  for(k=mink; k<(MAPSIZE-2)*(MAPSIZE-2); k++){
    
    i=nodes[k].x;
    j=nodes[k].y;
    if(sup2[i][j] ) continue;
    int f=0;
    waterEndX = 1000000;
    waterEndY = 1000000;
    for(a=maxv(1, i-10); a<minv(MAPSIZE-1, i+10)&& f != 2; a++){
      for(b=maxv(1, j-10); b<minv(MAPSIZE-1, j+10) && f != 2; b++){
        if(sup2[a][b] == 2) f = 2;
        else if(map[a][b] < 99){
          f = 1;
      
        }
      }
    }
    if(f==2) continue;
    if(f==1) break;
  }
  
  if(k==(MAPSIZE-2)*(MAPSIZE-2)){
    // MessageBox (HWND_DESKTOP, "", "Error river", MB_OK | MB_ICONEXCLAMATION);
     break;
  }
  mink = k+1;
  


  k = createRiver(i, j);
  
  if(k) ri--;
  
  
  }
  
  fprintf(fout, "hmm5.3.6\n");
  
}


void generateMap(){
  
  fprintf(fout, "hmm\n");
  unsigned int *partInd[partialTerrainCount][partialTerrainCount];
  
  
  int i, j;
  int z=0;
  
  memset(ind, 0, sizeof(unsigned int)*MAPSIZE*MAPSIZE*12);
  
  for(i=0; i<partialTerrainCount; i++){
    for(j=0; j<partialTerrainCount; j++){
      partInd[i][j] = ind + ((i+j*partialTerrainCount)*(MAPSIZE*MAPSIZE*12/(partialTerrainCount*partialTerrainCount)));
      partIBSizes[i][j] = 0;
    }    
  }
  
  fprintf(fout, "hmm2\n");
  FILE *mapData;
  if(SAVE)  mapData = fopen("map.dat", "wb");
  if(mapData == 0) fprintf(fout, "mapdat\n");
  Array2D<vec4> cvec(MAPSIZE+2, MAPSIZE+2);
  
  restart:
  z++;

  map.setAll(-50.0f);
  
  
  
  addRandomMapC(MAPSIZE/256,MAPSIZE/64,  50, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/256,MAPSIZE/64,  50, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/128,MAPSIZE/64,  50, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/128,MAPSIZE/64,  50, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/128,MAPSIZE/64,  50, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/64,MAPSIZE/64,  50, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/64,MAPSIZE/64,  50, MAPSIZE, map, 20);
  
  addRandomMapC(MAPSIZE/64,MAPSIZE/64,  50, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/64,MAPSIZE/64,  50, MAPSIZE, map, 20);
  //addRandomMapC(MAPSIZE/64,MAPSIZE/64,  40, MAPSIZE, map, 20);
  
  addRandomMapC(MAPSIZE/256,MAPSIZE/256, 80, MAPSIZE, map, 50);
  
  

  addRandomMap(MAPSIZE/128,MAPSIZE/128, 30, MAPSIZE, map);
  addRandomMap(MAPSIZE/128,MAPSIZE/128, 30, MAPSIZE, map);
  

  addRandomMap(MAPSIZE/64,MAPSIZE/64,  20, MAPSIZE, map);
  addRandomMap(MAPSIZE/64,MAPSIZE/64,  20, MAPSIZE, map);
  
  addRandomMapC(MAPSIZE/32,MAPSIZE/32,  10, MAPSIZE, map, 20);
  addRandomMapC(MAPSIZE/32,MAPSIZE/32,  10, MAPSIZE, map, 20);
  
  
  

  
  addRandomMap(MAPSIZE/8,MAPSIZE/8,  3, MAPSIZE, map);
  addRandomMap(MAPSIZE/8,MAPSIZE/8,  3, MAPSIZE, map);
  
  
  addRandomMap(MAPSIZE/4,MAPSIZE/4,  2,   MAPSIZE, map);
  addRandomMap(MAPSIZE/2,MAPSIZE/2, 1,   MAPSIZE, map);
  addRandomMap(MAPSIZE-1,MAPSIZE-1,0.5, MAPSIZE, map);
  
  //addRandomMap(1023,1023,0.0054, 1024, map);
  
  fprintf(fout, "hmm3\n");
  float minh=10000, maxh=-10000;
  int w=0;
  
  
  
  for(i=0; i<MAPSIZE; i++){
    for(j=0; j<MAPSIZE; j++){
      randSeed[i][j] = rand();
      //map[i][j] = map[i][j]*6-70;
      if(map[i][j] < 100) w++;
      temp[i][j] = 0;
      
      if(map[i][j] < 110) map[i][j] =map[i][j]*0.5+55;
      else if(map[i][j] < 140){
        float h = 1-0.5f*(140-map[i][j])/30.0f;
         map[i][j] = map[i][j]*h + map[i][j]*(1-h);
      }
      
    }
  }
  
  
  
  fprintf(fout, "hmm4\n");
  
  earth.setAll(0);
  addRandomMapC(16,16, 1, MAPSIZE, earth, 20);
  
  
  for(i=0; i<MAPSIZE; i++){
    for(j=0; j<MAPSIZE; j++){
  
      if(map[i][j] > 180) map[i][j] += 50*earth[i][j];
      else if(map[i][j] > 130){
        float h = (map[i][j]-130)*2.0f+130;
         map[i][j] = (1-earth[i][j])*map[i][j] + (earth[i][j])*h;
      }
    }
  }
  
  
  earth.setAll(0);
  addRandomMapC(16,16,0.8, MAPSIZE, earth, 40);
  addRandomMapC(64,64,0.2, MAPSIZE, earth, 50);
  
  fprintf(fout, "hmm5\n");
  
  for(i=0; i<MAPSIZE; i++){
    for(j=0; j<MAPSIZE; j++){
  
      if(map[i][j] > 180) map[i][j] -= 40*earth[i][j];
      else if(map[i][j] > 130){
        float h = (map[i][j]-130)*0.2f+130;
         map[i][j] = (1-earth[i][j])*map[i][j] + (earth[i][j])*h;
      }
    }
  }
  
  
  for(i=0; i<MAPSIZE; i++){
    for(j=0; j<MAPSIZE; j++){
      if(earth[i][j] < 0.2)earth[i][j]=0;
      else earth[i][j] = (earth[i][j]-0.2)*0.7;
    }
  }
  
  dirt.setAll(0);
  
  addRandomMap(MAPSIZE,MAPSIZE,0.2, MAPSIZE, dirt);
  
  fprintf(fout, "hmm5.1\n");
  landFix();
  fprintf(fout, "hmm5.2\n");
  
  waterFix();
  fprintf(fout, "hmm5.3\n");
  riverFlow();
  fprintf(fout, "hmm5.4\n");
 
  
  
  fprintf(fout, "hmm6\n");
  norms.setAll(vec3(0,0,0));
  int n = 0;
  int m = 0;
  
  

  height.setAll(0);
  for(i=1; i<MAPSIZE; i++){
    for(j=1; j<MAPSIZE; j++){
      height[i][j] = (map[i][j] + map[i][j-1] + map[i-1][j] + map[i-1][j-1])/4.0f;
    }
    height[i][0] = height[i][1] ;
    height[0][i] = height[1][i] ;
  }
  height[0][0] = height[1][1] ;
      
      
  for(i=0; i<MAPSIZE-1; i++){
    for(j=0; j<MAPSIZE-1; j++){
      float k = height[i][j]+height[i][j+1]+height[i+1][j]+height[i+1][j+1];
      k/=4;
      map[i][j] = k;
       if(sup2[i][j]) map[i][j]-=1;
    }
    map[j][0] = map[j-1][0];
    map[0][j] = map[0][j-1];
  }
  map[i][i] = map[i-1][i-1];


    
  n=m=0;
  fprintf(fout, "hmm7\n");
  for(i=1; i<MAPSIZE-1; i++){
           
  
    for(j=1; j<MAPSIZE-1; j++){
        
      float k = map[i][j];
      buf[n+0].pos = vec3(2*(i-0.5-MAPSIZE/2),   2*(j-0.5-MAPSIZE/2),   height[i][j]);
      buf[n+1].pos = vec3(2*(i+0.5-MAPSIZE/2),   2*(j-0.5-MAPSIZE/2),   height[i+1][j]);
      buf[n+2].pos = vec3(2*(i+0.5-MAPSIZE/2),   2*(j+0.5-MAPSIZE/2),   height[i+1][j+1]);
      buf[n+3].pos = vec3(2*(i-0.5-MAPSIZE/2),   2*(j+0.5-MAPSIZE/2),   height[i][j+1]);
      buf[n+4].pos = vec3(2*(i    -MAPSIZE/2),   2*(j    -MAPSIZE/2),   k);
      
      int a = i/(MAPSIZE/partialTerrainCount);
      int b = j/(MAPSIZE/partialTerrainCount);
      
      m = partIBSizes[a][b]; 
         
      vec3 n1=normalize(cross( 
      vec3(i,   j,   height[i][j])   - vec3(i+0.5, j+0.5,  k), 
      vec3(i+1, j,   height[i+1][j]) - vec3(i+0.5, j+0.5,  k) ));
      
      partInd[a][b][m+0] = n+0;
      partInd[a][b][m+1] = n+1;
      partInd[a][b][m+2] = n+4;
      
      vec3 n2=normalize(cross(
      vec3(i, j+1,   height[i][j+1]) - vec3(i+0.5, j+0.5,  k), 
      vec3(i,   j,   height[i][j])   - vec3(i+0.5, j+0.5,  k) ));
       
      partInd[a][b][m+3] = n+3;
      partInd[a][b][m+4] = n+0;
      partInd[a][b][m+5] = n+4;
      
      vec3 n3 = normalize(cross(
      vec3(i+1, j,   height[i+1][j])   - vec3(i+0.5, j+0.5,  k), 
      vec3(i+1, j+1, height[i+1][j+1]) - vec3(i+0.5, j+0.5,  k) ));
       
      partInd[a][b][m+6] = n+1;
      partInd[a][b][m+7] = n+2;
      partInd[a][b][m+8] = n+4;
      
      vec3 n4 = normalize(cross(
      vec3(i+1, j+1, height[i+1][j+1]) - vec3(i+0.5, j+0.5,  k), 
      vec3(i, j+1,   height[i][j+1])   - vec3(i+0.5, j+0.5,  k) ));    
       
      partInd[a][b][m+9]  = n+2;
      partInd[a][b][m+10] = n+3;
      partInd[a][b][m+11] = n+4;
      
      buf[n+4].nor =  (n1+n2+n3+n4)/4.0f;
      
      norms[i][j] += (n1+n2)/2.0f;
      norms[i+1][j] += (n1+n3)/2.0f;
      norms[i+1][j+1] += (n3+n4)/2.0f;
      norms[i][j] += (n2+n4)/2.0f;
      
      n+=5;
      partIBSizes[a][b]+=12;
    
    }
  }
  fprintf(fout, "hmm8\n");

  for(i=1; i<MAPSIZE; i++){
    for(j=1; j<MAPSIZE; j++){
      norms[i][j] = normalize(norms[i][j]);
    }    
    norms[i][0] = norms[i][1] ;
    norms[0][i] = norms[1][i] ;
  }
  norms[0][0] = norms[1][1] ;
      
  for(i=1; i<MAPSIZE-1; i++){
    for(j=1; j<MAPSIZE-1; j++){
  
      vec4 c;
      vec4 v1, v2;
      v1 = opts[0];
      float t = earth[i][j];
      float snowHeight = 200+earth[i][j]*40;
      
      if(map[i][j] > snowHeight) t = 0;
      else if(map[i][j] > 150) t *= 1-(map[i][j]-150)/(snowHeight-150.0f);
      
      if(norms[i][j][2] < 0.4) t = 0;
      else if(norms[i][j][2] < 0.9) t*= (norms[i][j][2]-0.4)/0.5 ;
      
      v2 = (1-t)*opts[1] + (t)*vec4(0.0, 0.4, 0.0, 1);
      
      
      float k = map[i][j];
      if(k<100) c = v1;
      else if(k<110){
         float h = (k-100)/10;
         h = sqrt(h);
         c = v2*(h)+v1*(1-h);
         
      }else if (k<snowHeight-20){ 
        c = v2;
        
      }else if (k<snowHeight){
        
        c = v2*(snowHeight-k)/20 + vec4(0.8, 0.8, 0.8, 1)*(1-(snowHeight-k)/20);
      }else{
        c = vec4(0.8, 0.8, 0.8, 1);
      }
       
      float g = (c[0]+c[1]+c[2])/3;
      
      c = c*0.4 + 0.6*(c*dirt[i][j]+vec4(g,g,g,1)*(1-dirt[i][j]));
      
      
      cvec[i][j] = c;
       
        
    }
  //    if(i%10==0 )  fprintf(fout, "\n");
    
    cvec[i][0] = cvec[i][1] ;
    cvec[0][i] = cvec[1][i] ;
  }
  cvec[0][0] = cvec[1][1] ;
  
  n = 0;
  for(i=1; i<MAPSIZE-1; i++){
    for(j=1; j<MAPSIZE-1; j++){
        

        
        buf[n+0].color = vec4touint((cvec[i-1][j-1]+cvec[i-1][j]+cvec[i][j-1]+cvec[i][j])/4.0f);
        buf[n+1].color = vec4touint((cvec[i][j-1]+cvec[i][j]+cvec[i+1][j-1]+cvec[i+1][j])/4.0f);
        buf[n+2].color = vec4touint((cvec[i][j+1]+cvec[i][j]+cvec[i+1][j+1]+cvec[i+1][j])/4.0f);
        buf[n+3].color = vec4touint((cvec[i-1][j+1]+cvec[i-1][j]+cvec[i][j+1]+cvec[i][j])/4.0f);
        buf[n+4].color = vec4touint(cvec[i][j]);
        n+=5;
    }
    
  }

  n = 0;
  for(i=1; i<MAPSIZE-1; i++){
    for(j=1; j<MAPSIZE-1; j++){
      
        buf[n+0].nor =  norms[i][j];
        buf[n+1].nor =  norms[i+1][j];
        buf[n+2].nor =  norms[i+1][j+1];
        buf[n+3].nor =  norms[i][j+1];
        n+=5;
    }
  }


  for(i=0; i<partialTerrainCount; i++){
    for(j=0; j<partialTerrainCount; j++){
      glGenBuffersARB(1, &partIB[i][j]);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, partIB[i][j]);
      glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*partIBSizes[i][j], partInd[i][j], GL_STATIC_DRAW_ARB);
      //fprintf(fout, "%d %d %d %d %d\n", i, j,  partIBSizes[i][j], partIB[i][j], partInd[i][j][0]);
    }  
  }
  glGenBuffersARB(1, &vb);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb); 
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertex)*(MAPSIZE-2)*(MAPSIZE-2)*5, buf, GL_STATIC_DRAW_ARB);
 
  glGenBuffersARB(1, &ib);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ib);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*MAPSIZE*MAPSIZE*12, ind, GL_STATIC_DRAW_ARB);
      
 
  memcpy(bufBig, buf, sizeof(Vertex)*(MAPSIZE-2)*(MAPSIZE-2)*5);
  
  if(SAVE){
           
    fwrite(randSeed, sizeof(randSeed), 1, mapData);
    
    fwrite(buf, sizeof(Vertex)*(MAPSIZE-2)*(MAPSIZE-2)*5, 1, mapData);
    fwrite(ind, sizeof(unsigned int)*MAPSIZE*MAPSIZE*12, 1, mapData);
    
    for(i=0; i<partialTerrainCount; i++){
      for(j=0; j<partialTerrainCount; j++){
        fwrite(&partIBSizes[i][j], sizeof(unsigned int), 1, mapData);
        fwrite(partInd[i][j], sizeof(unsigned int)*partIBSizes[i][j], 1, mapData);
      }  
    }
  
      
    fwrite(map.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
    
    fwrite(earth.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
    fwrite(dirt.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
    
    fwrite(temp.data, sizeof(int)*(MAPSIZE+2)*(MAPSIZE+2), 1, mapData);
    fwrite(norms.data, sizeof(vec3)*MAPSIZE*MAPSIZE, 1, mapData);
    
    fwrite(&mw, sizeof(int), 1, mapData);
  
  }
  
 
    
   
  for(int a=0; a<mw; a++){
    int n=0, m=0;
    int t=0;
    for(i=1; i<MAPSIZE-1; i++){
      for(j=1; j<MAPSIZE-1; j++){
        if(temp[i][j] == a 
        || temp[i+1][j+1] == a || temp[i+1][j] == a|| temp[i][j+1] == a
        || temp[i-1][j-1] == a || temp[i-1][j] == a|| temp[i][j-1] == a
        || temp[i-1][j+1] == a || temp[i+1][j-1] == a
        
        ){
          t++;
          vec4 c2=opts[2];
          
          unsigned int c = vec4touint(c2);
          buf[n+0].color = c;
          buf[n+1].color = c;
          buf[n+2].color = c;
          buf[n+3].color = c;
          buf[n+4].color = c;
          
          buf[n+0].pos = vec3(2*(i-0.5-MAPSIZE/2),   2*(j-0.5-MAPSIZE/2), 100);
          buf[n+1].pos = vec3(2*(i+0.5-MAPSIZE/2),   2*(j-0.5-MAPSIZE/2), 100);
          buf[n+2].pos = vec3(2*(i+0.5-MAPSIZE/2),   2*(j+0.5-MAPSIZE/2), 100);
          buf[n+3].pos = vec3(2*(i-0.5-MAPSIZE/2),   2*(j+0.5-MAPSIZE/2), 100);
          buf[n+4].pos = vec3(2*(i    -MAPSIZE/2),   2*(j    -MAPSIZE/2), 100);
      
        
          buf[n+0].nor = buf[n+1].nor = buf[n+2].nor = buf[n+3].nor = buf[n+4].nor = vec3(0, 0, 1);
          
          ind[m+0]=n+0;
          ind[m+1]=n+1;
          ind[m+2]=n+4;
          
          ind[m+3]=n+3;
          ind[m+4]=n+0;
          ind[m+5]=n+4;
          
          ind[m+6]=n+1;
          ind[m+7]=n+2;
          ind[m+8]=n+4;
          
          ind[m+9]=n+2;
          ind[m+10]=n+3;
          ind[m+11]=n+4;
          n+=5;
          m+=12;
        }
      }
    }
    
    glGenBuffersARB(1, &wvb[a]);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, wvb[a]); 
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertex)*n, buf, GL_STATIC_DRAW_ARB);
 
 
    glGenBuffersARB(1, &wib[a]);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, wib[a]);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*m, ind, GL_STATIC_DRAW_ARB);
    wsizes[a] = m;
 
    if(SAVE){
      
      fwrite(&n, sizeof(int), 1, mapData);
      fwrite(&m, sizeof(int), 1, mapData);
    
      fwrite(buf, sizeof(Vertex)*n, 1, mapData);
      fwrite(ind, sizeof(unsigned int)*m, 1, mapData);
    }
 
  }
 
  if(SAVE){
    fwrite(&riverCount, sizeof(int), 1, mapData);
    fwrite(sup2.data, sizeof(char)*MAPSIZE*MAPSIZE, 1, mapData);
    fwrite(sup.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
  }
  
  fprintf(fout, "hmm8\n");
  for(int a=0; a<riverCount; a++){
    int n = 0;
    int m = 0;
    
    vec4 c2 = opts[2];
    unsigned int c = vec4touint(c2);
    if(SAVE){
        int k = rivers[a].size();
        fwrite(&k, sizeof(int), 1, mapData);
    }
     
    for(int b=0; b<rivers[a].size(); b++){
      
      if(SAVE){ 
        fwrite(&rivers[a][b], sizeof(int)*3, 1, mapData);
      }
    
      i=rivers[a][b].x;
      j=rivers[a][b].y;
      
      
      vec3 v0 = buf[n+0].pos = vec3(2*(i-0.5-(MAPSIZE/2)),   2*(j-0.5-(MAPSIZE/2)), height[i][j]);
      vec3 v1 = buf[n+1].pos = vec3(2*(i+0.5-(MAPSIZE/2)),   2*(j-0.5-(MAPSIZE/2)), height[i+1][j]);
      vec3 v2 = buf[n+2].pos = vec3(2*(i+0.5-(MAPSIZE/2)),   2*(j+0.5-(MAPSIZE/2)), height[i+1][j+1]);
      vec3 v3 = buf[n+3].pos = vec3(2*(i-0.5-(MAPSIZE/2)),   2*(j+0.5-(MAPSIZE/2)), height[i][j+1]);
      vec3 v4 = buf[n+4].pos = vec3(2*(i    -(MAPSIZE/2)),   2*(j    -(MAPSIZE/2)), map[i][j]+0.1);
      
      
      if(b==0||b==rivers[a].size()-1){
        
        
        int yy;
        if(b==0){
          yy = rivers[a][b+1].z;
          if(yy < 2) yy = 1-yy;
          if(yy >= 2) yy = 5-yy;
        }else{
          yy = rivers[a][b].z;
        }
        if(yy==0){
          
          buf[n+0].pos = (v0*3.0f+v3)/4.0f;
          buf[n+3].pos = (v3*3.0f+v0)/4.0f;
          if(b)buf[n+1].pos = buf[n+2].pos = v4;
        }else if(yy==1){
          
          buf[n+1].pos = (v1*3.0f+v2)/4.0f;
          buf[n+2].pos = (v2*3.0f+v1)/4.0f;
          if(b)buf[n+0].pos = buf[n+3].pos = v4;
        }else if(yy==2){
          
          buf[n+1].pos = (v1*3.0f+v0)/4.0f;
          buf[n+0].pos = (v0*3.0f+v1)/4.0f;
          
          if(b) buf[n+2].pos = buf[n+3].pos = v4;
        }else if(yy==3){
          
          buf[n+2].pos = (v2*3.0f+v3)/4.0f;
          buf[n+3].pos = (v3*3.0f+v2)/4.0f;
          
          if(b)buf[n+0].pos = buf[n+1].pos = v4;
        }else{
          c = 0xFF000000;
        }
        
      }else if(b != 0 && b!=rivers[a].size()-1){
        int xx = rivers[a][b].z;
        int yy = rivers[a][b+1].z;
        
        if( (xx==2&&yy==2) || (xx==3&&yy==3)){
          buf[n+1].pos = (v1*3.0f+v0)/4.0f;
          buf[n+0].pos = (v0*3.0f+v1)/4.0f;
          
          buf[n+2].pos = (v2*3.0f+v3)/4.0f;
          buf[n+3].pos = (v3*3.0f+v2)/4.0f;
        }else if( (xx==0&&yy==0) || (xx==1&&yy==1)){
          
          buf[n+1].pos = (v1*3.0f+v2)/4.0f;
          buf[n+2].pos = (v2*3.0f+v1)/4.0f;
          
          buf[n+0].pos = (v0*3.0f+v3)/4.0f;
          buf[n+3].pos = (v3*3.0f+v0)/4.0f;
        }else if( (xx==0&&yy==2) || (xx==3&&yy==1)){
          
          buf[n+0].pos = (v0*3.0f+v3)/4.0f;
          buf[n+3].pos = (v3*3.0f+v0)/4.0f;
          
          
          buf[n+2].pos = (v3*3.0f+v2)/4.0f;
          buf[n+1].pos = (v2*3.0f+v3)/4.0f;
          
         // buf[n+4].pos = (v4+v3)/2.0f;
          
        }else if( (xx==0&&yy==3) || (xx==2&&yy==1)){
          
          buf[n+0].pos = (v0*3.0f+v3)/4.0f;
          buf[n+3].pos = (v3*3.0f+v0)/4.0f;
          
          
          buf[n+1].pos = (v0*3.0f+v1)/4.0f;
          buf[n+2].pos = (v1*3.0f+v0)/4.0f;
          
          //buf[n+4].pos = (v4+v0)/2.0f;
          
          
          
        }else if( (xx==1&&yy==2) || (xx==3&&yy==0)){
          
          buf[n+3].pos = (v1*3.0f+v2)/4.0f;
          buf[n+0].pos = (v2*3.0f+v1)/4.0f;
          
          
          buf[n+2].pos = (v3*3.0f+v2)/4.0f;
          buf[n+1].pos = (v2*3.0f+v3)/4.0f;
          
         // buf[n+4].pos = (v4+v2)/2.0f;
          
        }else if( (xx==1&&yy==3) || (xx==2&&yy==0)){
          
          buf[n+3].pos = (v1*3.0f+v2)/4.0f;
          buf[n+0].pos = (v2*3.0f+v1)/4.0f;
          
          
          buf[n+1].pos = (v0*3.0f+v1)/4.0f;
          buf[n+2].pos = (v1*3.0f+v0)/4.0f;
          
         // buf[n+4].pos = (v4+v1)/2.0f;
          
        }
        
      }
       buf[n+4].pos =  (buf[n+0].pos+buf[n+1].pos+buf[n+2].pos+buf[n+3].pos)/4.0f+0.1f;
      buf[n+0].color = c;
      buf[n+1].color = c;
      buf[n+2].color = c;
      buf[n+3].color = c;
      buf[n+4].color = c;
      c =  vec4touint(c2);;
      
      buf[n+0].nor = buf[n+1].nor = buf[n+2].nor = buf[n+3].nor = buf[n+4].nor = vec3(0, 0, 1);
      
      ind[m+0]=n+0;
      ind[m+1]=n+1;
      ind[m+2]=n+4;
      
      ind[m+3]=n+3;
      ind[m+4]=n+0;
      ind[m+5]=n+4;
      
      ind[m+6]=n+1;
      ind[m+7]=n+2;
      ind[m+8]=n+4;
      
      ind[m+9]=n+2;
      ind[m+10]=n+3;
      ind[m+11]=n+4;
      n+=5;
      m+=12;
    
    }
      
    glGenBuffersARB(1, &rivervb[a]);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, rivervb[a]); 
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertex)*n, buf, GL_STATIC_DRAW_ARB);
 
 
    glGenBuffersARB(1, &riverib[a]);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, riverib[a]);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*m, ind, GL_STATIC_DRAW_ARB);
    riversizes[a] = m;
    
    if(SAVE){
      fwrite(&n, sizeof(int), 1, mapData);
      fwrite(&m, sizeof(int), 1, mapData);
    
      fwrite(buf, sizeof(Vertex)*n, 1, mapData);
      fwrite(ind, sizeof(unsigned int)*m, 1, mapData);
    }
  }
  
  
  if(SAVE) fclose(mapData);
  
  
  
}
