#pragma once

#include <Windows.h>
#include <cstdlib> //rand 호출 용
#include <ctime> //rand 호출 용

#define DF_BG_SIZEX 200
#define DF_BG_SIZEY 40
struct Tile
{
	char Text;
	unsigned short Color;
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
};