#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK enumWindowProc(HWND, LPARAM);
HWND gHPaintWindowHandle;
BOOLEAN gbMouseLeftButtonClicked = FALSE;
POINT ptPrev;
WINDOWINFO winInfo;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Local Variables //
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR AppName[] = TEXT("Windows");

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = AppName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, TEXT("IDI_ICON1"));
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(& wndclass);

	hwnd = CreateWindow(AppName,
		TEXT("First Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (NULL == hwnd) {
		MessageBox(NULL, TEXT("Window Not Created"), TEXT("Error..."), 0);
		exit(0);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	EnumWindows(enumWindowProc, NULL);
	while (GetMessage(& msg, NULL, 0, 0)) {
		TranslateMessage(& msg);
		DispatchMessage(& msg);
	}

	
	return((int)msg.wParam);
	//Previously in Visual Studio 6
	//return(msg.wParam);
}

// Window Procedure
/*
the window procedure prepares for drawing when the user presses and holds the left mouse button (sending the WM_LBUTTONDOWN message). 
As the user moves the cursor within the window, the window procedure receives a series of WM_MOUSEMOVE messages.
For each message, the window procedure draws a line connecting the previous position and the current position. 
To draw the line, the procedure uses GetDC to retrieve a display device context; then, as soon as drawing is complete and before returning from the message, 
the procedure uses the ReleaseDC function to release the display device context. As soon as the user releases the mouse button, 
the window procedure clears the flag, and the drawing stops (which sends the WM_LBUTTONUP message).
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	switch (iMsg) {
	case WM_LBUTTONDOWN: {
		gbMouseLeftButtonClicked = TRUE;
		//BOOL res = PostMessage(gHPaintWindowHandle, iMsg, wParam, lParam);
		ptPrev.x = LOWORD(lParam);
		ptPrev.y = HIWORD(lParam);
		break;
	}
		
	case WM_MOUSEMOVE:
		if (gbMouseLeftButtonClicked == TRUE) {
			hdc = GetDC(gHPaintWindowHandle);
			MoveToEx(hdc, ptPrev.x, ptPrev.y, NULL);
			LineTo(hdc, ptPrev.x = LOWORD(lParam),
				ptPrev.y = HIWORD(lParam));
			ReleaseDC(gHPaintWindowHandle, hdc);
		}
			
		break;
	case WM_LBUTTONUP:
		//PostMessage(gHPaintWindowHandle, iMsg, wParam, lParam);
		if (gbMouseLeftButtonClicked == TRUE) {
			hdc = GetDC(gHPaintWindowHandle);
			MoveToEx(hdc, ptPrev.x, ptPrev.y, NULL);
			LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
			ReleaseDC(gHPaintWindowHandle, hdc);
			
		}
		gbMouseLeftButtonClicked = FALSE;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		PostMessage(gHPaintWindowHandle, WM_QUIT, NULL, NULL);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

//It receives top - level window handles.
BOOL CALLBACK enumWindowProc(HWND hwnd, LPARAM lParam) {
	//get window information
	winInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd,&winInfo);
	TCHAR windowTitle[255];

	//get the title of the window
	GetWindowText(hwnd, windowTitle, 255);
	TCHAR paintTitle[255] = TEXT("Untitled - Paint");
	
	//get handle of untitled paint window
	if (wcscmp(windowTitle, paintTitle) == 0)
		gHPaintWindowHandle = hwnd;
	return TRUE;
}