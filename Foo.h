#ifndef GL_FRAMEWORK__INCLUDED
#define GL_FRAMEWORK__INCLUDED


#include <windows.h>

#include "Perlin.h"
#include "water.h"

void TerminateApplication (GL_Window* window);

void ToggleFullscreen (GL_Window* window);

BOOL Initialize (GL_Window* window, Keys* keys);

void Deinitialize (void);

void Update (DWORD milliseconds);

void Draw (void);	


#define print fprintf(fout,

#endif
