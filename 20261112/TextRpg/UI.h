#pragma once

#include <string>
#include <vector>

class Player;
class Enemy;

namespace UI {
	void Init(); //콘솔 크기 고정

	void SetTitle(const std::string& title);
	void Log(const std::string& text);
	void ClearLog();

	int Menu(const Player& player, const std::vector<std::string>& options, 
		int first = 1, const Enemy* enemy = nullptr);

	void SetEquipPage(bool on);

	std::string AskText(const Player& player, const std::string& prompt);
}