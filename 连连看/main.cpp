//�������岿�ִ���

#include"header.h"
//������Դ
#define ID_TIMER 1
#define IDE_EDIT1 40003
#define IDE_EDIT2 40004
////////////////////////////////////////////////////

HWND		g_edAccount;		//�û��༭����
HWND		g_edPassword;		//����༭����
HINSTANCE	g_hInstance;		//ȫ��ʵ������
DRAWWINDOW	*drawWnd;			//���ڻ�����
CHESSRAND	chessRand;			//���Ӳ�����
DBCONNECTOR	dbConnector;		//���ݿ������

int			modeFlag = 0;		//�����ڽ����ʶ,0Ϊ������,1Ϊ��Ϸ����,2Ϊ��������,3Ϊ���ڽ���,4Ϊʧ�ܽ���,5Ϊ�ɹ�ͨ�ؽ���
int			limitTime = 180;	//��Ϸ����ʱ��
int			score = 0;			//�÷�
int			clickTime = 0;		//�������
bool		isRegister = false;	//ȷ�ϵ����Ƿ�ɹ�
int			prevX, prevY, prevV, curX, curY, curV;

//��ģ���ļ������к�������
LRESULT CALLBACK RegisProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//ע�ᴰ����Ϣ������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);				//��������Ϣ������
HRESULT InitWindow(HINSTANCE hInstance, int cmdShow);											//��ʼ��������
HRESULT InitRegis(HINSTANCE hInstance, int cmdShow);											//��ʼ��ע�ᴰ��										
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);	//������ں���
//////////////////////////////////////////////////////////////////////////////////////////////

HRESULT InitRegis(HINSTANCE hInstance, int cmdShow)
{
	HWND		hWnd;
	WNDCLASSEX	wc;
	g_hInstance = hInstance;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursorFromFile(TEXT("cursor.ani"));
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = RegisProc;
	wc.lpszClassName = TEXT("regis");
	wc.lpszMenuName = NULL;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("����ע����ʧ��"), NULL, 0);
		return E_FAIL;
	}
	
	hWnd = CreateWindow(TEXT("regis"), TEXT("����ȵ���da��ze~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 350, NULL, NULL, hInstance, NULL);
	g_edAccount=CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD |ES_LEFT ,
		200, 55, 100, 20, hWnd, (HMENU)IDE_EDIT1, hInstance, NULL);
	g_edPassword=CreateWindow(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_LEFT|ES_PASSWORD,
		200, 135, 100, 20, hWnd, (HMENU)IDE_EDIT2, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), NULL, 0);
		return E_FAIL;
	}
	SetWindowPos(hWnd, NULL, 0, 0, 500, 350, SWP_NOMOVE);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return S_OK;
}

LRESULT CALLBACK RegisProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	PAINTSTRUCT		ps;
	switch (message)
	{
	case WM_CREATE:
	{
		HDC hdc1 = GetDC(hWnd);
		drawWnd = new DRAWWINDOW(hdc1, g_hInstance);
	}
	break;
	
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		drawWnd->DrawInterface(L"res\\interface\\login.bmp");
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		hdc = GetDC(hWnd);
		if (x > 225 && x < 275 && y>210 && y < 240)
		{
			wchar_t			acc[32];
			wchar_t			pwd[32];
			SendMessage(g_edAccount, WM_GETTEXT, 32, (LPARAM)acc);
			SendMessage(g_edAccount, WM_SETTEXT, NULL, (LPARAM)acc);

			SendMessage(g_edPassword, WM_GETTEXT, 32, (LPARAM)pwd);
			SendMessage(g_edPassword, WM_SETTEXT, NULL, (LPARAM)pwd);
			GetWindowText(g_edPassword, pwd, 32);

			if (dbConnector.IsLogin(acc, pwd))
			{
				isRegister = true;
				MessageBox(NULL, L"����ɹ���~", L"ok da��ze", 0);
				SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			}
			else
			{
				MessageBox(NULL, L"�˻����������������������", L"fail daze.", 0);
			}
			break;
		}
		if (x > 225 && x < 275 && y>270 && y < 300)
		{
			wchar_t			acc[32];
			wchar_t			pwd[32];
			SendMessage(g_edAccount, WM_GETTEXT, 32, (LPARAM)acc);
			SendMessage(g_edAccount, WM_SETTEXT, NULL, (LPARAM)acc);

			SendMessage(g_edPassword, WM_GETTEXT, 32, (LPARAM)pwd);
			
			SendMessage(g_edPassword, WM_SETTEXT, NULL, (LPARAM)pwd);
			if (dbConnector.RunRegister(acc, pwd))
			{
				MessageBox(NULL, L"ע��ɹ���~", L"ok da��ze", 0);
				wcscat(pwd, L"");
			}
			else
			{
				MessageBox(NULL, L"ע��ʧ���˵�˵������", L"error ze", 0);
				wcscat(pwd, L"");
			}
		}
			break;
	}

	case WM_DESTROY:
	{
		if (!isRegister)
		{
			PostQuitMessage(0);
		}
		else
		{
			DestroyWindow(hWnd);
		}
		break;
	}

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	if(!isRegister)
		if (FAILED(InitRegis(hInstance, cmdShow)))
			return 0;

	if(isRegister)
		if (FAILED(InitWindow(hInstance, cmdShow)))
			return 0;

	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)
	{
		if (isRegister)
		{
			isRegister = false;
			if (FAILED(InitWindow(hInstance, cmdShow)))
				return 0;
		}

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
		MessageBox(NULL, TEXT("����ע����ʧ��"), NULL, 0);
		return E_FAIL;
	}

	hWnd = CreateWindow(TEXT("main frame"), TEXT("���ɰ�����������~"), WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), NULL, 0);
		return E_FAIL;
	}
	SetWindowPos(hWnd, NULL, 0, 0, 800, 600, SWP_NOMOVE);
	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
			if (modeFlag == 0 && x > 250 && x < 550 && y>270 && y < 330)//��ʼ��Ϸ
			{

				modeFlag = 1;
				limitTime = 180;
				chessRand.Clear();

				chessRand.GetRandPic();

				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
				SetTimer(hWnd, ID_TIMER, 1000, NULL);
				break;
			}
			if (modeFlag == 1 && x > 100 && x < 660 && y>200 && y < 520)//��Ϸ��������
			{
				int temCol = (x - 100) / 40+1;
				int temRow = (y - 200) / 40+1;
				clickTime++;
				if (clickTime % 2==0)//�����Ϊ˫��ʱ�Ž��������ж�
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
			if (modeFlag == 1 && x > 680 && x < 780 && y>200 && y < 250)//��Ϸ������������
			{
				chessRand.Clear();
				chessRand.GetRandPic();
				limitTime = 180;
				score = 0;
				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			}
			if(modeFlag == 1 && x > 680 && x < 780 && y>300 && y < 350)//��Ϸ����������ʾ
			{
				drawWnd->Tip();
			}
			if (modeFlag == 0 && x > 250 && x < 550 && y>355 && y < 415)//����
			{
				modeFlag = 2;
				drawWnd->DrawInterface(TEXT("res\\interface\\help.bmp"));
			}
	
			if (modeFlag == 0 && x > 250 && x < 550 && y>440 && y < 500)//����
			{
				modeFlag = 3;
				drawWnd->DrawInterface(TEXT("res\\interface\\about.bmp"));
			}
			if (modeFlag == 4)											//ʧ������
			{
				modeFlag = 1;
				chessRand.Clear();
				chessRand.GetRandPic();
				limitTime = 180;
				score = 0;
				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			}
			if (modeFlag == 5)											//ͨ������
			{
				modeFlag = 1;
				chessRand.Clear();
				chessRand.GetRandPic();
				limitTime = 180;
				score = 0;
				drawWnd->GetPic(chessRand.m_ChessId);
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			}
			if (modeFlag != 0 && x > 100 && x < 200 && y<150 && y > 100)//����
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
			if (modeFlag == 1 &&limitTime == 0)
			{
				modeFlag = 4;
				drawWnd->DrawInterface(TEXT("res\\interface\\fail.bmp"));
			}
			if (modeFlag == 1)
			{
				
				drawWnd->DrawGameMap(hWnd, limitTime, score);
			}
				
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