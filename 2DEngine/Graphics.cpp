#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	rendertarget = NULL;
	brush = NULL;
}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (rendertarget) rendertarget->Release();
	if (brush) brush->Release();
}

bool Graphics::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)), &rendertarget
		);

	if (res != S_OK) return false;

	res = rendertarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);

	return true;
}

void Graphics::ClearScreen(float r, float g, float b)
{
	rendertarget->Clear(D2D1::ColorF(r, g, b));
}
void Graphics::DrawCircle(float x, float y, float radius, D2D1::ColorF color)
{
	brush->SetColor(color);
	rendertarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, 1.0f);
}
void Graphics::FillRect(float x, float y, float width, float height, D2D1::ColorF color)
{
	brush->SetColor(color);
	rendertarget->FillRectangle(D2D1::RectF(x, y, x + width, y + height), brush);
}
void Graphics::DrawLine(float x, float y, float x2, float y2, D2D1::ColorF color)
{
	brush->SetColor(color);
	rendertarget->DrawLine(D2D1::Point2F(x, y), D2D1::Point2F(x2, y2), brush, 1.0f);
}
void Graphics::DrawTextA(std::string string, float x, float y, float width, float height, D2D1::ColorF color)
{
	DrawTextA(string, x, y, width, height, color);
}