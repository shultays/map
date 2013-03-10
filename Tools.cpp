#include "Tools.h"

GL_Window*	g_window;
Keys*		g_keys;
int LOAD;
int SAVE;


Array2D<float> height(MAPSIZE, MAPSIZE);

Vertex *buf;
Vertex *bufBig;
unsigned int *ind;


int riverCount = 0;
vector<int3> rivers[max_river];
int riversizes[max_river];

int dir[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}};

PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = NULL;


PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv = NULL;

PFNGLGETSHADERIVPROC glGetShaderiv = NULL;

PFNGLGETINFOLOGARBPROC glGetInfoLogARB = NULL;

PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = NULL;

PFNGLATTACHOBJECTARBPROC glAttachObjectARB = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = NULL;

PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;

PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM2FPROC glUniform2f = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORM4FPROC glUniform4f = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLUNIFORM2IPROC glUniform2i = NULL;
PFNGLUNIFORM3IPROC glUniform3i = NULL;
PFNGLUNIFORM4IPROC glUniform4i = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
PFNGLUNIFORM3IVPROC glUniform3iv = NULL;
PFNGLUNIFORM4IVPROC glUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;

FILE *fout;


GLuint vb, ib;

GLuint partIB[partialTerrainCount][partialTerrainCount];
int partIBSizes[partialTerrainCount][partialTerrainCount];


GLuint wvb[max_river], wib[max_river];
GLuint rivervb[max_river], riverib[max_river];

float mx=0, my=0, mz=-100;

float ang = 0;
float vertAng = 90;
float car = 20;



Array2D<float> map(MAPSIZE, MAPSIZE);
Array2D<int> temp(MAPSIZE+2, MAPSIZE+2);

Array2D<float> dirt(MAPSIZE, MAPSIZE);
Array2D<float> earth(MAPSIZE, MAPSIZE);

Array2D<vec3> norms(MAPSIZE,MAPSIZE);
  
int wsizes[1000];


int water[1000];
int waterB[1000][4];
int mw=0;

Array2D<char> sup2(MAPSIZE, MAPSIZE);
Array2D<float> sup(MAPSIZE, MAPSIZE);

int sopt=0;
vec4 opts[10]; 


unsigned int vec4touint(vec4 c){
   unsigned int r = (unsigned int)(c[0]*255);
   unsigned int g = (unsigned int)(c[1]*255);
   unsigned int b = (unsigned int)(c[2]*255);
   unsigned int a = (unsigned int)(c[3]*255);
  
   return a<<24 | b<<16 | g<<8 | r;
}

vec4 uinttovec4(unsigned int c){
  vec4 r;
  r[0] = (c&255)/255.0f;
  r[1] = ((c>>8)&255)/255.0f;
  r[2] = ((c>>16)&255)/255.0f;
  r[3] = ((c>>24)&255)/255.0f;
  
  return r;
}

GLuint detailedMeshVB;
GLuint detailedMeshIB;

int2 detailedMeshIDs[(gridPerPart*6)*(gridPerPart*6)];
int detailedMeshSize = 0;

int2 oldDetailedPos = {-1, -1};


int mouseNewX=0;
int mouseNewY=0;

int mouseDiffX=0;
int mouseDiffY=0;

short randSeed[MAPSIZE][MAPSIZE];

float detailedHeights[(gridPerPart*6)*(gridPerPart*6)][detailedMeshDetail+1][detailedMeshDetail+1];

int onDetailedID = -1;

unsigned int lcg_rand(unsigned int a){
    return ((unsigned long long)a * 279470273UL) % 4294967291UL;
}

float cameraHeight = 200;


using namespace std;
int getFileLength(FILE *f){
    fseek(f, 0L, SEEK_END);
    int len = ftell(f);
    
    fseek(f, 0L, SEEK_SET);

    return len;
}

int loadfile(char* filename, char** ShaderSource, int* len)
{
  FILE *shaderfile = fopen(filename, "rb");
  
  *len = getFileLength(shaderfile);
  
  if (*len==0) return -2;
  
  *ShaderSource = (char*)malloc(sizeof(char)*((*len)+1));
  if (*ShaderSource == 0) return -3;
  
  
  fread(*ShaderSource, 1, *len, shaderfile);
  (*ShaderSource)[*len] = '\0';
  
  fclose(shaderfile);
    
  return 0;
}


int unloadfile(GLubyte** ShaderSource){
  free(*ShaderSource);
}


int loadVertexShader(char *filename){
  int vertexShaderObject;  
  vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
  
  int len;
  char *source;
  loadfile(filename, &source, &len);
  
  glShaderSourceARB(vertexShaderObject, 1, (const GLcharARB**)&source, &len);
  glCompileShaderARB(vertexShaderObject);
  
  GLint blen = 0;	  
  GLsizei slen = 0;
  glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH , &blen);       
  if (blen > 1)
  {
    GLchar* compiler_log = (GLchar*)malloc(blen);
    glGetInfoLogARB(vertexShaderObject, blen, &slen, compiler_log);
    fprintf(fout, "%s\n",compiler_log);
    free (compiler_log);
  }
  /*
  GLint compiled;
  
  glGetObjectParameteriv(vertexShaderObject, GL_COMPILE_STATUS, &compiled);
  if (compiled){
    printf("error! %s is not compiled\n", filename);
  }   
  */
  return vertexShaderObject;     
}

int loadPixelShader(char *filename){
  int fragmentShaderObject;

  fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  
  int len;
  char *source;
  loadfile(filename, &source, &len);
  
  glShaderSourceARB(fragmentShaderObject, 1, (const GLcharARB**)&source, &len);
  glCompileShaderARB(fragmentShaderObject);

  GLint blen = 0;	  
  GLsizei slen = 0;
  glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH , &blen);       
  if (blen > 1)
  {
    GLchar* compiler_log = (GLchar*)malloc(blen);
    glGetInfoLogARB(fragmentShaderObject, blen, &slen, compiler_log);
    fprintf(fout, "%s\n",compiler_log);
    free (compiler_log);
  }
  return fragmentShaderObject;
}
shader loadShader(char *filenameVertex, char *filenamePixel){
  shader s;
  s.vertexShader=loadVertexShader(filenameVertex);
  s.fragmentShader=loadPixelShader(filenamePixel);
  return s;  
}





