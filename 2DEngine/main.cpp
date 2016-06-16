#include <Windows.h>
#include <vector>
#include "Graphics.h"
#include <mutex>
#include <map>

Graphics* graphics;
int screenWidth = 600;
int screenHeight = 600;
void draw();
void reset();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }

	if (uMsg == WM_PAINT)
	{
		graphics->BeginDraw();

		graphics->ClearScreen(1.0f, 1.0f, 1.0f);
		
		reset();
		draw();

		graphics->EndDraw();
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void draw()
{
	
}

void reset()
{
	
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow)
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = "MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

	RECT rect = {0, 0, screenWidth, screenHeight};
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW); 

	HWND windowhandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWindow", "Knights Tour", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!windowhandle) return -1;

	graphics = new Graphics();
	if (!graphics->Init(windowhandle))
	{
		delete graphics;
		return -1;
	}

	ShowWindow(windowhandle, nCmdShow);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		DispatchMessage(&message);
	}

	delete graphics;

	return 0;
}