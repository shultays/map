#include "Model.h"

#include <stdio.h>
#include <stdlib.h>

int loadModel(char *file, struct Model *model){
  void *iBuffer, *vBuffer;
  
  FILE *b = fopen(file, "rb");
  
  fread(&(model->subsetCount), 1, sizeof(int), b);
  model->sets = (struct Subset*)malloc(sizeof(struct Subset)*model->subsetCount);
  

    
  fread(&(model->vSize), 1, sizeof(int), b);
  vBuffer=malloc(sizeof(float)*6*model->vSize);
  fread(vBuffer, sizeof(float)*6, model->vSize, b);
  
  fread(&(model->iSize), 1, sizeof(int), b);
  iBuffer=malloc(sizeof(short)*3*model->iSize);
  int k = fread(iBuffer, 1,sizeof(short)*3*model->iSize, b);
  
  
  glGenBuffersARB(1, &model->iBuffer);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, model->iBuffer);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
  sizeof(short)*3*model->iSize, iBuffer, GL_STATIC_DRAW_ARB);
  
  glGenBuffersARB(1, &model->vBuffer);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, model->vBuffer); 
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
  sizeof(float)*6*model->vSize, vBuffer, GL_STATIC_DRAW_ARB);
 
 
  
  
  for(int i=0; i<model->subsetCount; i++){
      
    fread(&(model->sets[i].color), 1, sizeof(int), b);
    fread(&(model->sets[i].iBegin), 1, sizeof(int), b);
    fread(&(model->sets[i].iCount), 1, sizeof(int), b);
    fread(&(model->sets[i].vBegin), 1, sizeof(int), b);
    fread(&(model->sets[i].vCount), 1, sizeof(int), b);
    
    printf("0x%x\n", model->sets[i].color);
  }
  
  fclose(b);  
  free(vBuffer);
  free(iBuffer);
  printf("%d %d\n", model->vBuffer, model->iBuffer);
}

void drawModel(struct Model *model){
  glColor3f(1.0, 0.0, 0.0);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, model->vBuffer);
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
  glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
  
     
  glEnableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(float)*6, BUFFER_OFFSET(0));
  glNormalPointer(GL_FLOAT, sizeof(float)*6, BUFFER_OFFSET(12));
     
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, model->iBuffer);
  
  glDrawElements(GL_TRIANGLES,  model->iSize*3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  
  glEnableClientState(GL_COLOR_ARRAY);
  /*
  for(int i=0; i<model->subsetCount; i++){
    
  int iBegin;
  int iCount;
  int vBegin;
  int vCount;
  
  glDrawElements(GL_TRIANGLES, 
  model, 
  GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    
    
     glDrawRangeElements(GL_TRIANGLES, 
     model->set[i].iBegin,
     model->set[i].iCount, model->set[i].vCount, GL_UNSIGNED_SHORT, index);
  }
  */
}

