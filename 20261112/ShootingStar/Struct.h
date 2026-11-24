#pragma once

#include <Windows.h>
#include <cstdlib> //rand 호출 용
#include <ctime> //rand 호출 용

//게임 화면 크기
#define DF_BG_SIZEX 120
#define DF_BG_SIZEY 40 

//게임 UI 화면 크기
#define DF_UI_SIZEY 9
#define DF_SCREEN_SIZEY (DF_BG_SIZEY + DF_UI_SIZEY)

//플레이어 최대 체력
#define PLAYER_MAXHP 3
//최대 발사 라인 수
#define MAX_SHOTLINE 5

struct Tile
{
	char Text;
	unsigned short Color;
};


enum Team {
	PTEAM,
	ETEAM
};

typedef struct Transform {
	COORD Position;
} Player;


struct Enemy {
	COORD Position;
	bool Active;
	ULONGLONG NextFireTick;
};

struct Bullet {
	float X, Y; //위치 좌표 값
	float vx, vy; //벡터값. (한 프레임에 움직이는 양. (방향 + 속도) 
	bool Active; //날아가는 중인지 확인하는 용도
	Team Owner;
};

//벡터 값을 정수좌표 값으로 변경
inline COORD BulletCoord(const Bullet& b) {
	return { (SHORT)b.X, (SHORT)b.Y };
}

struct Star {
	float X;
	SHORT Y;
	float Speed;
	char Text;
	unsigned short Color;
};

struct Effect {
	COORD Center;
	int Frame;
	bool Active;
};