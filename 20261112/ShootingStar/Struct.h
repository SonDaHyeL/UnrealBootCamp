#pragma once
#include <conio.h>
#include <Windows.h>

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


