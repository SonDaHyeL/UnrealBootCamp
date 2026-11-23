#pragma once

#include <Windows.h>
#include <cstdlib> //rand 호출 용
#include <ctime> //rand 호출 용

//게임 화면 크기
#define DF_BG_SIZEX 200
#define DF_BG_SIZEY 40 

//게임 UI 화면 크기
#define DF_UI_SIZEY 9
#define DF_SCREEN_SIZEY (DF_BG_SIZEY + DF_UI_SIZEY)

//플레이어 최대 체력
#define PLAYER_MAXHP 3

struct Tile
{
	char Text;
	unsigned short Color;
};

struct Vector {
	float X;
	float Y;
};

typedef struct Transform {
	COORD Position;
} Player;


struct Bullet {
	COORD Position;
	bool Active; //날아가는 중인지 확인하는 용도
};

struct Enemy {
	COORD Position;
	bool Active;
	ULONGLONG NextFireTick;
};