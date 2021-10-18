#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


// Global variable
HINSTANCE hInst;
UINT  MessageCount = 0;
UINT  Count = 0;
int posX = 0;
int posY = 0;
int R = 0;
int G = 0;
int B = 0;
int dEl = 0;

typedef struct {
	int elX1, elX2, elY1, elY2;
	int R, G, B;
} Elips;
Elips elipses[100];
int elc = 0;
int elX1, elX2, elY1, elY2;
int elSel = -1;
int mooSel = -1;

// Function prototypes.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);
void paintRGBVal(HWND hWnd, HDC hDC, PAINTSTRUCT ps);
void paintElipse(HDC hDC, int posX1, int posY1, int posX2, int posY2);
void paintElipseRGB(HDC hDC, int posX1, int posY1, int posX2, int posY2, int R, int G, int B);

// Application entry point. This is the same as main() in standart C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	WNDCLASS wcx;          // register class
	HWND hWnd;

	hInst = hInstance;     // Save the application-instance handle.
		// Fill in the window class structure with parameters that describe the main window.

	wcx.style = CS_HREDRAW | CS_VREDRAW;              // redraw if size changes
	wcx.lpfnWndProc = (WNDPROC)MainWndProc;          // points to window procedure
	wcx.cbClsExtra = 0;                               // no extra class memory
	wcx.cbWndExtra = 0;                               // no extra window memory
	wcx.hInstance = hInstance;                        // handle to instance
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);      // predefined app. icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);        // predefined arrow
	wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
	wcx.lpszMenuName = (LPCSTR)"MainMenu";          // name of menu resource
	wcx.lpszClassName = (LPCSTR)"MainWClass";        // name of window class

	// Register the window class.

	if (!RegisterClass(&wcx)) return FALSE;

	// create window of registered class

	hWnd = CreateWindow(
		"MainWClass",        // name of window class
		"ITU",               // title-bar string
		WS_OVERLAPPEDWINDOW, // top-level window
		200,                  // default horizontal position
		25,                 // default vertical position
		1000,                // default width
		700,                 // default height
		(HWND)NULL,         // no owner window
		(HMENU)NULL,        // use class menu
		hInstance,           // handle to application instance
		(LPVOID)NULL);      // no window-creation data
	if (!hWnd) return FALSE;

	// Show the window and send a WM_PAINT message to the window procedure.
	// Record the current cursor position.

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// loop of message processing
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}



LRESULT CALLBACK MainWndProc(
	HWND hWnd,        // handle to window
	UINT uMsg,        // message identifier
	WPARAM wParam,    // first message parameter
	LPARAM lParam)    // second message parameter
{
	HDC         hDC;
	PAINTSTRUCT ps;
	POINT cursorPosition;

	// init cursor position 
	GetCursorPos(&cursorPosition);
	ScreenToClient(hWnd, &cursorPosition);
	Elips newE;
	switch (uMsg)
	{
	case WM_CREATE:
		break;

	// character input 
	case WM_CHAR:
		switch (wParam) {
		case 0x08:  // backspace
		case 0x0A:  // linefeed
		case 0x1B:  // escape
			break;

		case 0x09:  // tab
			memset(elipses, 0, 100);
			elc = 0;
			InvalidateRect(hWnd, NULL, 1);
			break;
		case 'r':
		case 'R':
			R = !R;
			InvalidateRect(hWnd, NULL, 1);
			break;
		case 'g':
		case 'G':
			G = !G;
			InvalidateRect(hWnd, NULL, 1);
			break;
		case 'b':
		case 'B':
			B = !B;
			InvalidateRect(hWnd, NULL, 1);
			break;
		default:
			break;
		}
		break;

	// key input
	case WM_KEYDOWN:
		switch (wParam) {
	        // update posX and posY in order to move object
		case VK_LEFT: // left arrow
			if (mooSel != -1) {
				elipses[mooSel].elX1-=5;
				elipses[mooSel].elX2-=5;
				InvalidateRect(hWnd, NULL, 1);
			}
			break;
		case VK_RIGHT: // right arrow
			if (mooSel != -1) {
				elipses[mooSel].elX1+=5;
				elipses[mooSel].elX2+=5;
				InvalidateRect(hWnd, NULL, 1);
			}
			break;
		case VK_UP: // up arrow
			if (mooSel != -1) {
				elipses[mooSel].elY1-=5;
				elipses[mooSel].elY2-=5;
				InvalidateRect(hWnd, NULL, 1);
			}
			break;
		case VK_DOWN: // down arrow
			if (mooSel != -1) {
				elipses[mooSel].elY1+=5;
				elipses[mooSel].elY2+=5;
				InvalidateRect(hWnd, NULL, 1);
			}
			break;
			

		// react on the other pressed keys 
		case VK_SPACE: // space
			break;
		case VK_BACK: // backspace
			break;
		case VK_TAB: // tab
			break;
	        // more virtual codes can be found here: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		}
		
		break;

	// get cursor position 
	case WM_MOUSEMOVE:
		posX = cursorPosition.x;
		posY = cursorPosition.y;
		elX2 = cursorPosition.x;
		elY2 = cursorPosition.y;
		InvalidateRect(hWnd, NULL, 1);
		break;

	// react on mouse clicks
	case WM_LBUTTONDOWN:
		dEl = 1;
		elX1 = cursorPosition.x;
		elY1 = cursorPosition.y;
		InvalidateRect(hWnd, NULL, 1);
		break;
	case WM_LBUTTONUP:
		newE.elX1 = elX1;
		newE.elY1 = elY1;
		dEl = 0;
		elX2 = cursorPosition.x;
		elY2 = cursorPosition.y;
		newE.elX2 = elX2;
		newE.elY2 = elY2;
		newE.R = R;
		newE.G = G;
		newE.B = B;
		InvalidateRect(hWnd, NULL, 1);
		elipses[elc++] = newE;
		break;

	case WM_RBUTTONDOWN:
		for (int i = 99; i >= 0; i--) {
			if (posX > elipses[i].elX1 && posX < elipses[i].elX2 && posY > elipses[i].elY1 && posY < elipses[i].elY2) {
				elSel = i;
				break;
			}
		}
		if (elSel != -1) {
			elipses[elSel].R = R;
			elipses[elSel].G = G;
			elipses[elSel].B = B;
			elSel = -1;
		}
		InvalidateRect(hWnd, NULL, 1);
		break;
	case WM_RBUTTONUP:
		for (int i = 99; i >= 0; i--) {
			if (posX > elipses[i].elX1 && posX < elipses[i].elX2 && posY > elipses[i].elY1 && posY < elipses[i].elY2) {
				mooSel = i;
				break;
			}
		}
		if (mooSel != -1) {
			elipses[mooSel].R = R;
			elipses[mooSel].G = G;
			elipses[mooSel].B = B;
		}
		InvalidateRect(hWnd, NULL, 1);
		break;
	// paint objects
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		paintRGBVal(hWnd, hDC, ps);
		paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
		paintPosition(hWnd, hDC, ps);
		if(dEl)
		paintElipse(hDC, elX1, elY1, elX2, elY2);
		for (int i = 0; i < elc; i++) {
			paintElipseRGB(hDC, elipses[i].elX1, elipses[i].elY1, elipses[i].elX2, elipses[i].elY2, elipses[i].R, elipses[i].G, elipses[i].B);
		}
		// paint other objects
		// paintObject2(hWnd, hDC, ps, posX, posY, cursorPosition);
		// paintObject3(hWnd, hDC, ps, posX, posY, cursorPosition);
		EndPaint(hWnd, &ps);
		DeleteDC(hDC);
		break;

		//
		// Process other messages.
		//

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void paintElipseRGB(HDC hDC, int posX1, int posY1, int posX2, int posY2, int R, int G, int B)
{
	HBRUSH brush, oldbrush;
	brush = CreateSolidBrush(RGB(150*R, 150*G, 150*B));
	oldbrush = SelectObject(hDC, brush);
	Ellipse(hDC, posX1, posY1, posX2, posY2);
	// Paint rectangles, ellipses, polygons, lines etc.
	return;
}

void paintElipse(HDC hDC, int posX1, int posY1, int posX2,int posY2)
{
	Ellipse(hDC, posX1, posY1, posX2, posY2);
	// Paint rectangles, ellipses, polygons, lines etc.
	return;
}
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY, POINT cursorPosition)
{
	// Paint rectangles, ellipses, polygons, lines etc.
	return;
}

void paintRGBVal(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "R: %s G: %s B: %s", R ? "ON" : "OFF", G ? "ON" : "OFF", B ? "ON" : "OFF");
	TextOut(hDC, 350, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
	// Paint rectangles, ellipses, polygons, lines etc.
	return;
}


void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps)
{
	char        text[40];          // buffer to store an output text
	HFONT       font;              // new large font
	HFONT       oldFont;           // saves the previous font

	font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, 0);
	oldFont = (HFONT)SelectObject(hDC, font);
	sprintf(text, "Position -- x:%d, y:%d", posX, posY);
	TextOut(hDC, 50, 600, text, (int)strlen(text));
	SelectObject(hDC, oldFont);
	DeleteObject(font);
}
