#include"drawWindow.h"
#include<atlimage.h>


int			g_curve, g_cx, g_cy, g_x1, g_x2, g_y1, g_y2;
const int	dir[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
bool		vis[10][16];

DRAWWINDOW::DRAWWINDOW(HDC &hdc, HINSTANCE &hInstance) :m_hdc(hdc), m_hInstance(hInstance)
{
	m_picPath[0] = TEXT("res\\chess\\1.bmp");
	m_picPath[1] = TEXT("res\\chess\\2.bmp");
	m_picPath[2] = TEXT("res\\chess\\3.bmp");
	m_picPath[3] = TEXT("res\\chess\\4.bmp");
	m_picPath[4] = TEXT("res\\chess\\5.bmp");
	m_picPath[5] = TEXT("res\\chess\\6.bmp");
	m_picPath[6] = TEXT("res\\chess\\7.bmp");
	m_picPath[7] = TEXT("res\\chess\\8.bmp");
	m_picPath[8] = TEXT("res\\chess\\9.bmp");
	m_picPath[9] = TEXT("res\\chess\\10.bmp");
	m_picPath[10] = TEXT("res\\chess\\11.bmp");
	m_picPath[11] = TEXT("res\\chess\\12.bmp");
	m_picPath[12] = TEXT("res\\chess\\13.bmp");
	m_picPath[13] = TEXT("res\\chess\\14.bmp");
	m_picPath[14] = TEXT("res\\chess\\15.bmp");
	m_picPath[15] = TEXT("res\\chess\\16.bmp");
	m_picPath[16] = TEXT("res\\chess\\17.bmp");
	m_picPath[17] = TEXT("res\\chess\\18.bmp");
	m_picPath[18] = TEXT("res\\chess\\19.bmp");
	m_picPath[19] = TEXT("res\\chess\\20.bmp");
	m_picPath[20] = TEXT("res\\chess\\21.bmp");
	m_picPath[21] = TEXT("res\\chess\\22.bmp");
	m_picPath[22] = TEXT("res\\chess\\23.bmp");

	m_picSelectedPath[0] = TEXT("res\\chess\\31.bmp");
	m_picSelectedPath[1] = TEXT("res\\chess\\32.bmp");
	m_picSelectedPath[2] = TEXT("res\\chess\\33.bmp");
	m_picSelectedPath[3] = TEXT("res\\chess\\34.bmp");
	m_picSelectedPath[4] = TEXT("res\\chess\\35.bmp");
	m_picSelectedPath[5] = TEXT("res\\chess\\36.bmp");
	m_picSelectedPath[6] = TEXT("res\\chess\\37.bmp");
	m_picSelectedPath[7] = TEXT("res\\chess\\38.bmp");
	m_picSelectedPath[8] = TEXT("res\\chess\\39.bmp");
	m_picSelectedPath[9] = TEXT("res\\chess\\40.bmp");
	m_picSelectedPath[10] = TEXT("res\\chess\\41.bmp");
	m_picSelectedPath[11] = TEXT("res\\chess\\42.bmp");
	m_picSelectedPath[12] = TEXT("res\\chess\\43.bmp");
	m_picSelectedPath[13] = TEXT("res\\chess\\44.bmp");
	m_picSelectedPath[14] = TEXT("res\\chess\\45.bmp");
	m_picSelectedPath[15] = TEXT("res\\chess\\46.bmp");
	m_picSelectedPath[16] = TEXT("res\\chess\\47.bmp");
	m_picSelectedPath[17] = TEXT("res\\chess\\48.bmp");
	m_picSelectedPath[18] = TEXT("res\\chess\\49.bmp");
	m_picSelectedPath[19] = TEXT("res\\chess\\50.bmp");
	m_picSelectedPath[20] = TEXT("res\\chess\\51.bmp");
	m_picSelectedPath[21] = TEXT("res\\chess\\52.bmp");
	m_picSelectedPath[22] = TEXT("res\\chess\\53.bmp");
	memset(m_picMap, 0, sizeof(m_picMap));
}

DRAWWINDOW::DRAWWINDOW()
{

}

DRAWWINDOW::~DRAWWINDOW()
{
	DeleteDC(m_hdc);
}

void DRAWWINDOW::DrawGameMap(HWND hWnd, int timeLength, int score)//绘制游戏界面
{
	HDC hdc;
	hdc = GetDC(hWnd);
	HDC bgDC = CreateCompatibleDC(m_hdc);
	RECT rect;
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(bgDC, hBrush);
	GetClientRect(hWnd, &rect);
	Rectangle(bgDC, rect.left, rect.top, rect.right, rect.bottom);
	HBITMAP allBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(bgDC, allBitmap);
	CImage image;
	image.Load(TEXT("res\\interface\\game.bmp"));
	image.Draw(bgDC, 0, 0, image.GetWidth(), image.GetHeight());
	image.Destroy();

	
	if (timeLength >= 20)
		hBrush = CreateSolidBrush(RGB(0, 128, 0));
	else
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(bgDC, hBrush);
	Rectangle(bgDC, 100, 20, 100 + timeLength * 3, 50);//绘制时间条
	
	for (int iRow = 1; iRow<9; iRow++)
	{
		for (int iCol = 1; iCol<15; iCol++)
		{
			if (m_picMap[iRow][iCol] != 0)
			{
				HDC memDC = CreateCompatibleDC(m_hdc);
				const TCHAR *p = m_picPath[m_picMap[iRow][iCol] - 1];
				HBITMAP hh = (HBITMAP)LoadImage(m_hInstance, p, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				SelectObject(memDC, hh);

				CImage image;
				image.Load(p);
				image.Draw(memDC, 0, 0, image.GetWidth(), image.GetHeight());

				BitBlt(bgDC, 60 + iCol * 40, 160 + iRow * 40, 40, 40, memDC, 0, 0, SRCCOPY);
				DeleteObject(hh);
				DeleteDC(memDC);
			}
		}
	}

	int tscore = score;
	int tem = int(tscore / 200);
	if (tscore >= 1200)
		tem = 0;
	BitBlt(m_hdc, 0, 0, 900, 600, bgDC, 0, 0, SRCCOPY);

	DeleteObject(allBitmap);
//	DeleteObject(hBrush);
	DeleteDC(bgDC);
}

void DRAWWINDOW::DrawSelected(int x, int y, int val, bool mark)//绘制选中的方块
{
	if (mark && val != 0)
	{
		HDC memDC = CreateCompatibleDC(m_hdc);
		HBITMAP hh = (HBITMAP)LoadImage(m_hInstance, m_picSelectedPath[val - 1], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(memDC, hh);
		BitBlt(m_hdc, 60 + 40 * x - 10, 160 + 40 * y - 10, 60, 60, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
	}

	if (mark == 0 && val != 0)
	{
		HDC memDC = CreateCompatibleDC(m_hdc);
		const TCHAR *p = m_picPath[val - 1];
		HBITMAP hh = (HBITMAP)LoadImage(m_hInstance, p, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(memDC, hh);
		BitBlt(m_hdc, 60 + 40 * x, 160 + 40 * y, 40, 40, memDC, 0, 0, SRCCOPY);
		DeleteDC(memDC);
	}
}

void DRAWWINDOW::DrawInterface(const TCHAR* picPath)//绘制普通界面
{
	HDC bgDC = CreateCompatibleDC(m_hdc);
	BITMAP temBmp;
	HBITMAP hStartBmp = (HBITMAP)LoadImage(m_hInstance, picPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	::GetObject(hStartBmp, sizeof(temBmp), &temBmp);
	SelectObject(bgDC, hStartBmp);
	BitBlt(m_hdc, 0, 0, temBmp.bmWidth, temBmp.bmHeight, bgDC, 0, 0, SRCCOPY);
	DeleteDC(bgDC);
}

void DRAWWINDOW::GetPic(int val[10][16])
{
	for (int row = 1; row<9; row++)
		for (int column = 1; column<15; column++)
			m_picMap[row][column] = val[row][column];
}

bool DRAWWINDOW::Tip()//得到提示
{
	for (int cur_row = 1; cur_row<9; cur_row++)
	{
		for (int cur_column = 1; cur_column<15; cur_column++)
		{
			for (int row = 1; row<9; row++)
			{
				for (int column = 1; column<15; column++)
				{
					if (m_picMap[cur_row][cur_column] != 0 && m_picMap[row][column] != 0 && (cur_column != column || cur_row != row))
					{
						bool flag = IfDelete(cur_column, cur_row, m_picMap[cur_row][cur_column], column, row, m_picMap[row][column]);
						if (flag)
						{
							HPEN pen = CreatePen(PS_SOLID, 25, RGB(0, 0, 0));
							SelectObject(m_hdc, pen);
							MoveToEx(m_hdc, 60 + 40 * cur_column + 18, 160 + 40 * cur_row + 18, NULL);
							LineTo(m_hdc, 60 + 40 * cur_column + 22, 160 + 40 * cur_row + 22);
							MoveToEx(m_hdc, 60 + 40 * column + 18, 160 + 40 * row + 18, NULL);
							LineTo(m_hdc, 60 + 40 * column + 22, 160 + 40 * row + 22);
							DeleteObject(pen);
							return true;
						}
						else
							continue;
					}
				}
			}
		}
	}
	return false;
}

bool DRAWWINDOW::IfDelete(int x1, int y1, int v1, int x2, int y2, int v2)//消除算法的预处理部分
{
	if (v1 != v2)
		return false;
	if (x1 == x2&&y1 == y2)
		return false;
	g_cx = x1;
	g_cy = y1;
	g_x1 = x1;
	g_y1 = y1;
	g_x2 = x2;
	g_y2 = y2;
	g_curve = 0;
	memset(vis, false, sizeof(vis));
	vis[y1][x1] = true;
	if (sol(g_x1, g_y1))
		return true;
	else
		return false;
}

bool DRAWWINDOW::sol(int x, int y)//消除算法主体部分
{
	if (x == g_x2 && y == g_y2 && g_curve <= 2)
		return true;
	if (g_curve>2)
		return false;
	for (int i = 0; i != 4; i++)
	{
		int nx = x + dir[i][0];
		int ny = y + dir[i][1];

		if (nx>16 || ny>10 || nx < 0 || ny < 0)
			continue;
		if (m_picMap[ny][nx] != 0 || vis[ny][nx])
			if (nx != g_x2 || ny != g_y2)
				continue;
		vis[ny][nx] = true;
		if (nx != g_cx&&ny != g_cy)
		{
			g_curve++;
			int temx = g_cx;
			int temy = g_cy;
			g_cx = x;
			g_cy = y;
			if (sol(nx, ny))
				return true;
			g_curve--;
			g_cx = temx;
			g_cy = temy;
			vis[ny][nx] = false;
		}
		else
		{
			if (sol(nx, ny))
				return true;
			vis[ny][nx] = false;
		}
	}
	return false;
}

/*
void DRAWWINDOW::DrawTimeBar(HWND hWnd, int timeLength)
{
HDC hdc;
hdc = GetDC(hWnd);
RECT rect;
GetClientRect(hWnd, &rect);
HDC bgDC = CreateCompatibleDC(m_hdc);
HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
if (timeLength >= 20)
hBrush = CreateSolidBrush(RGB(0, 128, 0));
else
hBrush = CreateSolidBrush(RGB(255, 0, 0));
SelectObject(bgDC, hBrush);
Rectangle(bgDC, 100, 20, 100 + timeLength * 3, 50);//绘制时间条
DeleteDC(bgDC);
DeleteObject(hBrush);
}
*/
