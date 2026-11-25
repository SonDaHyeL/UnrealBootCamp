#ifndef GAME_H
#define GAME_H

#include "UI.h"
#include "Player.h"

class Game {
private: 
	enum class Scene {
		START,
		TOWN,
		HUNTING,
		STORE,
		QUIT,
	};

	//***메인 메뉴***
	void LoadOrCreate(); //이어하기, 새로 시작
	void MainMenu();
	void Save();

	//***모험(씬)***
	void Adventure();
	Scene Start();
	Scene Town();
	Scene Store();
	Scene Hunting();

	//***그 외 세부 메뉴***
	void TalkToChief();
	void BuyMenu();
	void SellMenu();
	void InvenMenu();

	//게임이 플레이어를 소유함
	Player player;

public:
	//게임 시작 > 메인 메뉴 > 종료까지 제어
	void Run();
};


#endif // !GAME_H