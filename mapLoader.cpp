#include "mapLoader.h"


#include "Tools.h"

void loadMap(char *name){
  
  FILE *mapData = fopen(name, "rb");
  int a, b;
  
  fread(randSeed, sizeof(randSeed), 1, mapData);
  fread(buf, sizeof(Vertex)*(MAPSIZE-2)*(MAPSIZE-2)*5, 1, mapData);
  fread(ind, sizeof(unsigned int)*MAPSIZE*MAPSIZE*12, 1, mapData);
  
  
    
  unsigned int *partInd = (unsigned int *)malloc(sizeof(unsigned int)*(MAPSIZE/partialTerrainCount)*(MAPSIZE/partialTerrainCount)*12);
  //fprintf(fout, "%d\n", (1024/partialTerrainCount)*(1024/partialTerrainCount)*12);
  for(int i=0; i<partialTerrainCount; i++){
    for(int j=0; j<partialTerrainCount; j++){
      
      fread(&partIBSizes[i][j], sizeof(unsigned int), 1, mapData);
      //fprintf(fout, "%d %d %d\n", i, j, partIBSizes[i][j]);
      fread(partInd, sizeof(unsigned int)*partIBSizes[i][j], 1, mapData);
      
      
      glGenBuffersARB(1, &partIB[i][j]);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, partIB[i][j]);
      glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*partIBSizes[i][j], partInd, GL_STATIC_DRAW_ARB);
      //fprintf(fout, "%d %d %d %d %d\n", i, j,  partIBSizes[i][j], partIB[i][j], partInd[0]);
    }  
  }
  
  free(partInd);
  
  fprintf(fout,"testo\n");
  
  glGenBuffersARB(1, &vb);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb); 
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertex)*(MAPSIZE-2)*(MAPSIZE-2)*5, buf, GL_STATIC_DRAW_ARB);
 
   glGenBuffersARB(1, &ib);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ib);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*MAPSIZE*MAPSIZE*12, ind, GL_STATIC_DRAW_ARB);
      
 
  memcpy(bufBig, buf, sizeof(Vertex)*(MAPSIZE-2)*(MAPSIZE-2)*5);
  
  
  
  fread(map.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
  
  fread(earth.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
  fread(dirt.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
  
  fread(temp.data, sizeof(int)*(MAPSIZE+2)*(MAPSIZE+2), 1, mapData);
  fread(norms.data, sizeof(vec3)*MAPSIZE*MAPSIZE, 1, mapData);
  
  fread(&mw, sizeof(int), 1, mapData);
  
    
  for(int a=0; a<mw; a++){
    int k, m;
      
    fread(&k, sizeof(int), 1, mapData);
    fread(&m, sizeof(int), 1, mapData);
  
  
    fread(buf, sizeof(Vertex)*k, 1, mapData);
    fread(ind, sizeof(unsigned int)*m, 1, mapData);
  
  
  
    glGenBuffersARB(1, &wvb[a]);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, wvb[a]); 
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertex)*k, buf, GL_STATIC_DRAW_ARB);
 
 
    glGenBuffersARB(1, &wib[a]);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, wib[a]);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*m, ind, GL_STATIC_DRAW_ARB);
    wsizes[a] = m;
 
 
  }
  
  
  fread(&riverCount, sizeof(int), 1, mapData);
  fread(sup2.data, sizeof(char)*MAPSIZE*MAPSIZE, 1, mapData);
  fread(sup.data, sizeof(float)*MAPSIZE*MAPSIZE, 1, mapData);
  
  fprintf(fout, "river count%d\n", riverCount);
  fflush(stdout);
  for(int a=0; a<riverCount; a++){
    int k, m;
    
    fread(&k, sizeof(int), 1, mapData);
    int arr[2];
    
    fprintf(fout, " r%d size: %d\n", a, k);
    fflush(stdout);
    while(k--){
      fread((void*)arr, sizeof(int)*3, 1, mapData);
      
      fflush(stdout);
      rivers[a].push_back( (int3){arr[0], arr[1], arr[2]});
    }
    
    
    fread(&k, sizeof(int), 1, mapData);
    fread(&m, sizeof(int), 1, mapData);
  
  
    fread(buf, sizeof(Vertex)*k, 1, mapData);
    fread(ind, sizeof(unsigned int)*m, 1, mapData);
  
  
    fflush(fout);
    glGenBuffersARB(1, &rivervb[a]);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, rivervb[a]); 
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vertex)*k, buf, GL_STATIC_DRAW_ARB);
 
 
    glGenBuffersARB(1, &riverib[a]);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, riverib[a]);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*m, ind, GL_STATIC_DRAW_ARB);
    riversizes[a] = m;
  }
  
  fclose(mapData);
  
  for(int i=0; i<MAPSIZE-1; i++){
    for(int j=0; j<MAPSIZE-1; j++){
      height[i][j] = (map[i][j] + map[i][j+1] + map[i+1][j] + map[i+1][j+1])/4;
    }
  }  
}
