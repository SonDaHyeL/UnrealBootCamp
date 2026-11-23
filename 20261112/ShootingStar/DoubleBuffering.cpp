#include "DoubleBuffering.h"
#include <Windows.h>
static int g_ScreenIndex = 0; //출력할 종이번호
static HANDLE g_hScreen[2];//종이 갯수
//화면 초기 세팅
void ScreenInit()
{
	g_hScreen[0] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);

	g_hScreen[1] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);


	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(g_hScreen[0], &cci);
	SetConsoleCursorInfo(g_hScreen[1], &cci);
}

void ScreenPrint(Tile(*_BG)[DF_BG_SIZEX])
{
	DWORD dw = 0;
	COORD Pos = { 0,0 }; //덩어리 시작 위치
	unsigned short TempColor = _BG[0][0].Color; //덩어리 색상
	char TempBG[(DF_BG_SIZEX * DF_SCREEN_SIZEY) + 1] = {};
	int i = 0;

	for (int y = 0; y < DF_SCREEN_SIZEY; ++y)
	{
		for (int x = 0; x < DF_BG_SIZEX; ++x)
		{
			//해당 타일의 색상이 기본 색상과 다르면? (색상이 바뀌었으면?)
			if (TempColor != _BG[y][x].Color) {
				SetConsoleCursorPosition(g_hScreen[g_ScreenIndex], Pos);
				SetConsoleTextAttribute(g_hScreen[g_ScreenIndex], TempColor);
				WriteFile(g_hScreen[g_ScreenIndex], TempBG, i, &dw, NULL);

				//memset(TempBG, 0, sizeof(TempBG)); //메모리 내 값 전부 0으로 초기화 하는 것. 해당 메모리 시작주소부터 크기까지 0으로 채움
				
				i = 0;
				//출발 시작점 변경
				Pos.X = (SHORT)x;
				Pos.Y = (SHORT)y;
				TempColor = _BG[y][x].Color;
			}
			TempBG[i++] = _BG[y][x].Text;
		}		
	}


	if (i != 0)
	{
		SetConsoleCursorPosition(g_hScreen[g_ScreenIndex], Pos);
		SetConsoleTextAttribute(g_hScreen[g_ScreenIndex], TempColor);
		WriteFile(g_hScreen[g_ScreenIndex], TempBG, i, &dw, NULL);
	}
}

void ScreenClear()
{
	DWORD dw = 0;
	FillConsoleOutputCharacter(
		g_hScreen[g_ScreenIndex], ' ',
		DF_BG_SIZEX * DF_SCREEN_SIZEY,
		COORD(), &dw);
}
void ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(g_hScreen[g_ScreenIndex]);
	//g_ScreenIndex = (g_ScreenIndex == 1) ? 0 : 1;
	g_ScreenIndex = !g_ScreenIndex;
}

void ScreenRelease()
{
	CloseHandle(g_hScreen[0]);
	CloseHandle(g_hScreen[1]);

}