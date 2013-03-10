#include <windows.h>										
#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>	
#include <gl\glext.h>									
#include <glm\glm.hpp>
#include <stdio.h>												
#include <time.h>	
											
#include "Foo.h"										

#ifndef CDS_FULLSCREEN
#define CDS_FULLSCREEN 4
#endif									

#include "mapGenerator.h"
#include "mapLoader.h"

#include "Model.h"
Model tree;

int camera = 1;
int refreshSubWorld = 1;
int seed = -1;

shader s;
GLhandleARB p;

int tri = 0;




float getHeight(float mx, float my){
  
  int a = (int)((mx+1+(MAPSIZE-2))/2);
  int b = (int)((my+1+(MAPSIZE-2))/2);
  
  float x = ((mx+1+(MAPSIZE-2))/2)-a;
  float y = ((my+1+(MAPSIZE-2))/2)-b;
  
  a=a<0?0:a;
  a=a>(MAPSIZE-1)?(MAPSIZE-1):a;
  b=b<0?0:b;
  b=b>(MAPSIZE-1)?(MAPSIZE-1):b;
  
  
  
  if(x>y && x < 1-y) tri = 0;
  if(x<y && x < 1-y) tri = 1;
  if(x>y && x > 1-y) tri = 2;
  if(x<y && x > 1-y) tri = 3;
  
  buf[0] = bufBig[(a*(MAPSIZE-2)+b)*5+0];
  buf[1] = bufBig[(a*(MAPSIZE-2)+b)*5+1];
  buf[2] = bufBig[(a*(MAPSIZE-2)+b)*5+2];
  buf[3] = bufBig[(a*(MAPSIZE-2)+b)*5+3];
  buf[4] = bufBig[(a*(MAPSIZE-2)+b)*5+4];
    
  vec3 v1, v2;
  if(tri == 0){
    v1 = buf[0].pos - buf[4].pos;
    v2 = buf[1].pos - buf[4].pos;
  }else  if(tri == 1){
    v1 = buf[3].pos - buf[4].pos;
    v2 = buf[0].pos - buf[4].pos;
  }else   if(tri == 2){
    v1 = buf[1].pos - buf[4].pos;
    v2 = buf[2].pos - buf[4].pos;
  }else   if(tri == 3){
    v1 = buf[2].pos - buf[4].pos;
    v2 = buf[3].pos - buf[4].pos;
  }
  
  vec3 c = cross(v1, v2);
  
  float h = (c[0]*(mx-buf[4].pos[0]) + c[1]*(my-buf[4].pos[1]))/-c[2]+buf[4].pos[2];
  
  return h;
}


float getHeight2(float mx, float my){
  
  int a1 = (int)((mx+1+(MAPSIZE-2))/2);
  int b1 = (int)((my+1+(MAPSIZE-2))/2);
  
  float x = ((mx+1+(MAPSIZE-2))/2)-a1;
  float y = ((my+1+(MAPSIZE-2))/2)-b1;
  
  int a = (int)(x*detailedMeshDetail);
  int b = (int)(y*detailedMeshDetail);
  
  
  float xBegin = (a1*2-1-(MAPSIZE-2)); 
  float yBegin = (b1*2-1-(MAPSIZE-2)); 
  
  x = (x*detailedMeshDetail-a)/detailedMeshDetail;
  y = (y*detailedMeshDetail-b)/detailedMeshDetail;
  
  
  
  if(x>y){
     if(x < 1-y) tri = 0;
     else tri = 2;
  }else{
    if(x < 1-y) tri = 1;
    else  tri = 3;
  }
        
  


  buf[0].pos = vec3(xBegin+((a)*2.0)/detailedMeshDetail,   yBegin+((b)*2.0)/detailedMeshDetail,   detailedHeights[onDetailedID][a][b]);
  buf[1].pos = vec3(xBegin+((a+1)*2.0)/detailedMeshDetail, yBegin+((b)*2.0)/detailedMeshDetail,   detailedHeights[onDetailedID][a+1][b]);
  buf[2].pos = vec3(xBegin+((a+1)*2.0)/detailedMeshDetail, yBegin+((b+1)*2.0)/detailedMeshDetail, detailedHeights[onDetailedID][a+1][b+1]);
  buf[3].pos = vec3(xBegin+((a)*2.0)/detailedMeshDetail,   yBegin+((b+1)*2.0)/detailedMeshDetail, detailedHeights[onDetailedID][a][b+1]);
  buf[4].pos = (buf[0].pos+buf[1].pos+buf[2].pos+buf[3].pos)/4.0f;
  
  vec3 v1, v2;
  if(tri == 0){
    v1 = buf[0].pos - buf[4].pos;
    v2 = buf[1].pos - buf[4].pos;
  }else  if(tri == 1){
    v1 = buf[3].pos - buf[4].pos;
    v2 = buf[0].pos - buf[4].pos;
  }else   if(tri == 2){
    v1 = buf[1].pos - buf[4].pos;
    v2 = buf[2].pos - buf[4].pos;
  }else   if(tri == 3){
    v1 = buf[2].pos - buf[4].pos;
    v2 = buf[3].pos - buf[4].pos;
  }
  
  vec3 c = cross(v1, v2);
  
  
  float h = (c[0]*(mx-buf[4].pos[0]) + c[1]*(my-buf[4].pos[1]))/-c[2]+buf[4].pos[2];
  fprintf(fout, "%d %d %f %f\n",a, b,  h/1000, getHeight(mx, my));
  return h;
}

int initedOnce = 0;
int initOnce(){
  if(!initedOnce){
      
    initedOnce = 1;
      
    fout = fopen("d.txt", "w");
    setvbuf(fout, NULL, _IONBF, 0);
    glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
    glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
    glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
    glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC) wglGetProcAddress("glBufferSubDataARB");
    
    glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");
    glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) wglGetProcAddress("glShaderSourceARB");
    glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) wglGetProcAddress("glCompileShaderARB");
    glGetObjectParameteriv = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameteriv");

  
    glGetShaderiv  = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetShaderiv");
    glGetInfoLogARB  = (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");

    glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");

    glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) wglGetProcAddress("glAttachObjectARB");
    glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) wglGetProcAddress("glLinkProgramARB");
    glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) wglGetProcAddress("glUseProgramObjectARB");

    glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
    glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");

    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");
    
    glUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress("glUniform1f");
    glUniform2f = (PFNGLUNIFORM2FPROC) wglGetProcAddress("glUniform2f");
    glUniform3f = (PFNGLUNIFORM3FPROC) wglGetProcAddress("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC) wglGetProcAddress("glUniform4f");
    glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
    glUniform2i = (PFNGLUNIFORM2IPROC) wglGetProcAddress("glUniform2i");
    glUniform3i = (PFNGLUNIFORM3IPROC) wglGetProcAddress("glUniform3i");
    glUniform4i = (PFNGLUNIFORM4IPROC) wglGetProcAddress("glUniform4i");
    glUniform1fv = (PFNGLUNIFORM1FVPROC) wglGetProcAddress("glUniform1fv");
    glUniform2fv = (PFNGLUNIFORM2FVPROC) wglGetProcAddress("glUniform2fv");
    glUniform3fv = (PFNGLUNIFORM3FVPROC) wglGetProcAddress("glUniform3fv");
    glUniform4fv = (PFNGLUNIFORM4FVPROC) wglGetProcAddress("glUniform4fv");
    glUniform1iv = (PFNGLUNIFORM1IVPROC) wglGetProcAddress("glUniform1iv");
    glUniform2iv = (PFNGLUNIFORM2IVPROC) wglGetProcAddress("glUniform2iv");
    glUniform3iv = (PFNGLUNIFORM3IVPROC) wglGetProcAddress("glUniform3iv");
    glUniform4iv = (PFNGLUNIFORM4IVPROC) wglGetProcAddress("glUniform4iv");

    glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) wglGetProcAddress("glUniformMatrix2fv");
    glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) wglGetProcAddress("glUniformMatrix3fv");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");


    buf    = (Vertex*)malloc(sizeof(Vertex)*MAPSIZE*MAPSIZE*5);
    bufBig = (Vertex*)malloc(sizeof(Vertex)*MAPSIZE*MAPSIZE*5);
    ind =  (unsigned int*)malloc(sizeof(unsigned int)*MAPSIZE*MAPSIZE*12);
    
    if(seed==-1) seed = time(0);
    
    opts[0] = vec4(0.6, 0.6, 0.5, 1);
    opts[1] = vec4(0.5, 0.4, 0.3, 1);
    opts[2] = vec4(0, 0.1, 0.4, 0.8); 
    
    srand(seed);  
    
	
    glGenBuffersARB(1, &detailedMeshVB);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, detailedMeshVB);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(struct Vertex)*(gridPerPart*6)*(gridPerPart*6)*5*detailedMeshDetail*detailedMeshDetail, NULL, GL_DYNAMIC_DRAW_ARB);
    
    glGenBuffersARB(1, &detailedMeshIB);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, detailedMeshIB);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int)*(gridPerPart*6)*(gridPerPart*6)*12*detailedMeshDetail*detailedMeshDetail, NULL, GL_DYNAMIC_DRAW_ARB);
      
    s = loadShader("vertex_shader.txt", "pixel_shader.txt");


  	p = glCreateProgramObjectARB();
  	
  	glAttachObjectARB(p,s.fragmentShader);
  	glAttachObjectARB(p,s.vertexShader);
  
  	glLinkProgramARB(p);
  	
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetObjectParameterivARB(p, GL_OBJECT_INFO_LOG_LENGTH_ARB,&infologLength);
    
    if (infologLength > 0)
    {
      infoLog = (char *)malloc(infologLength);
      glGetInfoLogARB(p, infologLength, &charsWritten, infoLog);
      fprintf(fout, "%s\n",infoLog);
      free(infoLog);
    }
  	glUseProgramObjectARB(p);
  	
  	loadModel("out.b", &tree);
  }
}



BOOL Initialize (GL_Window* window, Keys* keys)
{
  int i, j;
  initOnce();
  fprintf(fout, "initting\n");
	g_window	= window;
	g_keys		= keys;
	

  
  if(LOAD) loadMap("map.dat");
  else generateMap();
  
  
  
  const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
  const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
  const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

  glClearColor(0,0,0,1);
    
     
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
    
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA) ;
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);


  glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 
 
  fprintf(fout, "initted\n");

	return TRUE;
}

void Deinitialize (void)
{
  fprintf(fout, "deinitting\n");
  glDeleteBuffersARB(1, &ib);
  
  glDeleteBuffersARB(1, &vb);
  glDeleteBuffersARB(mw, wvb);
  glDeleteBuffersARB(mw, wib);
  
  
  
  glDeleteBuffersARB(riverCount, rivervb);
  glDeleteBuffersARB(riverCount, riverib);
  

  for(int i=0; i<partialTerrainCount; i++){
    for(int j=0; j<partialTerrainCount; j++){
      glDeleteBuffersARB(1, &partIB[i][j]);
    }
  }
  fprintf(fout, "deinitted\n");

}

void Update (DWORD milliseconds){
  int i;
  
  
  if(mouseDiffX != 0){
    ang += mouseDiffX*(milliseconds/40.0);
    debug(ang, %f);
    mouseDiffX = 0;
  }
  if(mouseDiffY != 0){
    
    vertAng += mouseDiffY*(milliseconds/40.0);
    
    if(vertAng < 0) vertAng = 0;
    if(vertAng > 179) vertAng = 179;
    mouseDiffY = 0;
  }
  if(g_keys->keyDown['P']){
    for(i=0; i<3; i++){
      fprintf(fout, "%d %f, %f, %f, %f\n", i, opts[i][0], opts[i][1], opts[i][2], opts[i][3]);
    }
    fprintf(fout, "\n");
  }
  
  for(i=0; i<10; i++){
    if(g_keys->keyDown ['0'+i]) sopt = i;    
  }
  
  if(g_keys->keyDown ['R']) cameraHeight = 1;
  if(g_keys->keyDown ['T']) cameraHeight = 10;
  if(g_keys->keyDown ['Y']) cameraHeight = 100;
  if(g_keys->keyDown ['U']) cameraHeight = 3000;


  if (g_keys->keyDown [VK_F5] == TRUE){
    FILE *cam = fopen("cam.txt", "wb");
    fwrite (&mx , 1, sizeof(mx), cam );
    fwrite (&my , 1, sizeof(my), cam );
    fwrite (&ang , 1, sizeof(ang), cam );
    fwrite (&vertAng , 1, sizeof(vertAng), cam );
    fclose(cam);
  }
  if (g_keys->keyDown [VK_F6] == TRUE){
    FILE *cam = fopen("cam.txt", "rb");
    fread (&mx , 1, sizeof(mx), cam );
    fread (&my , 1, sizeof(my), cam );
    fread (&ang , 1, sizeof(ang), cam );
    fread (&vertAng , 1, sizeof(vertAng), cam );
    fclose(cam);
  }
	if (g_keys->keyDown [VK_ESCAPE] == TRUE){
		TerminateApplication (g_window);
	}

	float speed = 1;
	if(camera == 2) speed = 0.1;
	if(camera == 3) speed = 0.0005;
	if (g_keys->keyDown['3']) speed *=10;
	
	if (g_keys->keyDown ['W'] || g_keys->keyDown['3'])						
	{
    mx += sin(ang*PI/180)*milliseconds*speed/5.0f;
    my += cos(ang*PI/180)*milliseconds*speed/5.0f;
	}else 	if (g_keys->keyDown ['S'] == TRUE)						
	{
    mx -= sin(ang*PI/180)*milliseconds*speed/5.0f;
    my -= cos(ang*PI/180)*milliseconds*speed/5.0f;
	}
  if (g_keys->keyDown ['D'] == TRUE)						
	{
    mx += sin((ang+90)*PI/180)*milliseconds*speed/5.0f;
    my += cos((ang+90)*PI/180)*milliseconds*speed/5.0f;
	}else 	if (g_keys->keyDown ['A'] == TRUE)						
	{
    mx += sin((ang-90)*PI/180)*milliseconds*speed/5.0f;
    my += cos((ang-90)*PI/180)*milliseconds*speed/5.0f;
	}
	if(g_keys->keyDown ['Q'] == TRUE) ang += milliseconds/10.0;
	if(g_keys->keyDown ['E'] == TRUE) ang -= milliseconds/10.0;
	
  if (g_keys->keyDown ['R'] == TRUE)						
	{
    car -= milliseconds/100.0f;
	}else 	if (g_keys->keyDown ['F'] == TRUE)						
	{
    car += milliseconds/100.0f;
	}
	if (g_keys->keyDown ['N'] == TRUE){
    SAVE = 1;
    LOAD = 0;
    camera = 1;
    
    seed=-1;
    Deinitialize();
    Initialize (g_window, g_keys);
  }
	
	if(car<1) car = 1;
	if(car>40) car = 40;
	
	if(g_keys->keyDown['C']){
    camera = 1; 
  }
  if(g_keys->keyDown['V']){
    refreshSubWorld = 1;
    detailedMeshSize = 0;
    camera = 2; 
    vertAng = 90;
  }
  if(g_keys->keyDown['B']){
    refreshSubWorld = 1;
    detailedMeshSize = 0;
    camera = 3; 
    vertAng = 90;
  }

}

#define mult 10000.0f

Array2D<float> submap(detailedMeshDetail*3, detailedMeshDetail*3);

void generateSubWorld(int x, int y, int t, int val){
  if(x<0||y<0||x>(MAPSIZE-1)||y>(MAPSIZE-1))return;
  int k = detailedMeshSize;
  
  #define iftest if(x==256&&y==253)
  if(k==(gridPerPart*6)*(gridPerPart*6)){
    if(t==0){
      for(int i=0; i<detailedMeshSize; i++){
        if(detailedMeshIDs[i].x == val){
          k = i;
          break;
        }
      }
    }else if(t==1){
      for(int i=0; i<detailedMeshSize; i++){
        if(detailedMeshIDs[i].y == val){
          k = i;
          break;
        }
      }
    }
  }else{
    detailedMeshSize++;    
  }
  if(k==(gridPerPart*6)*(gridPerPart*6)){
     fprintf(fout, "err\n");
     return;
  }
  detailedMeshIDs[k].x = x;
  detailedMeshIDs[k].y = y;
  
  iftest printf("%s %d\n", __FUNCTION__, __LINE__);
  
  
  int jump = k*5*detailedMeshDetail*detailedMeshDetail;
  int n = 0, m = 0;
  
  
  Vertex *vertices[5];
  
  
  
  float xBegin = (x*2-1-(MAPSIZE-2)); 
  float yBegin = (y*2-1-(MAPSIZE-2)); 
  
  
  iftest printf("%s %d\n", __FUNCTION__, __LINE__);
  vec3 coors[detailedMeshDetail+3][detailedMeshDetail+3];
  vec3 normals[detailedMeshDetail+3][detailedMeshDetail+3];
  vec4 color[detailedMeshDetail+3][detailedMeshDetail+3];
  
  iftest printf("%s %d\n", __FUNCTION__, __LINE__);
  memset(normals, 0, sizeof(normals));
  
  iftest printf("%s %d\n", __FUNCTION__, __LINE__);
  submap.setAll(0.0f);
        
  
  lcg_addRandomMap(x, y, 6, 80.0f, submap);
  lcg_addRandomMap(x, y, 7, 30.0f, submap);
  lcg_addRandomMap(x, y, 8, 20.0f, submap);
  lcg_addRandomMap(x, y, 8, 10.0f, submap);
  lcg_addRandomMap(x, y, 9, 10.0f, submap);
  lcg_addRandomMap(x, y, 10, 5.0f, submap);
  lcg_addRandomMap(x, y, 11, 4.0f, submap);
  lcg_addRandomMap(x, y, 12, 2.0f, submap);
  lcg_addRandomMap(x, y, 13, 1.0f, submap);
  
  
  iftest printf("%s %d\n", __FUNCTION__, __LINE__);
  for(int i=0; i<detailedMeshDetail+3; i++){
    for(int j=0; j<detailedMeshDetail+3; j++){
      float mx, my;
      iftest if(i==1)printf("%s %d %d\n", __FUNCTION__, __LINE__, i);
      mx = xBegin + (i-1)*2.0f/(detailedMeshDetail);
      my = yBegin + (j-1)*2.0f/(detailedMeshDetail);
      
      int xx=x, yy=y;


      if(i==0) xx = x-1;
      else if(i>=detailedMeshDetail+2) xx = x+1;
      
      
      if(j==0) yy = y-1;
      else if(j>=detailedMeshDetail+2) yy = y+1;


      vertices[0] = &bufBig[(xx*(MAPSIZE-2)+yy)*5+0];
      vertices[1] = &bufBig[(xx*(MAPSIZE-2)+yy)*5+1];
      vertices[2] = &bufBig[(xx*(MAPSIZE-2)+yy)*5+2];
      vertices[3] = &bufBig[(xx*(MAPSIZE-2)+yy)*5+3];
      vertices[4] = &bufBig[(xx*(MAPSIZE-2)+yy)*5+4];
    
      iftest if(i==1)printf("%s %d %d\n", __FUNCTION__, __LINE__, i);
      unsigned int r=0;
      
      int rx1=x, rx2=i-1, ry1=y, ry2=j-1;
      if(i==0){
        rx1 = x-1;
        rx2 = i+detailedMeshDetail;
      }else if(i>=detailedMeshDetail+1){
        rx1 = x+1;
        rx2 = i-detailedMeshDetail-1;
      }
      if(j==0){
        ry1 = y-1;
        ry2 = j+detailedMeshDetail;
      }else if(j>=detailedMeshDetail+1){
        ry1 = y+1;
        ry2 = j-detailedMeshDetail-1;
      }

      iftest if(i==1)printf("%s %d %d\n", __FUNCTION__, __LINE__, i);
      int id0, id1, id2;
      id0 = 4;
      int  t;
      if(i>j){
        if(i < detailedMeshDetail+2-j) { t=0; id1 = 1; id2 = 0; }
        else { t=1;; id1 = 2; id2 = 1; }
      }else{
        if(i< detailedMeshDetail+2-j){  t=2; id1 = 3; id2 = 0; }
        else {t=3; id1 = 3; id2 = 2; }
      }
      
      iftest if(i==1)printf("%s %d %d\n", __FUNCTION__, __LINE__, i);

      vec3 A = vec3(vertices[id0]->pos.x, vertices[id0]->pos.y, 0);
      vec3 B = vec3(vertices[id1]->pos.x, vertices[id1]->pos.y, 0);
      vec3 C = vec3(vertices[id2]->pos.x, vertices[id2]->pos.y, 0);
      
      iftest if(i==1)printf("%s %d %d\n", __FUNCTION__, __LINE__, i);
      vec3 P  = vec3(mx, my, 0);
      vec3 N = cross(B-A,C-A);
      float AreaABC = dot(N,cross(B-A,C-A));
      float AreaPBC = dot(N,cross(B-P,C-P));
      float u = AreaPBC / AreaABC;
    
      float AreaPCA = dot(N,cross(C-P,A-P));
      float v = AreaPCA / AreaABC;
      
      float w = 1-u-v;
      
      iftest if(i==1)printf("%s %d %d\n", __FUNCTION__, __LINE__, i);
      
      coors[i][j] = vertices[id0]->pos*u + vertices[id1]->pos*v + vertices[id2]->pos*w;

      
      coors[i][j].z += submap[i+detailedMeshDetail][j+detailedMeshDetail]/600.0f;

      iftest if(i==1)printf("%s %d %d\n", __FUNCTION__, __LINE__, i);
      coors[i][j] *= mult;
      color[i][j] = uinttovec4(vertices[id0]->color)*u + uinttovec4(vertices[id1]->color)*v + uinttovec4(vertices[id2]->color)*w;

      if(i!=0 && j!= 0 && i<=detailedMeshDetail+1 && j<=detailedMeshDetail+1){
        detailedHeights[k][i-1][j-1] = coors[i][j].z;
      }
      
    }
  }
  
  n=m=0;
  for(int i=0; i<detailedMeshDetail+2; i++){
    for(int j=0; j<detailedMeshDetail+2; j++){
      
      float mx = xBegin + i*2.0f/(detailedMeshDetail);
      float my = yBegin + j*2.0f/(detailedMeshDetail);
      
      buf[n+0].pos = coors[i][j];
      buf[n+1].pos = coors[i+1][j];
      buf[n+2].pos = coors[i+1][j+1];
      buf[n+3].pos = coors[i][j+1];
      
      buf[n+4].pos = (buf[n+0].pos+buf[n+1].pos+buf[n+2].pos+buf[n+3].pos)/4.0f;
      
      
      buf[n+0].color = vec4touint(color[i][j]);
      buf[n+1].color = vec4touint(color[i+1][j]);
      buf[n+2].color = vec4touint(color[i+1][j+1]);
      buf[n+3].color = vec4touint(color[i][j+1]);


      buf[n+4].color = vec4touint((color[i][j]+color[i+1][j]+color[i][j+1]+color[i+1][j+1])/4.0f);
      
      //buf[n+0].color=buf[n+1].color=buf[n+2].color=buf[n+3].color=buf[n+4].color = 0xFF007700;
      
      vec3 cen =  vec3(mx+1, my+1,  buf[n+4].pos.z);
      
      vec3 n1=normalize(cross( 
      buf[n+0].pos - buf[n+4].pos, 
      buf[n+1].pos - buf[n+4].pos ));
      
      vec3 n2=normalize(cross( 
      buf[n+3].pos - buf[n+4].pos, 
      buf[n+0].pos - buf[n+4].pos ));
      
      vec3 n3=normalize(cross( 
      buf[n+1].pos - buf[n+4].pos, 
      buf[n+2].pos - buf[n+4].pos ));
      
      vec3 n4=normalize(cross( 
      buf[n+2].pos - buf[n+4].pos, 
      buf[n+3].pos - buf[n+4].pos ));
      
      
      buf[n+0].nor =  normalize(n1+n2);
      buf[n+1].nor =  normalize(n1+n3);
      buf[n+2].nor =  normalize(n3+n4);
      buf[n+3].nor =  normalize(n1+n4);
      
      buf[n+4].nor = normalize(buf[n+0].nor+buf[n+1].nor+buf[n+2].nor+buf[n+3].nor);
        
      ind[m+0] = jump+n+0;
      ind[m+1] = jump+n+1;
      ind[m+2] = jump+n+4;
        
      ind[m+3] = jump+n+3;
      ind[m+4] = jump+n+0;
      ind[m+5] = jump+n+4;
         
      ind[m+6] = jump+n+1;
      ind[m+7] = jump+n+2;
      ind[m+8] = jump+n+4;
       
      ind[m+9]  = jump+n+2;
      ind[m+10] = jump+n+3;
      ind[m+11] = jump+n+4;
      
      
      norms[i][j] += (n1+n2)/2.0f;
      if(i!=detailedMeshDetail+1)norms[i+1][j] += (n1+n3)/2.0f;
      if(i!=detailedMeshDetail+1&&j!=detailedMeshDetail+1)norms[i+1][j+1] += (n3+n4)/2.0f;
      if(j!=detailedMeshDetail+1)norms[i][j+1] += (n2+n4)/2.0f;
      
      if(i!=0 && j!=0 &&i!=detailedMeshDetail+1&&j!=detailedMeshDetail+1){
          
        n+=5;
        m+=12;
      }
    }
  }
  for(int i=0; i<detailedMeshDetail+2; i++){
    for(int j=0; j<detailedMeshDetail+2; j++){
      norms[i][j] = normalize(norms[i][j]);
    }
  }
  n=0;
  for(int i=1; i<detailedMeshDetail+1; i++){
    for(int j=1; j<detailedMeshDetail+1; j++){
      
        buf[n+0].nor =  norms[i][j];
        buf[n+1].nor =  norms[i+1][j];
        buf[n+2].nor =  norms[i+1][j+1];
        buf[n+3].nor =  norms[i][j+1];
        
        buf[n+4].nor =  (buf[n+0].nor+buf[n+1].nor+buf[n+2].nor+buf[n+3].nor)/4.0f;
        buf[n+4].nor = normalize(buf[n+4].nor);
        n+=5;
    }
  }
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, detailedMeshVB); 
  glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 
  
  sizeof(struct Vertex)*5*k*detailedMeshDetail*detailedMeshDetail, 
  
  sizeof(struct Vertex)*5*detailedMeshDetail*detailedMeshDetail, buf); 
  
  
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, detailedMeshIB); 
  glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
  
  sizeof(unsigned int)*12*k*detailedMeshDetail*detailedMeshDetail, 
  sizeof(unsigned int)*12*detailedMeshDetail*detailedMeshDetail, ind); 
  //glBufferSubDataARB();
}


void renderWorld(int partX, int partY){
  
  glColor3f(0.8,0.8,0.8);
  
  
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(24));

#if 1
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ib);
  glDrawElements(GL_TRIANGLES,MAPSIZE*MAPSIZE*12, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
     
#else 
  for(int i=0; i<partialTerrainCount; i++){
    for(int j=0; j<partialTerrainCount; j++){
            
      if(abs(i-partX) <= 1 && abs(j-partY) <= 1) continue;
      //fprintf(fout, "%d %d %d %d\n", i, j,  partIBSizes[i][j], partIB[i][j]);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, partIB[i][j]);
      glDrawElements(GL_TRIANGLES,partIBSizes[i][j], GL_UNSIGNED_INT, BUFFER_OFFSET(0));
      
    }  
  }
  #endif

  for(int i=0; i<mw; i++){
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, wvb[i]);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(24));
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, wib[i]);
    glDrawElements(GL_TRIANGLES, wsizes[i], GL_UNSIGNED_INT, BUFFER_OFFSET(0)); 
    
  }
  
  for(int i=0; i<riverCount; i++){
    
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, rivervb[i]);
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
    glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
 
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(24));
 
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, riverib[i]);
 
    glDrawElements(GL_TRIANGLES, riversizes[i], GL_UNSIGNED_INT, BUFFER_OFFSET(0)); 
  }
  
  
  if(camera == 1 || camera == 2){
    glColor3f(0.8,0.8,0.8);
  
    glBegin(GL_TRIANGLES);
  
    int a = (int)((mx+1+(MAPSIZE-2))/2);
    int b = (int)((my+1+(MAPSIZE-2))/2);
    
    a=a<0?0:a;
    a=a>(MAPSIZE-1)?(MAPSIZE-1):a;
    b=b<0?0:b;
    b=b>(MAPSIZE-1)?(MAPSIZE-1):b;
    glVertex3f(mx, my, map[a][b]+0);
    glVertex3f(mx+5, my-5, map[a][b]+100);
    glVertex3f(mx-5, my-5, map[a][b]+100);
    
    glVertex3f(mx, my, map[a][b]+0);
    glVertex3f(mx+5, my+5, map[a][b]+100);
    glVertex3f(mx+5, my-5, map[a][b]+100);
    
    
    glVertex3f(mx, my, map[a][b]+0);
    glVertex3f(mx-5, my+5, map[a][b]+100);
    glVertex3f(mx+5, my+5, map[a][b]+100);
  
    glVertex3f(mx, my, map[a][b]+0);
    glVertex3f(mx-5, my-5, map[a][b]+100);
    glVertex3f(mx-5, my+5, map[a][b]+100);
    
    
    
    glColor3f(1.0,0.0,0.0);
    glVertex3f(mx-5, my-5, map[a][b]+100);
    glVertex3f(mx+5, my-5, map[a][b]+100);
    glVertex3f(mx+5, my+5, map[a][b]+100);
    
    glVertex3f(mx+5, my+5, map[a][b]+100);
    glVertex3f(mx-5, my+5, map[a][b]+100);
    glVertex3f(mx-5, my-5, map[a][b]+100);
    
    glEnd();
  }
  
  
}

void renderWorld(){
  renderWorld(-100,-100);    
}





int regenerateSubWorld(){
    
  int a = (int)((mx+1+(MAPSIZE-2))/2);
  int b = (int)((my+1+(MAPSIZE-2))/2);
  
  a=a<0?0:a;
  a=a>(MAPSIZE-1)?(MAPSIZE-1):a;
  b=b<0?0:b;
  b=b>(MAPSIZE-1)?(MAPSIZE-1):b;
  
  int partX = a/(MAPSIZE/partialTerrainCount);
  int partY = b/(MAPSIZE/partialTerrainCount);
  
	if(refreshSubWorld){
    refreshSubWorld = 0;
    int x = a - (a&1);
    int y = b - (b&1);
    oldDetailedPos.x = x;
    oldDetailedPos.y = y;
    
    for(int i=x-gridPerPart*3; i<x+gridPerPart*3; i++){
      for(int j=y-gridPerPart*3; j<y+gridPerPart*3; j++){
        
        printf("%d %d\n", i, j);
        generateSubWorld(i, j, -1, 0);
        printf(".\n", i, j);
      }
    }
    onDetailedID = -1;
      
  }
  
  if(onDetailedID == -1 || detailedMeshIDs[onDetailedID].x != a || detailedMeshIDs[onDetailedID].y != b ){
     
    for(int i=0; i<detailedMeshSize; i++) 
      if(detailedMeshIDs[i].x == a && detailedMeshIDs[i].y == b){ onDetailedID = i; break; }             
  }
  
  if( abs(a - oldDetailedPos.x) > 1|| abs(b - oldDetailedPos.y) > 1){
    
    
    if(b < oldDetailedPos.y){
      for(;b<oldDetailedPos.y;oldDetailedPos.y--){
        int j = oldDetailedPos.y-1-gridPerPart*3;
        for(int i=oldDetailedPos.x-gridPerPart*3; i<oldDetailedPos.x+gridPerPart*3; i++){
          generateSubWorld(i, j, 1, oldDetailedPos.y-1+gridPerPart*3);
        }
      }
    }else  if(b > oldDetailedPos.y){
      for(;b>oldDetailedPos.y;oldDetailedPos.y++){
        int j = oldDetailedPos.y+1+gridPerPart*3-1;
        for(int i=oldDetailedPos.x-gridPerPart*3; i<oldDetailedPos.x+gridPerPart*3; i++){
          generateSubWorld(i, j, 1, oldDetailedPos.y+1-gridPerPart*3-1);
        }
      }
    }else if(a < oldDetailedPos.x){
      for(;a<oldDetailedPos.x;oldDetailedPos.x--){
        int i = oldDetailedPos.x-1-gridPerPart*3;
        for(int j=oldDetailedPos.y-gridPerPart*3; j<oldDetailedPos.y+gridPerPart*3; j++){
          generateSubWorld(i, j, 0, oldDetailedPos.x-1+gridPerPart*3);
        }
      }
    }else  if(a > oldDetailedPos.x){
      for(;a>oldDetailedPos.x;oldDetailedPos.x++){
        int i = oldDetailedPos.x+1+gridPerPart*3-1;
        for(int j=oldDetailedPos.y-gridPerPart*3; j<oldDetailedPos.y+gridPerPart*3; j++){
          generateSubWorld(i, j, 0, oldDetailedPos.x+1-gridPerPart*3-1);
        }
      }
    }
  }
  
}
void renderSubWorld(){
  

  int a = (int)((mx+1+(MAPSIZE-2))/2);
  int b = (int)((my+1+(MAPSIZE-2))/2);
  
  glUniform1f(glGetUniformLocation(p,"clampDist"),0.0f);
  a=a<0?0:a;
  a=a>(MAPSIZE-1)?(MAPSIZE-1):a;
  b=b<0?0:b;
  b=b>(MAPSIZE-1)?(MAPSIZE-1):b;
  
  int partX = a/(MAPSIZE/partialTerrainCount);
  int partY = b/(MAPSIZE/partialTerrainCount);
  
  
  glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60.0f, (GLfloat)(g_window->init.width)/(GLfloat)(g_window->init.height),
	10000, 15000000.0f);		
	glMatrixMode (GL_MODELVIEW);
	    
	    
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0)); 
  glEnableClientState(GL_NORMAL_ARRAY);
  glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12)); 
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(24));

  glPushMatrix();
  glScalef(mult, mult, mult);
  
  
  if(!g_keys->keyDown['X'])renderWorld(partX, partY);
  glPopMatrix();

  glMatrixMode (GL_PROJECTION);	
	glLoadIdentity ();
	gluPerspective (60.0f, (GLfloat)(g_window->init.width)/(GLfloat)(g_window->init.height),
		10, 1500000.0f);		
	glMatrixMode (GL_MODELVIEW);
	glClear(GL_DEPTH_BUFFER_BIT);
	    
    
  glUniform1f(glGetUniformLocation(p,"clampDist"),1.0f);
	      
  glPushMatrix();
  
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, detailedMeshVB);
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
  glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
  glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), BUFFER_OFFSET(24));
     
     
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, detailedMeshIB);
  
  if(!g_keys->keyDown['Z'])glDrawElements(GL_TRIANGLES, detailedMeshSize*12*detailedMeshDetail*detailedMeshDetail, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    
  //glEnableClientState(GL_COLOR_ARRAY); 
  glPopMatrix();
  //renderWorld(partX, partY);
    
    
    
    /*
  glDisable(GL_DEPTH_TEST);
  glBegin(GL_TRIANGLES);
  vec3 v0 = bufBig[(a*(MAPSIZE-2)+b)*5+0].pos;
  v0 *=  mult;
  vec3 v1 = bufBig[(a*(MAPSIZE-2)+b)*5+1].pos;
  v1 *=  mult;
  vec3 v2 = bufBig[(a*(MAPSIZE-2)+b)*5+2].pos;
  v2 *=  mult;
  vec3 v3 = bufBig[(a*(MAPSIZE-2)+b)*5+3].pos;
  v3 *=  mult;
  vec3 v4 = bufBig[(a*(MAPSIZE-2)+b)*5+4].pos;
  v4 *=  mult;
  glColor4f(1.0, 0.0, 0.0, 0.1);
  glVertex3fv((float*)&v0);
  glVertex3fv((float*)&v1);
  glVertex3fv((float*)&v4);
    
    
  glColor4f(0.0, 1.0, 0.0, 0.1);
  glVertex3fv((float*)&v3);
  glVertex3fv((float*)&v0);
  glVertex3fv((float*)&v4);
    
 
  glColor4f(0.0, 0.0, 1.0, 0.1);
  glVertex3fv((float*)&v1);
  glVertex3fv((float*)&v2);
  glVertex3fv((float*)&v4);
    
  glColor4f(1.0, 1.0, 1.0, 0.1);
  glVertex3fv((float*)&v2);
  glVertex3fv((float*)&v3);
  glVertex3fv((float*)&v4);
    
    
  glEnd();

  if(tri == 0) glColor4f(0.5, 0.0, 0.0, 1);
  if(tri == 1) glColor4f(0.0, 0.5, 0.0, 1);
  if(tri == 2) glColor4f(0.0, 0.0, 0.5, 1);
  if(tri == 3) glColor4f(0.5, 0.5, 0.5, 1);
  
  glBegin(GL_QUADS);
  glVertex3f(mx*mult, my*mult, (map[a][b]-1)*mult);
  glVertex3f(mx*mult+100, my*mult, (map[a][b]-1)*mult);
  glVertex3f(mx*mult+100, my*mult+100, (map[a][b]-1)*mult);
  glVertex3f(mx*mult, my*mult+100, (map[a][b]-1)*mult);
  glEnd();
    
  glEnable(GL_DEPTH_TEST);*/
}

    static float t = 90;
int drawCube(float x, float y, float z){
  glPushMatrix();   
    glScalef(1000, 1000, 1000);
    glColor3f(1.0, 0.0, 0.0); 
    glTranslatef(x, y, z*10+0.8);
    
    glRotatef(t, 1, 0, 0);
    glRotatef(x, 0, 1, 0);
    drawModel(&tree);
  /*
    glColor3f(1.0, 0.0, 0.0); 
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f(x-50, y+50, z+1000);
    glVertex3f(x-50, y-50, z+1000);
    glVertex3f(x-50, y-50, z);
    glVertex3f(x-50, y+50, z);
    
    glNormal3f(1, 0, 0);
    glVertex3f(x+50, y-50, z+1000);
    glVertex3f(x+50, y+50, z+1000);
    glVertex3f(x+50, y+50, z);
    glVertex3f(x+50, y-50, z);
    
    glColor3f(1.0, 1.0, 0.0);
    glNormal3f(0, -1, 0);
    
    
    glVertex3f(x-50, y-50, z);
    glVertex3f(x-50, y-50, z+1000);
    glVertex3f(x+50, y-50, z+1000);
    glVertex3f(x+50, y-50, z);
    
    glNormal3f(0, 1, 0);
    glVertex3f(x+50, y+50, z);
    glVertex3f(x+50, y+50, z+1000);
    glVertex3f(x-50, y+50, z+1000);
    glVertex3f(x-50, y+50, z);
    glEnd();*/
    
  glPopMatrix();
    
}

float treePos[100000][2];

void Draw(void){
  double v1[3], v2[3], out[3];

	glUniform3f(glGetUniformLocation(p,"playerPos"),mx,my,mz);
	
  
  glUniform1f(glGetUniformLocation(p,"clampDist"),0.0f);
	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glLoadIdentity();
  if(camera == 1){
    
	
  	glMatrixMode (GL_PROJECTION);	
    glLoadIdentity ();
    gluPerspective (60.0f, (GLfloat)(g_window->init.width)/(GLfloat)(g_window->init.height),
				10.0f, 1000000.0f);		
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();	
    
    gluLookAt(mx-80*car*sin(ang*RadToPI),my-80*car*cos(ang*RadToPI),mz+40*car,
            mx,my,mz,
            0,0,1);
              
    glDisable(GL_FOG);
  }else if (camera == 2 || camera == 3 ){
    regenerateSubWorld();
    if(camera == 2){
      
      glDisable(GL_FOG);
      
    }else{
      GLfloat density = 0.000001;
      
      GLfloat fogColor[4] = {0.0, 0.0, 0.0, 1.0};
      glEnable (GL_FOG);
      glFogi (GL_FOG_MODE, GL_EXP2);
      
      glFogfv (GL_FOG_COLOR, fogColor);
      glFogf (GL_FOG_DENSITY, density);
      glHint (GL_FOG_HINT, GL_NICEST);
    }
  	glMatrixMode (GL_PROJECTION);	
    glLoadIdentity ();
    gluPerspective(60.0f, (GLfloat)(g_window->init.width)/(GLfloat)(g_window->init.height),
				10, 500000.0f);		
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();	
	    
    int a = (int)((mx+1+(MAPSIZE-2))/2);;
    int b = (int)((my+1+(MAPSIZE-2))/2);
    a=a<0?0:a;
    a=a>(MAPSIZE-1)?(MAPSIZE-1):a;
    b=b<0?0:b;
    b=b>(MAPSIZE-1)?(MAPSIZE-1):b;
    
    if(camera == 2)
     gluLookAt(mx*mult                ,my*mult                 ,        getHeight(mx, my)*mult*2.5+1000,
              mx*mult+sin(ang*RadToPI)*0,my*mult+cos(ang*RadToPI)*0,    getHeight(mx, my)*mult+1,
              0,1,0);
  if(camera == 3)     
  gluLookAt(mx*mult,my*mult,getHeight2(mx, my)+cameraHeight,
  
            mx*mult+sin(ang*RadToPI)*sin(vertAng*RadToPI)*1,
            my*mult+cos(ang*RadToPI)*sin(vertAng*RadToPI)*1,    
            getHeight2(mx, my)+cameraHeight+cos(vertAng*RadToPI)*1,
            
              0,0,1);
              
    //if(camera == 2) glScalef(mult, mult, mult);
  
  }
  
  
  if(g_keys->keyDown['Q']) t++;
  printf("%f\n",t);
  
  if(camera == 1 ){
   // glScalef(1, 1, 0.5);
    renderWorld();
  }else if(camera == 3 || camera == 2){
    
    renderSubWorld();  
      static int zz = 1;
    for(int i=0; i<100; i++)
    {
      if(zz){
        treePos[i][0] = 200.0f*(rand()%10000)/10000;
        treePos[i][1] = 200.0f*(rand()%10000)/10000;
      }
      drawCube(treePos[i][0],treePos[i][1], getHeight(treePos[i][0]/10, treePos[i][1]/10));
    }
        zz = 0;
  }
  printf("%f %f\n", mx, my);
  
  glPopMatrix();
}

