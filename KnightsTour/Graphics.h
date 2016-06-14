#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <string>

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* rendertarget;
	ID2D1SolidColorBrush* brush;

public:
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	void BeginDraw() { rendertarget->BeginDraw(); }
	void EndDraw() { rendertarget->EndDraw(); }

	void ClearScreen(float r, float g, float b);
	void DrawCircle(float x, float y, float radius, D2D1::ColorF color);
	void FillRect(float x, float y, float width, float height, D2D1::ColorF color);
	void DrawLine(float x, float y, float x2, float y2, D2D1::ColorF color);
	void DrawTextA(std::string string, float x, float y, float width, float height, D2D1::ColorF color);
};