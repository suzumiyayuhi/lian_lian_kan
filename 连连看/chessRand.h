#pragma once
#include"header.h"

struct CHESSRAND
{//成员变量
	int m_ChessId[10][16];					//存储棋子ID
	//成员函数
	CHESSRAND();
	~CHESSRAND();
	void GetRandPic();						//随机分配图片编号
	bool IsEmpty();							//判断是否全部消去
	bool Clear();							//清空所有位置
	//////////////////////////////////////////////////////////////////
	
};