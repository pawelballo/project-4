
#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <queue>
#include <winapifamily.h>
#include <cmath>
#include <math.h>
#include <atlstr.h>
#include <sstream>
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <gdiplus.h>

#define MAX_LOADSTRING 100
#define TMR_1 1

using namespace Gdiplus;

struct pozycja {
	float x;
	float y;
	float poz;
};

struct col {
	pozycja middle;
	pozycja end;
};

const int R = 200;
int q = 4;
float m = 0;
float n = 0;
bool zapisuje = false;
std::queue<col> zapis;
pozycja srodek;
pozycja koniec;
HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			

HWND hwndButton;
HWND hwndText;


RECT drawArea1 = { 0, 200, 1000, 601 };

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void PaintRobot(HDC hdc) {
	Graphics graphics(hdc);
	SmoothingMode smoothingMode = SmoothingModeAntiAlias;
	graphics.SetSmoothingMode(smoothingMode);
	SolidBrush brush1(Color(255, 255, 0, 0));
	Pen pen2(Color(255, 0, 0, 255), 2.5);
	Pen pen5(Color(255, 0, 0, 0), 2.5);
	float i, j;
	i = 500;
	j = 600;
	graphics.DrawLine(&pen5, 0, 600, 1500, 600);
	graphics.DrawLine(&pen2, srodek.x, srodek.y, koniec.x, koniec.y);
	graphics.DrawLine(&pen2, i, j, srodek.x, srodek.y);
	i = 10;
	graphics.FillEllipse(&brush1, srodek.x - 5, srodek.y - 5, i, i);
	graphics.FillEllipse(&brush1, koniec.x - 5, koniec.y - 5, i, i);
	graphics.FillEllipse(&brush1, 495, 595, 10, 10);
}

void StartPaintRobot(HDC hdc) {
	srodek.x = 660;
	srodek.y = 480;
	koniec.x = 820;
	koniec.y = 360;
	srodek.poz= 2 * 3.14 - asin(0.6);
	koniec.poz= 2 * 3.14 - asin(0.6);
	PaintRobot(hdc);
}

void RepaintRobot(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	hdc = BeginPaint(hWnd, &ps);
	PaintRobot(hdc);
	EndPaint(hWnd, &ps);
}


int OnCreate(HWND window)
{
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
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON9,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("odwtorz"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		720, 0,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON10,
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

void saving(std::queue<col> *save, pozycja *middle, pozycja *end) {
	col tymczas;
	tymczas.middle.poz = middle->poz;
	tymczas.middle.x = middle->x;
	tymczas.middle.y = middle->y;
	tymczas.end.poz = end->poz;
	tymczas.end.x = end->x;
	tymczas.end.y = end->y;
	if (!(*save).empty()) {
		col r;
		r = (*save).back();
		if (r.end.x != tymczas.end.x && r.end.y != tymczas.end.y && r.end.poz != tymczas.end.poz && r.middle.x != tymczas.middle.x && r.middle.y != tymczas.middle.y && r.middle.poz != tymczas.middle.poz) (*save).push(tymczas);
	}
	if ((*save).empty()) (*save).push(tymczas);
}

void spraw(float *k1, float *k2) {
	while (*k1<0 || *k1>=360) {
		if (*k1 >= 360)*k1 = *k1 - 360;
		if (*k1 < 0)* k1 = *k1 + 360;
	}
	while (*k2 < 0 || *k2 >= 360) {
		if (*k2 >= 360)* k2 = *k2 - 360;
		if (*k2 < 0)* k2 = *k2 + 360;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	float pp= 2 * 3.14 - asin(0.6);
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
		case ID_BUTTON1 :
			InvalidateRect(hWnd, NULL, TRUE);
			m = m + q;
			spraw(&m, &n);
			float r1,r2;
			r1 = koniec.poz;
			r2 = srodek.poz;
			koniec.poz = pp + ((3.14 * m) / 180);
			spr(&koniec, &srodek);
			koniec.x = srodek.x + cos(koniec.poz) * R;
			koniec.y = srodek.y + sin(koniec.poz) * R;
			if (koniec.y > 600) {
				koniec.poz = 6.28-r2;
				spr(&koniec, &srodek);
				m = m - q + (((koniec.poz-r1)*180)/3.14);
				spraw(&m, &n);
				koniec.y = 600;
				koniec.x = srodek.x + cos(koniec.poz) * R;
			}
			if (zapisuje == true) saving(&zapis, &srodek, &koniec);
			RepaintRobot(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON2:
			InvalidateRect(hWnd, NULL, TRUE);
			m = m - q;
			spraw(&m, &n);
			float r3, r4;
			r3 = koniec.poz;
			r4 = srodek.poz;
			koniec.poz = pp + ((3.14 * m) / 180);
			koniec.x = srodek.x + cos(koniec.poz) * R;
			koniec.y = srodek.y + sin(koniec.poz) * R;
			spr(&koniec, &srodek);
			if (koniec.y > 600) {
				koniec.poz = 6.28 - r4;
				spr(&koniec, &srodek);
				m = m + q -  (((koniec.poz - r1) * 180) / 3.14);
				spraw(&m, &n);
				koniec.y = 600;
				koniec.x = srodek.x + cos(koniec.poz) * R;
			}
			if (zapisuje == true) saving(&zapis, &srodek, &koniec);
			RepaintRobot(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON3:
			InvalidateRect(hWnd, NULL, TRUE);
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
			if (srodek.y>=600 && ((k2<6.28 && k2>4.71)||k2==0)) {
				koniec.poz = k1 + 6.28 - k2;
				srodek.poz =0;
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
				koniec.poz = k1 + (srodek.poz-k2);
				spr(&koniec, &srodek);
				m = m - q + (((srodek.poz - k2) * 180) / 3.14);
				n = n - q + (((srodek.poz - k2) * 180) / 3.14);
				spraw(&m, &n);
				srodek.x = 500 + cos(srodek.poz) * R;
				koniec.x = srodek.x + cos(koniec.poz) * R;
				koniec.y = 600;
				srodek.y = 600 + sin(srodek.poz) * R;
			}
			if (zapisuje == true) saving(&zapis, &srodek, &koniec);
			RepaintRobot(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON4:
			InvalidateRect(hWnd, NULL, TRUE);
			float p1,p2;
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
			if (srodek.y >= 600 && ((p1 > 3.14 && p1<4.71))||p1==3.14) {
				koniec.poz = p2 - p1 + 3.14;
				srodek.poz = 3.14;
				spr(&koniec, &srodek);
				m = m +q-(((p1 - 3.14) * 180) / 3.14);
			    n = n+ q-(((p1 - 3.14) * 180) / 3.14); 
				spraw(&m, &n);
				srodek.x = 500 + cos(srodek.poz) * R;
				koniec.x = srodek.x + cos(koniec.poz) * R;
				srodek.y = 600;
				koniec.y = srodek.y + sin(koniec.poz) * R;
			}
			else if(koniec.y>600) {
				srodek.poz = 3.14 + ((p1 - p2) / 2);
				koniec.poz = p2 - ((p1 + p2) / 2) + 3.14;
				spr(&koniec, &srodek);
				m = m +q- (((((p1 + p2) / 2) - 3.14) * 180) / 3.14);
				n = n + q- (((((p1 + p2) / 2) - 3.14) * 180) / 3.14);
				spraw(&m, &n);
				srodek.x = 500 + cos(srodek.poz) * R;
				koniec.x = srodek.x + cos(koniec.poz) * R;
				koniec.y = 600;
				srodek.y = 600 + sin(srodek.poz)*R;
			}
			if (zapisuje == true) saving(&zapis, &srodek, &koniec);
			RepaintRobot(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON6:
			q-=2;
			if (q <=0) q = 1;
			break;
		case ID_BUTTON5:
			q+=2;
			if (q >= 30) q = 28;
			break;
		case ID_BUTTON7:
			InvalidateRect(hWnd, NULL, TRUE);
			n = 0;
			m = 0;
			srodek.x = 660;
			srodek.y = 480;
			koniec.x = 820;
			koniec.y = 360;
			srodek.poz =pp;
			koniec.poz =pp;
			if (zapisuje == true) saving(&zapis, &srodek, &koniec);
 			RepaintRobot(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON8:
			zapisuje = true;
			saving(&zapis, &srodek, &koniec);
			break;
		case ID_BUTTON9:
			zapisuje = false;
			break;
		case ID_BUTTON10:
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