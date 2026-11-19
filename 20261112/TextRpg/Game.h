#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Object.h"

enum class Scene {
	MENU,
	START,
	TOWN,
	HUNTING,
	STORE,
	QUIT,
};

int Select();
void StartGame(Player& player);
Scene Town(Player& player);
Scene Store(Player& player);
Scene Hunting(Player& player);


#endif // !GAME_H