//程序主体部分代码

#include"header.h"
#define ID_TIMER 1

HINSTANCE	g_hInstance;		//全局实例对象
DRAWWINDOW	*drawWnd;
CHESSRAND	chessRand;
int			modeFlag = 0;		//界面标识,0为主界面,1为游戏界面,2为帮助界面,3为关于界面,4为失败界面,5为成功通关界面
int			limitTime = 180;	//游戏限制时间
int			score = 0;			//得分
int			clickTime = 0;		//点击次数
int			prevX, prevY, prevV, curX, curY, curV;


//该模块文件的所有函数声明
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//windows消息处理函数
HRESULT InitWindow(HINSTANCE hInstance, int cmdShow);											//创始化窗口
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);	//程序入口函数
//////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	if (FAILED(InitWindow(hInstance, cmdShow)))
		return 0;

	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

HRESULT InitWindow(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;
	WNDCLASSEX	wc;
	g_hInstance = hInstance;
	wc.cbClsExtra		= 0;
	wc.cbSize			= sizeof(wc);
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor			= LoadCursorFromFile(TEXT("cursor.ani"));
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hIconSm			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= TEXT("main frame");
	wc.lpszMenuName		= NULL;
	wc.style			= CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("窗口注册类失败"), NULL, 0);
		return E_FAIL;
	}

	hWnd = CreateWindow(TEXT("main frame"), TEXT("超可爱的连连看☆~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), NULL, 0);
		return E_FAIL;
	}
	SetWindowPos(hWnd, NULL, 0, 0, 800, 600, SWP_NOMOVE);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT			ps;
	HDC					hdc;
	switch (message)
	{
	case WM_CREATE:
		{
			HDC hdc1 = GetDC(hWnd);
			drawWnd = new DRAWWINDOW(hdc1 ,g_hInstance);
		}
		break;

	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			if (modeFlag == 0)
				drawWnd->DrawInterface(TEXT("res\\interface\\start.bmp"));
			if (modeFlag == 1)
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			if (modeFlag == 2)
				drawWnd->DrawInterface(TEXT("res\\interface\\help.bmp"));
			if (modeFlag == 3)
				drawWnd->DrawInterface(TEXT("res\\interface\\about.bmp"));
			EndPaint(hWnd, &ps);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			hdc = GetDC(hWnd);
			if (modeFlag == 0 && x > 250 && x < 550 && y>270 && y < 330)//开始游戏
			{

				modeFlag = 1;
				chessRand.Clear();

				chessRand.GetRandPic();

				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
				SetTimer(hWnd, ID_TIMER, 1000, NULL);
				break;
			}
			if (modeFlag == 1 && x > 100 && x < 660 && y>200 && y < 520)//游戏棋子区域
			{
				int temCol = (x - 100) / 40+1;
				int temRow = (y - 200) / 40+1;
				clickTime++;
				if (clickTime % 2==0)//点击数为双数时才进行消除判断
				{
					curX = temCol;
					curY = temRow;
					curV = drawWnd->m_picMap[curY][curX];
					bool sss = drawWnd->IfDelete(curX, curY, curV, prevX, prevY, prevV);
					
					if (sss)
					{
						drawWnd->m_picMap[curY][curX] = 0;
						drawWnd->m_picMap[prevY][prevX] = 0;
						chessRand.m_ChessId[curY][curX] = 0;
						chessRand.m_ChessId[prevY][prevX] = 0;
						drawWnd->DrawGameMap(hWnd, limitTime, score);
						if (chessRand.IsEmpty())
						{
							modeFlag = 5;
							drawWnd->DrawInterface(TEXT("res\\interface\\success.bmp"));
							break;
						}
					}
				}
				else
				{
					prevX = temCol;
					prevY = temRow;
					prevV = drawWnd->m_picMap[prevY][prevX];
	//				drawWnd->DrawSelected(prevX, prevY, prevV, false);
					drawWnd->DrawSelected(temCol, temRow, drawWnd->m_picMap[temRow][temCol],true);
				}
			}
			if (modeFlag == 1 && x > 680 && x < 780 && y>200 && y < 250)//游戏消除界面重来
			{
				chessRand.Clear();
				chessRand.GetRandPic();
				limitTime = 180;
				score = 0;
				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			}
			if(modeFlag == 1 && x > 680 && x < 780 && y>300 && y < 350)//游戏消除界面提示
			{
				drawWnd->Tip();
			}
			if (modeFlag == 0 && x > 250 && x < 550 && y>355 && y < 415)//帮助
			{
				modeFlag = 2;
				drawWnd->DrawInterface(TEXT("res\\interface\\help.bmp"));
			}
	
			if (modeFlag == 0 && x > 250 && x < 550 && y>440 && y < 500)//关于
			{
				modeFlag = 3;
				drawWnd->DrawInterface(TEXT("res\\interface\\about.bmp"));
			}
			if (modeFlag == 4)											//失败重来
			{
				modeFlag = 1;
				chessRand.Clear();
				chessRand.GetRandPic();
				limitTime = 180;
				score = 0;
				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			}
			if (modeFlag == 5)											//通关重来
			{
				modeFlag = 1;
				chessRand.Clear();
				chessRand.GetRandPic();
				limitTime = 180;
				score = 0;
				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			}
			if (modeFlag != 0 && x > 100 && x < 200 && y<150 && y > 100)//返回
			{
				modeFlag = 0;
				drawWnd->DrawInterface(TEXT("res\\interface\\start.bmp"));
			}
		}
		break;
	
	case WM_TIMER:
	{
		if (wParam == ID_TIMER)
		{
			limitTime--;
			if (limitTime == 0)
			{
				modeFlag = 4;
				drawWnd->DrawInterface(TEXT("res\\interface\\fail.bmp"));
			}
			if(modeFlag==1)
				drawWnd->DrawGameMap(hWnd, limitTime, score);
		}
	}
		break;

	case WM_DESTROY:
		{
			delete(drawWnd);
			PostQuitMessage(0);
		}
		break;

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}