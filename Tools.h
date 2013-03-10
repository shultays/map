#ifndef TOOLS_H
#define TOOLS_H

#include <windows.h>												
#include <gl/gl.h>												
#include <gl/glu.h>														
#include <gl/glext.h>														
#include <stdio.h>		


			
#include <glm/glm.hpp>	

#include <vector>
#include "Array2D.h"

#define PI 3.14159265
#define RadToPI PI/180.0
#define swapt(x, y, type) do{type t; t=x; x=y; y=t;}while(0) 

#define MAPSIZE 512

#define minv(x,y) ((x)<(y)?(x):(y))
#define maxv(x,y) ((x)>(y)?(x):(y))

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define max_river 10

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB  0x88E4
#define GL_DYNAMIC_DRAW_ARB 0x88E8


#define partialTerrainCount 256

#define gridPerPart MAPSIZE/partialTerrainCount

#define detailedMeshDetail 16

#define debug(a,b) fprintf(fout, #a " = " #b " \n", a)

using namespace glm;
using namespace std;

typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * PFNGLBUFFERSUBDATAARBPROC) (GLenum  target,  int  offset,  int  size,  const GLvoid *  data);






struct Vertex{
vec3 pos;
vec3 nor;
unsigned int color;
};


typedef struct {
  int x;
  int y;
}int2;

typedef struct {
  int x;
  int y;
  int z;
}int3;

typedef struct {
	HINSTANCE		hInstance;
	const char*		className;
} Application;

typedef struct {
	Application*		application;
	char*				title;
	int					width;
	int					height;
	int					bitsPerPixel;
	BOOL				isFullScreen;	
} GL_WindowInit;

typedef struct {
	BOOL keyDown [256];
} Keys;

typedef struct {
	Keys*				keys;
	HWND				hWnd;
	HDC					hDC;
	HGLRC				hRC;
	GL_WindowInit		init;
	BOOL				isVisible;
	DWORD				lastTickCount;
} GL_Window;

extern GL_Window*	g_window;
extern Keys*		g_keys;

extern int LOAD;
extern int SAVE;

extern Array2D<float> height;

  
extern GLuint partIB[partialTerrainCount][partialTerrainCount];
extern int partIBSizes[partialTerrainCount][partialTerrainCount];

extern Vertex *buf;
extern Vertex *bufBig;
extern unsigned int *ind;


extern int riverCount;
extern vector<int3> rivers[max_river];
extern int riversizes[max_river];

extern int dir[4][2];




extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv;


extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;

extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;

extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM2IPROC glUniform2i;
extern PFNGLUNIFORM3IPROC glUniform3i;
extern PFNGLUNIFORM4IPROC glUniform4i;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORM1IVPROC glUniform1iv;
extern PFNGLUNIFORM2IVPROC glUniform2iv;
extern PFNGLUNIFORM3IVPROC glUniform3iv;
extern PFNGLUNIFORM4IVPROC glUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;






extern FILE *fout;



extern GLuint vb, ib;
extern GLuint wvb[max_river], wib[max_river];


extern GLuint rivervb[max_river], riverib[max_river];



extern float mx, my, mz;

extern float ang;
extern float vertAng;
extern float car;

extern Array2D<float> map;
extern Array2D<int> temp;

extern Array2D<float> dirt;
extern Array2D<float> earth;

extern Array2D<vec3> norms;
  
extern int wsizes[1000];


extern int water[1000];
extern int waterB[1000][4];
extern int mw;

extern Array2D<char> sup2;
extern Array2D<float> sup;

extern int sopt;
extern vec4 opts[10]; 


extern GLuint detailedMeshVB;
extern GLuint detailedMeshIB;
extern int2 detailedMeshIDs[(gridPerPart*6)*(gridPerPart*6)];
extern int detailedMeshSize;

extern int2 oldDetailedPos;
unsigned int vec4touint(vec4 c);
//extern int detailedMeshK[76
extern int mouseNewX;
extern int mouseNewY;

extern int mouseDiffX;
extern int mouseDiffY;
vec4 uinttovec4(unsigned int c);

extern float detailedHeights[(gridPerPart*6)*(gridPerPart*6)][detailedMeshDetail+1][detailedMeshDetail+1];

extern int onDetailedID;

extern float cameraHeight;


//extern short seeds[1024][1024][2][10];

vec2 cross(vec2 &A, vec2 &B);
extern short randSeed[MAPSIZE][MAPSIZE];
unsigned int lcg_rand(unsigned int a);


int getFileLength(ifstream& file);
int loadshader(char* filename, char** ShaderSource, int* len);
int unloadshader(GLubyte** ShaderSource);


struct shader{
  GLuint vertexShader, fragmentShader;
  
};

int loadVertexShader(char *filename);
int loadPixelShader(char *filename);
shader loadShader(char *filenameVertex, char *filenamePixel);

#endif
