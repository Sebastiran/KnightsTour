#include <Windows.h>
#include <vector>
#include "Graphics.h"
#include <mutex>
#include <map>

Graphics* graphics;
int screenWidth = 600;
int screenHeight = 600;
void draw();
bool solve();
void reset();

const int startX = 0;
const int startY = 0;
const int size = 10;
int width = screenWidth / size;
std::mutex mutex;
std::pair<int, int> moveList[] = {
	{ 1, 2 },
	{ -1, 2 },
	{ 1, -2 },
	{ -1, -2 },
	{ 2, 1 },
	{ 2, -1 },
	{ -2, 1 },
	{ -2, -1 }
};

std::vector < std::pair<int, int> > tour;
bool chessboard[size][size];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY) { PostQuitMessage(0); return 0; }

	if (uMsg == WM_PAINT)
	{
		graphics->BeginDraw();

		graphics->ClearScreen(1.0f, 1.0f, 1.0f);
		
		reset();
		solve();
		draw();

		graphics->EndDraw();
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void draw()
{
	//Draw chessboard
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((i + j) % 2 == 0)
			{
				graphics->FillRect(i * width, j * width, width, width, D2D1::ColorF(0.7f, 0.7f, 0.7f, 1.0f));
			}
		}
	}

	mutex.lock();
	//Draw tour
	for (int i = 0; i < tour.size() - 1; i++)
	{
		graphics->DrawLine(
			tour.at(i).first * width + (width / 2), 
			tour.at(i).second * width + (width / 2), 
			tour.at(i + 1).first * width + (width / 2), 
			tour.at(i + 1).second * width + (width / 2), 
			D2D1::ColorF(0.0f, 0.7f, 0.0f, 1.0f));
	}
	mutex.unlock();
}

bool solve()
{
	mutex.lock();
	//If tour is done, stop.
	if (tour.size() == size * size)
	{
		mutex.unlock();
		return true;
	}
	mutex.unlock();
	auto p = tour.back();
	//Make a list of the legal moves in order of possible moves from that legal position from low to high.
	std::multimap<int, std::pair<int, int> > orderedMovesList;
	for (const auto &move : moveList)
	{
		auto x1 = p.first + move.first;
		auto y1 = p.second + move.second;
		if (x1 < 0 || x1 >= size || y1 < 0 || y1 >= size)
			continue;
		if (chessboard[x1][y1])
			continue;
		int count = 0;
		for (const auto &move2 : moveList)
		{
			auto x2 = x1 + move2.first;
			auto y2 = y1 + move2.second;
			if (x2 < 0 || x2 >= size || y2 < 0 || y2 >= size)
				continue;
			if (chessboard[x2][y2])
				continue;
			++count;
		}
		orderedMovesList.insert(std::make_pair(count, move));
	}
	//Pick the first valid legal move and add it to the tour
	for (const auto &move: orderedMovesList)
	{
		auto x = p.first + move.second.first;
		auto y = p.second + move.second.second;
		if (x < 0 || x >= size || y < 0 || y >= size)
			continue;
		if (chessboard[x][y])
			continue;
		mutex.lock();
		tour.push_back(std::make_pair(x, y));
		mutex.unlock();
		chessboard[x][y] = true;
		if (solve())
			return true;
		mutex.lock();
		tour.pop_back();
		mutex.unlock();
		chessboard[x][y] = false;
	}
	return false;
}

void reset()
{
	tour.clear();
	memset(chessboard, 0, sizeof(chessboard[0][0]) * size * size);

	tour.push_back(std::make_pair(startX, startY));
	chessboard[startX][startY] = true;
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