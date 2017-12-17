#include"chessRand.h"
#include<time.h>
CHESSRAND::CHESSRAND()
{
	memset(m_ChessId, 0, sizeof(m_ChessId));
	GetRandPic();
}

CHESSRAND::~CHESSRAND()
{

}

void CHESSRAND::GetRandPic()
{
	srand((unsigned)time(NULL));
	for (int x = 1; x < 9; x++)
	{
		for (int y = 1; y < 15; y++)
		{
			int temRand = (rand() % 22)+1;
			if (m_ChessId[x][y] == 0)
			{
				m_ChessId[x][y] = temRand;
				int temX = rand() % 8+1;
				int temY = rand() % 14+1;
				if (m_ChessId[temX][temY])
				{
					while (m_ChessId[temX][temY] != 0)
					{
						temX = rand() % 8 + 1;
						temY = rand() % 14 + 1;
					}
					m_ChessId[temX][temY] = temRand;
				}
				else
					m_ChessId[temX][temY] = temRand;
			}
		}
	}
}

bool CHESSRAND::Clear()
{
	memset(m_ChessId, 0, sizeof(m_ChessId));
	return true;
}

bool CHESSRAND::IsEmpty()
{
	for (int x = 1; x < 9; x++)
	{
		for (int y = 1; y < 15; y++)
		{
			if (m_ChessId[x][y])
				return false;
		}
	}
	return true;
}