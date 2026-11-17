#ifndef GAME_H
#define GAME_H

#include "Player.h"

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
Scene Town(Player& palyer);
Scene Hunting(Player& player);


#endif // !GAME_H