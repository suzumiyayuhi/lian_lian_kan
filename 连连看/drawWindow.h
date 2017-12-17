#pragma once
#include<Windows.h>
struct DRAWWINDOW
{
	
	//成员变量声明
	const TCHAR		*m_picPath[24];			//储存棋子图片路径
	const TCHAR		*m_picSelectedPath[24];	//储存被点击中的棋子图片路径
	int				m_picMap[10][16];		//储存棋子分布
	HDC				m_hdc;					//上下文句柄
	HINSTANCE		m_hInstance;			//窗口实例
	////////////////////////////////////////////////////////

	//成员函数声明
	DRAWWINDOW(HDC &hdc,HINSTANCE &hInstance );						//构造函数
	DRAWWINDOW();													//构造函数
	~DRAWWINDOW();													//析构函数
	void DrawGameMap(HWND hWnd,int timeLength,int score);			//绘制游戏界面的一些功能性组件
	void DrawSelected(int x, int y, int val, bool mark);			//绘制棋子选中效果
	void DrawInterface(const TCHAR *picPath);						//绘制界面
	void GetPic(int val[10][16]);									//随机分布棋子
	bool IfDelete(int x1, int y1, int v1, int x2, int y2, int v2);	//消去算法输入部分
	bool sol(int x, int y);											//消去算法输出部分
	bool Tip();														//使用提示
	////////////////////////////////////////////////////////
};