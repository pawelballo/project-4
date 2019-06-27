
#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <queue>
#include <winapifamily.h>
#include <cmath>
#include <math.h>
#include <atlstr.h>
#include <ctime>
#include <sstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <gdiplus.h>

using namespace Gdiplus;
struct pozycja {
	float x;
	float y;
	float poz;
};

struct object {
	float x;
	float y;
};

struct bufor {
	pozycja middle;
	pozycja end;
	object grabbing1;
	object grabbing2;
	object grabbing3;
	object grabbing4;
	object grabbing5;
	object grabbing6;
};


const int R = 200;
int q = 4;
float m = 0;
int id;
float n = 0;
int u = 0;
bool zapisuje = false;
bool dropping = false;
bool obj = false;
bool obj1 = false;
float roznicaX = 0;
float roznicaY = 0;
std::queue<bufor> zapis;
pozycja srodek;
pozycja koniec;
object obiekt1;
bool grabb1 = false;
object obiekt2;
bool grabb2 = false;
object obiekt3;
bool grabb3 = false;
object obiekt4;
bool grabb4 = false;
object obiekt5;
bool grabb5 = false;
object obiekt6;
bool grabb6 = false;
HINSTANCE hInst;

TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hwndButton;
HWND hwndText;


RECT drawArea1 = { 0, 150, 1000, 1000 };

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

void creatobject(object* b, int i);

void PaintRobot(HDC hdc) {
	Graphics graphics(hdc);
	SmoothingMode smoothingMode = SmoothingModeAntiAlias;
	graphics.SetSmoothingMode(smoothingMode);
	SolidBrush brus1(Color(255, 255, 0, 0));
	Pen pen2(Color(255, 0, 0, 255), 2.5);
	Pen pen5(Color(255, 0, 0, 0), 2.5);
	float i, j;
	i = 500;
	j = 600;
	graphics.DrawLine(&pen5, 0, 600, 1500, 600);
	graphics.DrawLine(&pen2, srodek.x, srodek.y, koniec.x, koniec.y);
	graphics.DrawLine(&pen2, i, j, srodek.x, srodek.y);
	i = 10;
	graphics.FillEllipse(&brus1, srodek.x - 5, srodek.y - 5, i, i);
	graphics.FillEllipse(&brus1, koniec.x - 5, koniec.y - 5, i, i);
	graphics.FillEllipse(&brus1, 495, 595, 10, 10);
	if (obj1 == true) {
		int i4 = 0;
		SolidBrush brush1(Color(255, 0, 100, 0));
		if (obj == true)creatobject(&obiekt1, i4);
		i4 = 1;
		graphics.FillRectangle(&brush1, (int)obiekt1.x, (int)obiekt1.y, 30,30);
		SolidBrush brush2(Color(255, 255, 215, 0));
		if (obj == true)creatobject(&obiekt2, i4);
		i4 = 2;           
		graphics.FillRectangle(&brush2, (int)obiekt2.x, (int)obiekt2.y, 30,30);
		SolidBrush brush3(Color(255, 255, 69, 0));
		if (obj == true)creatobject(&obiekt3, i4);
		i4 = 3;
		graphics.FillRectangle(&brush3, (int)obiekt3.x, (int)obiekt3.y, 30,30);
		SolidBrush brush4(Color(255, 106, 90, 205));
		if (obj == true)creatobject(&obiekt4, i4);
		i4 = 4;
		graphics.FillRectangle(&brush4, (int)obiekt4.x, (int)obiekt4.y, 30,30);
		SolidBrush brush5(Color(255, 236, 24, 208));
		if (obj == true)creatobject(&obiekt5, i4);
		i4 = 5;
		graphics.FillRectangle(&brush5, (int)obiekt5.x, (int)obiekt5.y, 30, 30);
		SolidBrush brush6(Color(255, 108, 91, 106));
		if (obj == true)creatobject(&obiekt6, i4);
		graphics.FillRectangle(&brush6, (int)obiekt6.x, (int)obiekt6.y, 30, 30);
	}
}

void StartPaintRobot(HDC hdc) {
	srodek.x = 660;
	srodek.y = 480;
	koniec.x = 820;
	koniec.y = 360;
	srodek.poz = 2 * 3.14 - asin(0.6);
	koniec.poz = 2 * 3.14 - asin(0.6);
	PaintRobot(hdc);
}

void RepaintRobot(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea)
{
	HBITMAP hBmpBk;
	HDC mem;
	mem = CreateCompatibleDC(GetDC(NULL));
	hBmpBk = CreateCompatibleBitmap(GetDC(NULL),1000, 1000);
	SelectObject(mem, hBmpBk);
	hdc = BeginPaint(hWnd, &ps);
	FillRect(mem, &drawArea1, (HBRUSH)GetStockObject(WHITE_BRUSH));
	SetBkMode(mem, TRANSPARENT);
	PaintRobot(mem);
	BitBlt(hdc, 0, 0, 1000, 1000, mem, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);
	DeleteObject(hBmpBk);
}


int OnCreate(HWND window)
{
	srand(time(NULL));
	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;


	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);



	hwndButton = CreateWindow(TEXT("button"),
		TEXT("koniec++"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON1,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("koniec--"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		80, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON2,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("srodek++"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		160, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON3,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("srodek---"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		240, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON4,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("predkosc++"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		320, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON5,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("predkosc---"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON6,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("reset"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		480, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON7,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("zapis"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		560, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON8,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("koniec zapisu"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		640, 0,
		100, 50,
		hWnd,
		(HMENU)ID_BUTTON9,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("odwt�rz"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		740, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON10,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("chwy�"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		820, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON11,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("pu��"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON12,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("stw�rz obiekty"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		980, 0,
		110, 50,
		hWnd,
		(HMENU)ID_BUTTON13,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("usu� zapis"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1090, 0,
		110, 50,
		hWnd,
		(HMENU)ID_BUTTON14,
		hInstance,
		NULL);
	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void spr(pozycja* koniec1, pozycja* srodek2) {
	while (koniec1->poz < 0 || koniec1->poz >= 6.28) {
		if (koniec1->poz >= 6.28) koniec1->poz = koniec1->poz - 6.28;
		if (koniec1->poz < 0) koniec1->poz = koniec1->poz + 6.28;
	}

	while (srodek2->poz < 0 || srodek2->poz >= 6.28) {
		if (srodek2->poz >= 6.28) srodek2->poz = srodek2->poz - 6.28;
		if (srodek2->poz < 0) srodek2->poz = srodek2->poz + 6.28;
	}
}

void saving(std::queue<bufor>* save, pozycja* middle, pozycja* end, object* obiekt1, object* obiekt2, object* obiekt3, object* obiekt4, object* obiekt5, object* obiekt6) {
	bufor tymczas;
	tymczas.middle.poz = middle->poz;
	tymczas.middle.x = middle->x;
	tymczas.middle.y = middle->y;
	tymczas.end.poz = end->poz;
	tymczas.end.x = end->x;
	tymczas.end.y = end->y;
	if (obj1 == true) {
		tymczas.grabbing1.x = obiekt1->x;
		tymczas.grabbing1.y = obiekt1->y;
		tymczas.grabbing2.x = obiekt2->x;
		tymczas.grabbing2.y = obiekt2->y;
		tymczas.grabbing3.x = obiekt3->x;
		tymczas.grabbing3.y = obiekt3->y;
		tymczas.grabbing4.x = obiekt4->x;
		tymczas.grabbing4.y = obiekt4->y;
		tymczas.grabbing5.x = obiekt5->x;
		tymczas.grabbing5.y = obiekt5->y;
		tymczas.grabbing6.x = obiekt6->x;
		tymczas.grabbing6.y = obiekt6->y;
	}

	if (!(*save).empty()) {
		bufor r;
		r = (*save).back();
		if (dropping ==true || (r.end.x != tymczas.end.x) || (r.end.y != tymczas.end.y) || (r.end.poz != tymczas.end.poz) || (r.middle.x != tymczas.middle.x) || (r.middle.y != tymczas.middle.y) || (r.middle.poz != tymczas.middle.poz)) (*save).push(tymczas);
	}
	if ((*save).empty()) (*save).push(tymczas);
}

void spraw(float* k1, float* k2) {
	while (*k1 < 0 || *k1 >= 360) {
		if (*k1 >= 360)* k1 = *k1 - 360;
		if (*k1 < 0)* k1 = *k1 + 360;
	}
	while (*k2 < 0 || *k2 >= 360) {
		if (*k2 >= 360)* k2 = *k2 - 360;
		if (*k2 < 0)* k2 = *k2 + 360;
	}
}

int play(std::queue<bufor>* save, HWND hWnd) {
	if (u == (*save).size()){
		u = 0;
		return 0; 
	}
	else {
		u++;
		InvalidateRect(hWnd, &drawArea1, TRUE);
		(*save).push((*save).front());
		srodek.x = ((*save).front()).middle.x;
		srodek.y = ((*save).front()).middle.y;
		koniec.x = ((*save).front()).end.x;
		koniec.y = ((*save).front()).end.y;
		obiekt1.x= ((*save).front()).grabbing1.x;
		obiekt1.y= ((*save).front()).grabbing1.y;
		obiekt2.x= ((*save).front()).grabbing2.x;
		obiekt2.y= ((*save).front()).grabbing2.y;
		obiekt3.x= ((*save).front()).grabbing3.x;
		obiekt3.y= ((*save).front()).grabbing3.y;
		obiekt4.x= ((*save).front()).grabbing4.x;
		obiekt4.y= ((*save).front()).grabbing4.y;
		obiekt5.x = ((*save).front()).grabbing5.x;
		obiekt5.y = ((*save).front()).grabbing5.y;
		obiekt6.x = ((*save).front()).grabbing6.x;
		obiekt6.y = ((*save).front()).grabbing6.y;
		(*save).pop();
		return 1;
	}
}

void creatobject(object* b, int i) {
	int wylos = 0;
	if (i == 0) {
		wylos = (std::rand() % 700) + 110;
		(*b).y = 570;
		(*b).x = wylos;
	}
	if (i == 1) {
		do {
			wylos = (std::rand() % 700) + 110;
		} while (!((obiekt1.x > wylos + 30 && obiekt1.x > wylos) || (obiekt1.x + 30 < wylos && obiekt1.x + 30 < wylos + 30)));
		(*b).y = 570;
		(*b).x = wylos;
	}
	if (i == 2) {
		do {
			wylos = (std::rand() % 700) + 110;
		} while (!(((obiekt1.x > wylos + 30 && obiekt1.x > wylos) || (obiekt1.x + 30 < wylos && obiekt1.x + 30 < wylos + 30)) && ((obiekt2.x > wylos + 30 && obiekt2.x > wylos) || (obiekt2.x + 30 < wylos && obiekt2.x + 30 < wylos + 30))));
		(*b).y = 570;
		(*b).x = wylos;
	}
	if (i == 3) {
		do {
			wylos = (std::rand() % 700) + 110;
		} while (!(((obiekt1.x > wylos + 40 && obiekt1.x > wylos) || (obiekt1.x + 40 < wylos && obiekt1.x + 40 < wylos + 40)) && ((obiekt2.x > wylos + 40 && obiekt2.x > wylos) || (obiekt2.x + 40 < wylos && obiekt2.x + 40 < wylos + 40)) && ((obiekt3.x > wylos + 40 && obiekt3.x > wylos) || (obiekt3.x + 40 < wylos && obiekt3.x + 40 < wylos + 40))));
		(*b).y = 570;
		(*b).x = wylos;
	}
	if (i == 4) {
		do {
			wylos = (std::rand() % 700) + 110;
		} while (!(((obiekt1.x > wylos + 40 && obiekt1.x > wylos) || (obiekt1.x + 40 < wylos && obiekt1.x + 40 < wylos + 40)) && ((obiekt2.x > wylos + 40 && obiekt2.x > wylos) || (obiekt2.x + 40 < wylos && obiekt2.x + 40 < wylos + 40)) && ((obiekt3.x > wylos + 40 && obiekt3.x > wylos) || (obiekt3.x + 40 < wylos && obiekt3.x + 40 < wylos + 40))&&((obiekt4.x > wylos + 30 && obiekt4.x > wylos) || (obiekt4.x + 30 < wylos && obiekt4.x + 30 < wylos + 30))));
		(*b).y = 570;
		(*b).x = wylos;
	}
	if (i == 5) {
		do {
			wylos = (std::rand() % 700) + 110;
		} while (!(((obiekt1.x > wylos + 40 && obiekt1.x > wylos) || (obiekt1.x + 40 < wylos && obiekt1.x + 40 < wylos + 40)) && ((obiekt2.x > wylos + 40 && obiekt2.x > wylos) || (obiekt2.x + 40 < wylos && obiekt2.x + 40 < wylos + 40)) && ((obiekt3.x > wylos + 40 && obiekt3.x > wylos) || (obiekt3.x + 40 < wylos && obiekt3.x + 40 < wylos + 40))&&((obiekt4.x > wylos + 30 && obiekt4.x > wylos) || (obiekt4.x + 30 < wylos && obiekt4.x + 30 < wylos + 30))&&((obiekt5.x > wylos + 30 && obiekt5.x > wylos) || (obiekt5.x + 30 < wylos && obiekt5.x + 30 < wylos + 30))));
		(*b).y = 570;
		(*b).x = wylos;
	}
	grabb1 = false;
	grabb2 = false;
	grabb3 = false;
	grabb4 = false;
	grabb5 = false;
	grabb6 = false;
}

void dlt() {
	while (!zapis.empty()) {
		zapis.pop();
	}
}

void grab() {
	if (grabb1 == true) {
		obiekt1.x = koniec.x + roznicaX;
		obiekt1.y = koniec.y + roznicaY;
	}
	if (grabb2 == true) {
		obiekt2.x = koniec.x + roznicaX;
		obiekt2.y = koniec.y + roznicaY;
	}
	if (grabb3 == true) {
		obiekt3.x = koniec.x + roznicaX;
		obiekt3.y = koniec.y + roznicaY;
	}
	if (grabb4 == true) {
		obiekt4.x = koniec.x + roznicaX;
		obiekt4.y = koniec.y + roznicaY;
	}
	if (grabb5 == true) {
		obiekt5.x = koniec.x + roznicaX;
		obiekt5.y = koniec.y + roznicaY;
	}
	if (grabb6 == true) {
		obiekt6.x = koniec.x + roznicaX;
		obiekt6.y = koniec.y + roznicaY;
	}
}

int drop() {
	if (grabb1 == true) {
		obiekt1.y = obiekt1.y + 10;
		if (obiekt1.y >= obiekt2.y - 30 && ((obiekt1.x > obiekt2.x - 30 && obiekt1.x<=obiekt2.x)|| (obiekt1.x>=obiekt2.x && obiekt1.x < obiekt2.x + 30))) { obiekt1.y = obiekt2.y - 30; return 0; }
		if (obiekt1.y >= obiekt3.y - 30 &&((obiekt1.x > obiekt3.x - 30 && obiekt1.x <= obiekt3.x) || (obiekt1.x >= obiekt3.x && obiekt1.x < obiekt3.x + 30))) { obiekt1.y = obiekt3.y - 30; return 0; }
		if (obiekt1.y >= obiekt4.y - 30 &&((obiekt1.x > obiekt4.x - 30 && obiekt1.x <= obiekt4.x) || (obiekt1.x >= obiekt4.x && obiekt1.x < obiekt4.x + 30))) { obiekt1.y = obiekt4.y - 30; return 0; }
		if (obiekt1.y >= obiekt5.y - 30 &&((obiekt1.x > obiekt5.x - 30 && obiekt1.x <= obiekt5.x) || (obiekt1.x >= obiekt5.x && obiekt1.x < obiekt5.x + 30))) { obiekt1.y = obiekt5.y - 30; return 0; }
		if (obiekt1.y >= obiekt6.y - 30 &&((obiekt1.x > obiekt6.x - 30 && obiekt1.x <= obiekt6.x) || (obiekt1.x >= obiekt6.x && obiekt1.x < obiekt6.x + 30))) { obiekt1.y = obiekt6.y - 30; return 0; }
		if (obiekt1.y >= 570) {
			obiekt1.y = 570;
			return 0;
		}
		else return 1;
	}
	if (grabb2 == true) {
		obiekt2.y = obiekt2.y + 10;
		if (obiekt2.y >= obiekt1.y - 30 &&((obiekt2.x > obiekt1.x - 30 && obiekt2.x <= obiekt1.x) || (obiekt2.x >= obiekt1.x && obiekt2.x < obiekt1.x + 30))) { obiekt2.y = obiekt1.y - 30; return 0; }
		if (obiekt2.y >= obiekt3.y - 30 &&((obiekt2.x > obiekt3.x - 30 && obiekt2.x <= obiekt3.x) || (obiekt2.x >= obiekt3.x && obiekt2.x < obiekt3.x + 30))){ obiekt2.y = obiekt3.y - 30; return 0; }
		if (obiekt2.y >= obiekt4.y - 30 &&((obiekt2.x > obiekt4.x - 30 && obiekt2.x <= obiekt4.x) || (obiekt2.x >= obiekt4.x && obiekt2.x < obiekt4.x + 30))){ obiekt2.y = obiekt4.y - 30; return 0; }
		if (obiekt2.y >= obiekt5.y - 30 &&((obiekt2.x > obiekt5.x - 30 && obiekt2.x <= obiekt5.x) || (obiekt2.x >= obiekt5.x && obiekt2.x < obiekt5.x + 30))){ obiekt2.y = obiekt5.y - 30; return 0; }
		if (obiekt2.y >= obiekt6.y - 30 &&((obiekt2.x > obiekt6.x - 30 && obiekt2.x <= obiekt6.x) || (obiekt2.x >= obiekt6.x && obiekt2.x < obiekt6.x + 30))){ obiekt2.y = obiekt6.y - 30; return 0; }
		if (obiekt2.y >= 570) {
			obiekt2.y = 570;
			return 0;
		}
		else return 1;
	}
	if (grabb3 == true) {
		obiekt3.y = obiekt3.y + 10;
		if (obiekt3.y >= obiekt1.y - 30 &&((obiekt3.x > obiekt1.x - 30 && obiekt3.x <= obiekt1.x) || (obiekt3.x >= obiekt1.x && obiekt3.x < obiekt1.x + 30))) { obiekt3.y = obiekt1.y - 30; return 0; }
		if (obiekt3.y >= obiekt2.y - 30 &&((obiekt3.x > obiekt2.x - 30 && obiekt3.x <= obiekt2.x) || (obiekt3.x >= obiekt2.x && obiekt3.x < obiekt2.x + 30))) { obiekt3.y = obiekt2.y - 30; return 0; }
		if (obiekt3.y >= obiekt4.y - 30 &&((obiekt3.x > obiekt4.x - 30 && obiekt3.x <= obiekt4.x) || (obiekt3.x >= obiekt4.x && obiekt3.x < obiekt4.x + 30))) { obiekt3.y = obiekt4.y - 30; return 0; }
		if (obiekt3.y >= obiekt5.y - 30 &&((obiekt3.x > obiekt5.x - 30 && obiekt3.x <= obiekt5.x) || (obiekt3.x >= obiekt5.x && obiekt3.x < obiekt5.x + 30))) { obiekt3.y = obiekt5.y - 30; return 0; }
		if (obiekt3.y >= obiekt6.y - 30 &&((obiekt3.x > obiekt6.x - 30 && obiekt3.x <= obiekt6.x) || (obiekt3.x >= obiekt6.x && obiekt3.x < obiekt6.x + 30))) { obiekt3.y = obiekt6.y - 30; return 0; }
		if (obiekt3.y >= 570) {
			obiekt3.y = 570;
			return 0;
		}
		else return 1;
	}
	if (grabb4 == true) {
		obiekt4.y = obiekt4.y + 10;
		if (obiekt4.y >= obiekt1.y - 30 &&((obiekt4.x > obiekt1.x - 30 && obiekt4.x <= obiekt1.x) || (obiekt4.x >= obiekt1.x && obiekt4.x < obiekt1.x + 30))) { obiekt4.y = obiekt1.y - 30; return 0; }
		if (obiekt4.y >= obiekt2.y - 30 &&((obiekt4.x > obiekt2.x - 30 && obiekt4.x <= obiekt2.x) || (obiekt4.x >= obiekt2.x && obiekt4.x < obiekt2.x + 30))) { obiekt4.y = obiekt2.y - 30; return 0; }
		if (obiekt4.y >= obiekt3.y - 30 &&((obiekt4.x > obiekt3.x - 30 && obiekt4.x <= obiekt3.x) || (obiekt4.x >= obiekt3.x && obiekt4.x < obiekt3.x + 30))) { obiekt4.y = obiekt3.y - 30; return 0; }
		if (obiekt4.y >= obiekt5.y - 30 &&((obiekt4.x > obiekt5.x - 30 && obiekt4.x <= obiekt5.x) || (obiekt4.x >= obiekt5.x && obiekt4.x < obiekt5.x + 30))) { obiekt4.y = obiekt5.y - 30; return 0; }
		if (obiekt4.y >= obiekt6.y - 30 &&((obiekt4.x > obiekt6.x - 30 && obiekt4.x <= obiekt6.x) || (obiekt4.x >= obiekt6.x && obiekt4.x < obiekt6.x + 30))) { obiekt4.y = obiekt6.y - 30; return 0; }
		if (obiekt4.y >= 570) {
			obiekt4.y = 570;
			return 0;
		}
		else return 1;
	}
	if (grabb5 == true) {
		obiekt5.y = obiekt5.y + 10;
		if (obiekt5.y >= obiekt1.y - 30 && ((obiekt5.x > obiekt1.x - 30 && obiekt5.x <= obiekt1.x) || (obiekt5.x >= obiekt1.x && obiekt5.x < obiekt1.x + 30))) { obiekt5.y = obiekt1.y - 30; return 0; }
		if (obiekt5.y >= obiekt2.y - 30 && ((obiekt5.x > obiekt2.x - 30 && obiekt5.x <= obiekt2.x) || (obiekt5.x >= obiekt2.x && obiekt5.x < obiekt2.x + 30))) { obiekt5.y = obiekt2.y - 30; return 0; }
		if (obiekt5.y >= obiekt3.y - 30 && ((obiekt5.x > obiekt3.x - 30 && obiekt5.x <= obiekt3.x) || (obiekt5.x >= obiekt3.x && obiekt5.x < obiekt3.x + 30))) { obiekt5.y = obiekt3.y - 30; return 0; }
		if (obiekt5.y >= obiekt4.y - 30 && ((obiekt5.x > obiekt4.x - 30 && obiekt5.x <= obiekt4.x) || (obiekt5.x >= obiekt4.x && obiekt5.x < obiekt4.x + 30))) { obiekt5.y = obiekt4.y - 30; return 0; }
		if (obiekt5.y >= obiekt6.y - 30 && ((obiekt5.x > obiekt6.x - 30 && obiekt5.x <= obiekt6.x) || (obiekt5.x >= obiekt6.x && obiekt5.x < obiekt6.x + 30))) { obiekt5.y = obiekt6.y - 30; return 0; }
		if (obiekt5.y >= 570) {
			obiekt5.y = 570;
			return 0;
		}
		else return 1;
	}
	if (grabb6 == true) {
		obiekt6.y = obiekt6.y + 10;
		if (obiekt6.y >= obiekt1.y - 30 && ((obiekt6.x > obiekt1.x - 30 && obiekt6.x <= obiekt1.x) || (obiekt6.x >= obiekt1.x && obiekt6.x < obiekt1.x + 30))) { obiekt6.y = obiekt1.y - 30; return 0; }
		if (obiekt6.y >= obiekt2.y - 30 && ((obiekt6.x > obiekt2.x - 30 && obiekt6.x <= obiekt2.x) || (obiekt6.x >= obiekt2.x && obiekt6.x < obiekt2.x + 30))) { obiekt6.y = obiekt2.y - 30; return 0; }
		if (obiekt6.y >= obiekt3.y - 30 && ((obiekt6.x > obiekt3.x - 30 && obiekt6.x <= obiekt3.x) || (obiekt6.x >= obiekt3.x && obiekt6.x < obiekt3.x + 30))) { obiekt6.y = obiekt3.y - 30; return 0; }
		if (obiekt6.y >= obiekt4.y - 30 && ((obiekt6.x > obiekt4.x - 30 && obiekt6.x <= obiekt4.x) || (obiekt6.x >= obiekt4.x && obiekt6.x < obiekt4.x + 30))) { obiekt6.y = obiekt4.y - 30; return 0; }
		if (obiekt6.y >= obiekt5.y - 30 && ((obiekt6.x > obiekt5.x - 30 && obiekt6.x <= obiekt5.x) || (obiekt6.x >= obiekt5.x && obiekt6.x < obiekt5.x + 30))) { obiekt6.y = obiekt5.y - 30; return 0; }
		if (obiekt6.y >= 570) {
			obiekt6.y = 570;
			return 0;
		}
		else return 1;
	}
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	float pp = 2 * 3.14 - asin(0.6);
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1:
			InvalidateRect(hWnd, &drawArea1, TRUE);
			m = m + q;
			spraw(&m, &n);
			float r1, r2;
			r1 = koniec.poz;
			r2 = srodek.poz;
			koniec.poz = pp + ((3.14 * m) / 180);
			spr(&koniec, &srodek);
			koniec.x = srodek.x + cos(koniec.poz) * R;
			koniec.y = srodek.y + sin(koniec.poz) * R;
			if (koniec.y > 600) {
				koniec.poz = 6.28 - r2;
				spr(&koniec, &srodek);
				m = m - q + (((koniec.poz - r1) * 180) / 3.14);
				spraw(&m, &n);
				koniec.y = 600;
				koniec.x = srodek.x + cos(koniec.poz) * R;
			}
			if (grabb1 == true || grabb2 == true || grabb3 == true || grabb4 == true || grabb5 == true || grabb6 == true) {
				grab();
				if (obiekt1.y+30 > 600) {
					obiekt1.y = 570;
					koniec.y=obiekt1.y-roznicaY;
				}
				if (obiekt2.y + 30 > 600) {
					obiekt2.y = 570;
					koniec.y = obiekt2.y - roznicaY;
				}
				if (obiekt3.y + 30 > 600) {
					obiekt3.y = 570;
					koniec.y = obiekt3.y - roznicaY;
				}
				if (obiekt4.y + 30 > 600) {
					obiekt4.y = 570;
					koniec.y = obiekt4.y - roznicaY;
				}
				if (obiekt5.y + 30 > 600) {
					obiekt5.y = 570;
					koniec.y = obiekt5.y - roznicaY;
				}
				if (obiekt6.y + 30 > 600) {
					obiekt6.y = 570;
					koniec.y = obiekt6.y - roznicaY;
				}
			}
			if (zapisuje == true) saving(&zapis, &srodek, &koniec, &obiekt1, &obiekt2, &obiekt3, &obiekt4, &obiekt5, &obiekt6);
			RepaintRobot(hWnd, hdc,ps,&drawArea1);
			break;
		case ID_BUTTON2:
			InvalidateRect(hWnd, &drawArea1, TRUE);
			m = m - q;
			spraw(&m, &n);
			float r3, r4;
			r3 = koniec.poz;
			r4 = srodek.poz;
			koniec.poz = pp + ((3.14 * m) / 180);
			koniec.x = srodek.x + cos(koniec.poz) * R;
			koniec.y = srodek.y + sin(koniec.poz) * R;
			spr(&koniec, &srodek);
			if (koniec.y >600) {
				koniec.poz = 6.28 - r4;
				spr(&koniec, &srodek);
				m = m + q - (((koniec.poz - r3) * 180) / 3.14);
				spraw(&m, &n);
				koniec.y = 600;
				koniec.x = srodek.x + cos(koniec.poz) * R;
			} 
			if (grabb1 == true || grabb2 == true || grabb3 == true || grabb4 == true || grabb5 == true || grabb6 == true) {
				grab();
				if (obiekt1.y + 30 > 600) {
					obiekt1.y = 570;
					koniec.y = obiekt1.y - roznicaY;
				}
				if (obiekt2.y + 30 > 600) {
					obiekt2.y = 570;
					koniec.y = obiekt2.y - roznicaY;
				}
				if (obiekt3.y + 30 > 600) {
					obiekt3.y = 570;
					koniec.y = obiekt3.y - roznicaY;
				}
				if (obiekt4.y + 30 > 600) {
					obiekt4.y = 570;
					koniec.y = obiekt4.y - roznicaY;
				}
				if (obiekt5.y + 30 > 600) {
					obiekt5.y = 570;
					koniec.y = obiekt5.y - roznicaY;
				}
				if (obiekt6.y + 30 > 600) {
					obiekt6.y = 570;
					koniec.y = obiekt6.y - roznicaY;
				}
			}
			if (zapisuje == true) saving(&zapis, &srodek, &koniec,&obiekt1,&obiekt2,&obiekt3,&obiekt4, &obiekt5, &obiekt6);
			RepaintRobot(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON3:
			InvalidateRect(hWnd, &drawArea1, TRUE);
			float k1, k2;
			k2 = srodek.poz;
			k1 = koniec.poz;
			n = n + q;
			m = m + q;
			spraw(&m, &n);
			srodek.poz = pp + ((3.14 * n) / 180);
			koniec.poz = ((3.14 * q) / 180) + koniec.poz;
			spr(&koniec, &srodek);
			srodek.x = 500 + cos(srodek.poz) * R;
			srodek.y = 600 + sin(srodek.poz) * R;
			koniec.x = srodek.x + cos(koniec.poz) * R;
			koniec.y = srodek.y + sin(koniec.poz) * R;
			if (srodek.y >= 600 && ((k2 < 6.28 && k2>4.71) || k2 == 0)) {
				koniec.poz = k1 + 6.28 - k2;
				srodek.poz = 0;
				spr(&koniec, &srodek);
				m = m - q + (((6.28 - k2) * 180) / 3.14);
				n = n - q + (((6.28 - k2) * 180) / 3.14);
				spraw(&m, &n);
				srodek.x = 500 + cos(srodek.poz) * R;
				koniec.x = srodek.x + cos(koniec.poz) * R;
				srodek.y = 600;
				koniec.y = srodek.y + sin(koniec.poz) * R;
			}
			else if (koniec.y > 600) {
				srodek.poz = 3.14 - ((k1 - k2) / 2);
				koniec.poz = k1 + (srodek.poz - k2);
				spr(&koniec, &srodek);
				m = m - q + (((srodek.poz - k2) * 180) / 3.14);
				n = n - q + (((srodek.poz - k2) * 180) / 3.14);
				spraw(&m, &n);
				srodek.x = 500 + cos(srodek.poz) * R;
				koniec.x = srodek.x + cos(koniec.poz) * R;
				koniec.y = 600;
				srodek.y = 600 + sin(srodek.poz) * R;
			}
			if (grabb1 == true || grabb2 == true || grabb3 == true || grabb4 == true || grabb5 == true || grabb6 == true) {
				grab();
				if (obiekt1.y + 30 > 600) {
					obiekt1.y = 570;
					koniec.y = obiekt1.y - roznicaY;
				}
				if (obiekt2.y + 30 > 600) {
					obiekt2.y = 570;
					koniec.y = obiekt2.y - roznicaY;
				}
				if (obiekt3.y + 30 > 600) {
					obiekt3.y = 570;
					koniec.y = obiekt3.y - roznicaY;
				}
				if (obiekt4.y + 30 > 600) {
					obiekt4.y = 570;
					koniec.y = obiekt4.y - roznicaY;
				}
				if (obiekt5.y + 30 > 600) {
					obiekt5.y = 570;
					koniec.y = obiekt5.y - roznicaY;
				}
				if (obiekt6.y + 30 > 600) {
					obiekt6.y = 570;
					koniec.y = obiekt6.y - roznicaY;
				}
			}
			if (zapisuje == true) saving(&zapis, &srodek, &koniec, &obiekt1, &obiekt2, &obiekt3, &obiekt4, &obiekt5, &obiekt6);
			RepaintRobot(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON4:
			InvalidateRect(hWnd, &drawArea1, TRUE);
			float p1, p2;
			p1 = srodek.poz;
			p2 = koniec.poz;
			n = n - q;
			m = m - q;
			spraw(&m, &n);
			srodek.poz = pp + ((3.14 * n) / 180);
			koniec.poz = koniec.poz - ((3.14 * q) / 180);
			spr(&koniec, &srodek);
			srodek.x = 500 + cos(srodek.poz) * R;
			srodek.y = 600 + sin(srodek.poz) * R;
			koniec.x = srodek.x + cos(koniec.poz) * R;
			koniec.y = srodek.y + sin(koniec.poz) * R;
			if (srodek.y >=600 && ((p1 > 3.14 && p1 < 4.71)) || p1 == 3.14) {
				koniec.poz = p2 - p1 + 3.14;
				srodek.poz = 3.14;
				spr(&koniec, &srodek);
				m = m + q - (((p1 - 3.14) * 180) / 3.14);
				n = n + q - (((p1 - 3.14) * 180) / 3.14);
				spraw(&m, &n);
				srodek.x = 500 + cos(srodek.poz) * R;
				koniec.x = srodek.x + cos(koniec.poz) * R;
				srodek.y = 600;
				koniec.y = srodek.y + sin(koniec.poz) * R;
			}
			else if (koniec.y > 600) {
				srodek.poz = 3.14 + ((p1 - p2) / 2);
				koniec.poz = p2 - ((p1 + p2) / 2) + 3.14;
				spr(&koniec, &srodek);
				m = m + q - (((((p1 + p2) / 2) - 3.14) * 180) / 3.14);
				n = n + q - (((((p1 + p2) / 2) - 3.14) * 180) / 3.14);
				spraw(&m, &n);
				srodek.x = 500 + cos(srodek.poz) * R;
				koniec.x = srodek.x + cos(koniec.poz) * R;
				koniec.y = 600;
				srodek.y = 600 + sin(srodek.poz) * R;
			}
			if (grabb1 == true || grabb2 == true || grabb3 == true || grabb4 == true || grabb5 == true || grabb6 == true) grab();
			if (zapisuje == true) saving(&zapis, &srodek, &koniec, &obiekt1, &obiekt2, &obiekt3, &obiekt4, &obiekt5, &obiekt6);
			RepaintRobot(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON6:
			q -= 2;
			if (q <= 0) q = 1;
			break;
		case ID_BUTTON5:
			q += 2;
			if (q >= 30) q = 28;
			break;
		case ID_BUTTON7:
			InvalidateRect(hWnd, &drawArea1, TRUE);
			n = 0;
			m = 0;
			srodek.x = 660;
			srodek.y = 480;
			koniec.x = 820;
			koniec.y = 360;
			srodek.poz = pp;
			koniec.poz = pp;
			if (zapisuje == true) saving(&zapis, &srodek, &koniec, &obiekt1, &obiekt2, &obiekt3, &obiekt4, &obiekt5, &obiekt6);
			RepaintRobot(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON8:
			zapisuje = true;
			saving(&zapis, &srodek, &koniec, &obiekt1, &obiekt2, &obiekt3, &obiekt4, &obiekt5, &obiekt6);
			break;
		case ID_BUTTON9:
			zapisuje = false;
			break;
		case ID_BUTTON10:
			id = ID_BUTTON10;
			if (zapisuje == false) SetTimer(hWnd, TMR_1, 120, NULL);
			break;
		case ID_BUTTON11:
			 
			if (obj1 == true && obiekt1.x<=koniec.x && koniec.x<=obiekt1.x+30 && obiekt1.y <= koniec.y && koniec.y <= obiekt1.y + 30) {
				grabb1 = true;
				roznicaX = obiekt1.x - koniec.x;
				roznicaY = obiekt1.y - koniec.y;
			}
			if (obj1 == true && obiekt2.x <= koniec.x && koniec.x <= obiekt2.x + 30 && obiekt2.y <= koniec.y && koniec.y <= obiekt2.y + 30) {
				grabb2 = true;
				roznicaX = obiekt2.x - koniec.x;
				roznicaY = obiekt2.y - koniec.y;
			}
			if (obj1 == true && obiekt3.x <= koniec.x && koniec.x <= obiekt3.x + 30 && obiekt3.y <= koniec.y && koniec.y <= obiekt3.y + 30) {
				grabb3 = true;
				roznicaX = obiekt3.x - koniec.x;
				roznicaY = obiekt3.y - koniec.y;
			}
			if (obj1 == true && obiekt4.x <= koniec.x && koniec.x <= obiekt4.x + 30 && obiekt4.y <= koniec.y && koniec.y <= obiekt4.y + 30) {
				grabb4 = true;
				roznicaX = obiekt4.x - koniec.x;
				roznicaY = obiekt4.y - koniec.y;
			}
			if (obj1 == true && obiekt5.x <= koniec.x && koniec.x <= obiekt5.x + 30 && obiekt5.y <= koniec.y && koniec.y <= obiekt5.y + 30) {
				grabb5 = true;
				roznicaX = obiekt5.x - koniec.x;
				roznicaY = obiekt5.y - koniec.y;
			}
			if (obj1 == true && obiekt6.x <= koniec.x && koniec.x <= obiekt6.x + 30 && obiekt6.y <= koniec.y && koniec.y <= obiekt6.y + 30) {
				grabb6 = true;
				roznicaX = obiekt6.x - koniec.x;
				roznicaY = obiekt6.y - koniec.y;
			}
			break;
		case ID_BUTTON12:
			if (grabb1 == true || grabb2 == true || grabb3 == true || grabb4 == true || grabb5 == true || grabb6 == true) {
				id = ID_BUTTON12;
				dropping = true;
				SetTimer(hWnd, TMR_1, 35, NULL);
			}
			break;
		case ID_BUTTON13:
			InvalidateRect(hWnd, &drawArea1, TRUE);
			obj = true;
			obj1 = true;
			RepaintRobot(hWnd, hdc, ps, &drawArea1);
			obj = false;
			break;
		case ID_BUTTON14:
			if(id!= ID_BUTTON10)dlt();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		StartPaintRobot(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			if (id == ID_BUTTON12) {
				int t2;
				t2 = drop();
				if (t2 == 1) {
					InvalidateRect(hWnd, &drawArea1, TRUE);
					if (zapisuje == true) saving(&zapis, &srodek, &koniec, &obiekt1, &obiekt2, &obiekt3, &obiekt4, &obiekt5, &obiekt6);
					RepaintRobot(hWnd, hdc, ps, &drawArea1);
				}
				if (t2 == 0) {
					KillTimer(hWnd, TMR_1);
					id = 0;
					InvalidateRect(hWnd, &drawArea1, TRUE);
					if (zapisuje == true) saving(&zapis, &srodek, &koniec, &obiekt1, &obiekt2, &obiekt3, &obiekt4, &obiekt5, &obiekt6);
					dropping = false;
					RepaintRobot(hWnd, hdc, ps, &drawArea1);
					if (grabb1 == true)  grabb1 = false;
					if (grabb2 == true) grabb2 = false;
					if (grabb3 == true) grabb3 = false;
					if (grabb4 == true) grabb4 = false;
					if (grabb5 == true) grabb5 = false;
					if (grabb6 == true) grabb6 = false;
				}
			}
			if (id == ID_BUTTON10) {
				int t1;
				t1 = play(&zapis, hWnd);
				if (t1 == 1)RepaintRobot(hWnd, hdc, ps, &drawArea1);
				if (t1 == 0) {
					RepaintRobot(hWnd, hdc, ps, &drawArea1);
					KillTimer(hWnd, TMR_1);
					id = 0;
				}
			}
			break;
		}


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}