#ifndef MODEL_H
#define MODEL_H

#include "Tools.h"

struct Subset{
  int color;
  int iBegin;
  int iCount;
  int vBegin;
  int vCount;
};

struct Model{
  int subsetCount;
  struct Subset *sets;
  int iSize;
  GLuint iBuffer;
  int vSize;
  GLuint vBuffer; 
};

int loadModel(char *file, struct Model *model);
void drawModel(struct Model *model);


#endif
