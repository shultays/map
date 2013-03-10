#include <windows.h>												
#include <gl/gl.h>												
#include <gl/glu.h>														
#include <gl/glext.h>
#include <io.h>
#include <fcntl.h>														
#include <stdio.h>														
#include "Foo.h"												
#include "Tools.h"							

		
#define WM_TOGGLEFULLSCREEN (WM_USER+1)									

static BOOL g_isProgramLooping;
static BOOL g_createFullScreen;

void TerminateApplication (GL_Window* window){
	PostMessage (window->hWnd, WM_QUIT, 0, 0);
	g_isProgramLooping = FALSE;
}

void ToggleFullscreen (GL_Window* window)
{
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
}

void ReshapeGL (int width, int height)
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (60.0f, (GLfloat)(width)/(GLfloat)(height),
					1.0f, 1000000.0f);		
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel)
{
  
	DEVMODE dmScreenSettings;	
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));
	dmScreenSettings.dmSize				= sizeof (DEVMODE);
	dmScreenSettings.dmPelsWidth		= width;
	dmScreenSettings.dmPelsHeight		= height;
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;
	}
	return TRUE;
}


BOOL CreateWindowGL (GL_Window* window)
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof (PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		window->init.bitsPerPixel,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};


	RECT windowRect = {0, 0, window->init.width, window->init.height};

	GLuint PixelFormat;

	if (window->init.isFullScreen == TRUE)
	{
		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;
		}
		else
		{
			windowStyle = WS_POPUP;
			windowExtendedStyle |= WS_EX_TOPMOST;
		}
	}
	else
	{
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	window->hWnd = CreateWindowEx (windowExtendedStyle,
								   "Hello",
								   "Hello",
								   windowStyle,
								   0, 0,
								   windowRect.right - windowRect.left,
								   windowRect.bottom - windowRect.top,
								   HWND_DESKTOP,
								   0,
								   window->init.application->hInstance,
								   window);

	if (window->hWnd == 0)
	{
		return FALSE;
	}

	window->hDC = GetDC (window->hWnd);
	if (window->hDC == 0)
	{
		DestroyWindow (window->hWnd);
		window->hWnd = 0;
		return FALSE;
	}

	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);
	if (PixelFormat == 0)
	{
		ReleaseDC (window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow (window->hWnd);
		window->hWnd = 0;
		return FALSE;
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)
	{
		ReleaseDC (window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow (window->hWnd);
		window->hWnd = 0;
		return FALSE;
	}

	window->hRC = wglCreateContext (window->hDC);
	if (window->hRC == 0)
	{
		ReleaseDC (window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow (window->hWnd);
		window->hWnd = 0;
		return FALSE;
	}

	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		wglDeleteContext (window->hRC);
		window->hRC = 0;
		ReleaseDC (window->hWnd, window->hDC);
		window->hDC = 0;
		DestroyWindow (window->hWnd);
		window->hWnd = 0;
		return FALSE;
	}

	ShowWindow (window->hWnd, SW_NORMAL);
	window->isVisible = TRUE;

	ReshapeGL (window->init.width, window->init.height);

	ZeroMemory (window->keys, sizeof (Keys));

	window->lastTickCount = GetTickCount ();

	return TRUE;
}

BOOL DestroyWindowGL (GL_Window* window)
{
	if (window->hWnd != 0)
	{	
		if (window->hDC != 0)	
		{
			wglMakeCurrent (window->hDC, 0);
			if (window->hRC != 0)
			{
				wglDeleteContext (window->hRC);
				window->hRC = 0;

			}
			ReleaseDC (window->hWnd, window->hDC);
			window->hDC = 0;
		}
		DestroyWindow (window->hWnd);
		window->hWnd = 0;
	}

	if (window->init.isFullScreen)
	{
		ChangeDisplaySettings (NULL,0);
		//ShowCursor (TRUE);
	}	
	return TRUE;
}


LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	switch (uMsg)
	{
		case WM_SYSCOMMAND:	
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}
		return 0;

		case WM_CREATE:
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;

		case WM_CLOSE:	
			TerminateApplication(window);
		return 0;

		case WM_SIZE:
			switch (wParam)
			{
				case SIZE_MINIMIZED:
					window->isVisible = FALSE;
				return 0;

				case SIZE_MAXIMIZED:
					window->isVisible = TRUE;
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));
				return 0;

				case SIZE_RESTORED:
					window->isVisible = TRUE;
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));
				return 0;
			}
		break;

		case WM_KEYDOWN:
      
			if ((wParam >= 0) && (wParam <= 255))
			{
				window->keys->keyDown [wParam] = TRUE;
				return 0;
			}
		break;

		case WM_KEYUP:	
      
			if ((wParam >= 0) && (wParam <= 255))	
			{
				window->keys->keyDown [wParam] = FALSE;
				return 0;
			}
		break;

		case WM_MOUSEMOVE :	
      
      POINT ptCursorPos;
      GetCursorPos(&ptCursorPos);

      mouseNewX = ptCursorPos.x; 
      mouseNewY = ptCursorPos.y;
      
      if(mouseNewX != window->init.width/2 || mouseNewY != window->init.height/2){
        SetCursorPos(window->init.width/2, window->init.height/2);
        
        mouseDiffX += mouseNewX-window->init.width/2;
        mouseDiffY += mouseNewY-window->init.height/2;
      }
      return 0;	
		case WM_TOGGLEFULLSCREEN:									
      
			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			PostMessage (hWnd, WM_QUIT, 0, 0);
			return 0;	
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);
}

BOOL RegisterWindowClass (Application* application)
{
	
	WNDCLASSEX windowClass;	
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));
	windowClass.cbSize			= sizeof (WNDCLASSEX);
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);
	windowClass.hInstance		= application->hInstance;
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName	= "Hello";
	if (RegisterClassEx (&windowClass) == 0)
	{
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  
	Application			application;
	GL_Window			window;
	Keys				keys;
	BOOL				isMessagePumpActive;
	MSG					msg;
	DWORD				tickCount;
	application.className = "OpenGL";
	application.hInstance = hInstance;

	ZeroMemory (&window, sizeof (GL_Window));
	window.keys	= &keys;
	window.init.application		= &application;
	window.init.title			= "avesum";
  RECT rc;
  GetWindowRect(GetDesktopWindow(), &rc);
	window.init.width			= rc.right-rc.left;
	window.init.height		=rc.bottom- rc.top;
	
	window.init.bitsPerPixel	= 32;
	window.init.isFullScreen	= TRUE;
	
	#if 0
  window.init.width	/=2;
	window.init.height	/=2;								
	window.init.isFullScreen	= FALSE;	
	#endif
	ZeroMemory (&keys, sizeof (Keys));
	
	
	if (RegisterWindowClass (&application) == FALSE)
	{
		MessageBox (HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

  int answer = MessageBox (HWND_DESKTOP, "Load Map?", "", MB_YESNOCANCEL);
  if(answer == IDYES)
  {
   LOAD = 1;
   SAVE = 0;
    
  }else if(answer == IDNO){
    
    LOAD = 0;
    SAVE = 1;
  }else if(answer == IDCANCEL){
    exit(0);
  }
    AllocConsole();

    ShowCursor(FALSE);
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;
    
	g_isProgramLooping = TRUE;
	g_createFullScreen = window.init.isFullScreen;
	while (g_isProgramLooping)
	{
		window.init.isFullScreen = g_createFullScreen;
		if (CreateWindowGL (&window) == TRUE)
		{
			if (Initialize (&window, &keys) == FALSE)
			{
				TerminateApplication (&window);
			}
			else
			{
				isMessagePumpActive = TRUE;
				while (isMessagePumpActive == TRUE)
				{
					if (PeekMessage (&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						if (msg.message != WM_QUIT)
						{
							DispatchMessage (&msg);
						}
						else
						{
							isMessagePumpActive = FALSE;
						}
					}
					
					tickCount = GetTickCount ();
					if(window.lastTickCount-tickCount>0){
						Update (tickCount - window.lastTickCount);
						window.lastTickCount = tickCount;
						Draw ();

						SwapBuffers (window.hDC);
					}
				}	
			}

			Deinitialize ();
			DestroyWindowGL (&window);
		}
		else
		{
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;
		}
	}
	
	UnregisterClass ("Hello", application.hInstance);
	return 0;
}
