#pragma once

#include <string>
#include <vector>

class Player;
class Enemy;

namespace UI {
	struct ListEntry {
		std::string label; // 패널 왼쪽 글자 (아이템 이름)
		std::string tag;   // 패널 오른쪽 끝 글자 (가격)
		std::vector<std::string> info; // 커서가 올라가면 선택지 상자에 보일 줄들
	};


	void Init(); //콘솔 크기 고정

	void SetTitle(const std::string& title);
	void Log(const std::string& text);
	void ClearLog();

	std::string AskText(const Player& player, const std::string& prompt);

	int Menu(const Player& player, const std::vector<std::string>& options, 
		int first = 1, const Enemy* enemy = nullptr);
	//Menu와 동작 방식 같음. 뒤로가기는 마지막 선택지(그만두기, 돌아가기 등)을 고르게 함
	int MenuBack(const Player& player, const std::vector<std::string>& options);
	
	// 오른쪽 위 패널(인벤토리/장비)에서 직접 칸을 고른다.
	//   위/아래: 이동, 좌/우: 페이지 넘기기(allowPage일 때), Enter: 선택, 뒤로가기: 닫기
	//   돌려주는 값: 고른 칸 번호(0부터), 닫으면 -1.  guide는 선택지 상자 첫 줄 안내문
	int SelectSlot(const Player& player, bool allowPage, const std::string& guide);
	bool IsEquipPage();          // SelectSlot에서 장비 페이지를 보고 있었는지
	void ResetSlotSelect();      // 인벤토리 페이지로 되돌리고 > 표시를 지운다 (선택을 끝낼 때)

	//선택 리스트 UI
	int SelectList(const Player& player, const std::string& panelTitle,
		const std::vector<ListEntry>& entries, const std::string& guide, int& cursor);
	
}