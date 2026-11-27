#define NOMINMAX	// windows.h의 min/max 매크로가 std::max와 충돌하지 않게
#include <windows.h>
#include <algorithm>
#include <iostream>
#include <conio.h> //_getch 사용. (키 하나를 Enter 없이 바로 읽음)
#include "UI.h"
#include "Player.h"
#include "Enemy.h"
#include "Quest.h"
#include "Sound.h"
using namespace std;

namespace {
	const int LEFT_W = 68;
	const int RIGHT_W = 30;
	const int TOP_H = 18;
	const int BOTTOM_H = 9;
	const char* HELP = " 방향키: 이동   Enter: 선택";
	const char* HELP_SLOT = " 위/아래: 이동  Enter: 선택  지우기: 닫기";
	const char* HELP_SLOT_PAGE = " 위/아래: 이동  좌/우: 페이지  Enter: 선택  지우기: 닫기";
	const char* MARK = "> ";
	const char* NO_MARK = "  ";

	const int SYMBOL_W = 1; // ■ □ ─ │ ┌ ★ 같은 기호가 화면에서 차지하는 칸 수.
	// 예전 콘솔(conhost) + 한국어 = 2, Windows Terminal = 1
	// → 실행했을 때 오른쪽 테두리가 들쭉날쭉하면 이 값을 1로 바꿀 것

	string title;
	vector<string> logLines;
	bool equipPage = false;
	int panelCursor = -1; // 인벤토리/장비 패널의 > 위치 (-1이면 표시 안 함)

	// SelectList 중에만 쓰는 상태. 목록을 띄우는 동안만 가리킴
	const vector<UI::ListEntry>* listEntries = nullptr; 
	string listTitle;
	int listCursor = 0;
	const int LIST_ROWS = TOP_H - 2 - 3;

	// ASCII: 1바이트·1칸 / 한글: 2바이트·2칸 / 기호: 2바이트·SYMBOL_W칸
	int CharBytes(unsigned char c) { return c < 0x80 ? 1 : 2; }

	int CharWidth(const string& s, size_t i) {
		unsigned char c = s[i];
		if (c < 0x80) return 1; //아스키 값
		if (c == 0xA4 || (c >= 0xB0 && c <= 0xC8)) return 2;  //한글 
		return SYMBOL_W; //그 외는 특수기호 처리
	}

	int Width(const string& s) {
		int w = 0;
		for (size_t i = 0; i < s.size(); i += CharBytes(s[i])) w += CharWidth(s, i);
		return w;
	}

	string Fit(const string& s, int w) { //문자열 s를 폭 w에 맞게 자르거나 공백으로 채움
		string out;
		int cur = 0;
		for (size_t i = 0; i < s.size(); i += CharBytes(s[i])) {
			int cw = CharWidth(s, i);
			if (cur + cw > w) break;
			out += s.substr(i, CharBytes(s[i]));
			cur += cw;
		}
		return out + string(w - cur, ' ');
	}

	string HLine(int w) //w만큼 가로선
	{
		string r;
		for (int i = 0; i < w / SYMBOL_W; ++i) r += "─";
		return r + string(w % SYMBOL_W, ' ');
	}

	int Inner(int boxW) { return boxW - 2 * SYMBOL_W - 2; } //"│ " 와 " │" 를 뺀 폭

	vector<string> Box(const string& boxTitle, const vector<string>& lines, int w, int h) {
		vector<string> rows;
		string label = " " + boxTitle + " ";
		int remain = w - 2 * SYMBOL_W - Width(label);
		rows.push_back("┌" + HLine(remain / 2) + label + HLine(remain - remain / 2) + "┐");
		for (int i = 0; i < h - 2; ++i) {
			string text = (i < (int)lines.size()) ? lines[i] : "";
			rows.push_back("│ " + Fit(text, Inner(w)) + " │");
		}
		rows.push_back("└" + HLine(w - 2 * SYMBOL_W) + "┘");
		return rows;
	}

	string HpBar(int hp, int maxHp, int cells = 10) {
		int filled = (maxHp > 0) ? hp * cells / maxHp : 0;
		string bar = "[";
		for (int i = 0; i < cells; ++i) bar += (i < filled) ? '#' : '-';
		return bar + "]";
	}
	// ===== 각 패널 내용 =====
	vector<string> LogPanel() {          // 최근 로그만 보이게 (넘치면 위쪽부터 잘림)
		int rows = TOP_H - 2;
		int start = max(0, (int)logLines.size() - rows);
		return vector<string>(logLines.begin() + start, logLines.end());
	}

	vector<string> InvenPanel(const Player& p) {
		vector<string> v;
		for (int i = 0; i < INVEN_SIZE; ++i) {
			bool sel = (!equipPage && panelCursor == i);
			v.push_back(string(sel ? MARK : NO_MARK) + to_string(i + 1) + ". " + p.GetInven().Get(i).GetName());
		}
		v.push_back("");
		v.push_back("소지금 : " + to_string(p.GetGold()) + "G");
		return v;
	}

	vector<string> EquipPanel(const Player& p) {
		vector<string> v;
		for (int i = 0; i < EQUIPSLOT_END; ++i) {
			const ITEM& item = p.GetEquip(i);
			bool sel = (equipPage && panelCursor == i);
			v.push_back(string(sel ? MARK : NO_MARK) + Fit(EQUIPSLOT_NAME[i], 7) + ": " + (item.IsEmpty() ? "-": item.GetName()));
		}
		Player::EquipStats s = p.getEquipStats();
		v.push_back("");
		v.push_back("ATK +" + to_string(s.atk) + "  DEF +" + to_string(s.def));
		v.push_back("MAX HP +" + to_string(s.hp));
		return v;
	}

	vector<string> EnemyPanel(const Enemy& e) {
		return {
			e.GetName(),
			"",
			"HP " + HpBar(e.GetHp(), e.GetMaxHp()),
			"   " + to_string(e.GetHp()) + " / " + to_string(e.GetMaxHp()),
			"",
			"ATK " + to_string(e.GetAttack()) + "  DEF " + to_string(e.GetDefense()),
		};
	}

	vector<string> StatusPanel(const Player& p) {
		if (p.GetJobName().empty()) return { "(캐릭터 생성 전)" };
		vector<string> v = {
			p.GetName() + " [" + p.GetJobName() + "] Lv." + to_string(p.GetLevel()),
			"HP " + HpBar(p.GetHp(), p.GetMaxHp()),
			"   " + to_string(p.GetHp()) + " / " + to_string(p.GetMaxHp()),
			"ATK " + to_string(p.GetAttack()) + "  DEF " + to_string(p.GetDefense()),
		};
		if (p.HasQuest()) {
			const QuestData& q = GetQuestData(p.GetQuestId());
			bool done = p.GetQuestKills() >= q.count;
			v.push_back("");
			v.push_back("Q " + q.name);
			v.push_back("  " + to_string(p.GetQuestKills()) + "/" + to_string(q.count) + (done ? " (보고하자!)" : ""));
		}
		return v;
	}
	// cursor 번째 선택지 앞에 ▶. cursor가 -1이면 표시 없음. bottom은 맨 아래 줄(안내문/입력 줄)
	vector<string> OptionPanel(const vector<string>& options, int cursor, const string& bottom) {
		
		int colW = Inner(LEFT_W) / 2;
		int rows = (int)(options.size() + 1) / 2;
		vector<string> v;

		for (int r = 0; r < rows; ++r) {
			string line;
			for (int c = 0; c < 2; ++c) {
				int idx = r * 2 + c;
				if (idx < (int)options.size())
					line += Fit((idx == cursor ? MARK : NO_MARK) + options[idx], colW);
			}
			v.push_back(line);
		}
		while ((int)v.size() < BOTTOM_H - 3) v.push_back("");
		v.push_back(bottom);
		return v;
	}


	//선택지 리스트 
	vector<string> SlotInfoLines(const Player& p, const string& guide, bool allowPage) {
		const ITEM& item = equipPage ? p.GetEquip(panelCursor) : p.GetInven().Get(panelCursor);
		vector<string> v = { guide, "" };
		if (item.IsEmpty()) {
			v.push_back("(빈 칸)");
		}
		else {
			v.push_back("[" + string(item.GetName()) + "] " + GetItemInfo(item));
			v.push_back(GetItemData(item.ItemID).description);
		}

		while ((int)v.size() < BOTTOM_H - 3) v.push_back("");

		v.push_back(allowPage ? HELP_SLOT_PAGE : HELP_SLOT);
		return v;
	}
	int PageCount(int n) { return max(1, (n + LIST_ROWS - 1) / LIST_ROWS); }   // 올림 나눗셈
	string ListTitle() {
		int pages = PageCount((int)listEntries->size());
		if (pages == 1) return listTitle;
		return listTitle + " (" + to_string(listCursor / LIST_ROWS + 1) + "/" + to_string(pages) + ")";
	}
	vector<string> ListPanel(const Player& p) {
		vector<string> v;
		int page = listCursor / LIST_ROWS;
		int inner = Inner(RIGHT_W);
		for (int i = page * LIST_ROWS; i < (int)listEntries->size() && i < (page + 1) * LIST_ROWS; ++i) {
			const UI::ListEntry& e = (*listEntries)[i];
			string mark = (i == listCursor) ? MARK : NO_MARK;
			int labelW = inner - Width(mark) - Width(e.tag) - 1;          // tag를 오른쪽 끝에 붙이기 위한 폭
			v.push_back(mark + Fit(e.label, labelW) + " " + e.tag);
		}
		while ((int)v.size() < LIST_ROWS) v.push_back("");

		int used = 0;
		for (int i = 0; i < INVEN_SIZE; ++i)
			if (!p.GetInven().IsEmptyAt(i)) ++used;
		v.push_back("");
		v.push_back("소지금 : " + to_string(p.GetGold()) + "G");
		v.push_back("가방 : " + to_string(used) + "/" + to_string(INVEN_SIZE));
		return v;
	}
	vector<string> ListInfoLines(const string& guide) {
		vector<string> v = { guide, "" };
		for (const string& s : (*listEntries)[listCursor].info) v.push_back(s);
		v.resize(BOTTOM_H - 3);          // 모자라면 빈 줄로 채우고, 넘치면 잘라냄
		v.push_back(PageCount((int)listEntries->size()) > 1 ? HELP_SLOT_PAGE : HELP_SLOT);
		return v;
	}

	// 위,아래 각각 왼쪽 상자와 오른쪽 상자를 한 줄씩 이어 붙여 출력
	void Render(const Player& p, const vector<string>& optionLines, const Enemy* enemy) {
		// cls 대신 커서를 맨 위로 옮겨 덮어쓴다. 모든 줄이 정확히 98칸이라 이전 화면이 남지 않음 => 깜빡임 없음
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });

		vector<string> tl = Box(title, LogPanel(), LEFT_W, TOP_H);
		vector<string> tr = enemy ? Box("적", EnemyPanel(*enemy), RIGHT_W, TOP_H)
			: listEntries ? Box(ListTitle(), ListPanel(p), RIGHT_W, TOP_H)
			: equipPage ? Box("장 비 (2/2)", EquipPanel(p), RIGHT_W, TOP_H)
			: Box("인벤토리 (1/2)", InvenPanel(p), RIGHT_W, TOP_H);
		vector<string> bl = Box("선택지", optionLines, LEFT_W, BOTTOM_H);
		vector<string> br = Box("상 태", StatusPanel(p), RIGHT_W, BOTTOM_H);

		for (int i = 0; i < TOP_H; ++i) cout << tl[i] << tr[i] << '\n';
		for (int i = 0; i < BOTTOM_H; ++i) cout << bl[i] << br[i] << '\n';

	}

	//콘솔 커서(깜빡임) 보이기/숨기기
	void SetCursorVisible(bool visible) {
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(out, &info);
		info.bVisible = visible;
		SetConsoleCursorInfo(out, &info);
	}


	//***키보드 입력 함수들***
	enum class Key {
		NONE, UP, DOWN, LEFT, RIGHT, ENTER, BACK
	};

	Key ReadKey() {
		int ch = _getch();
		if (ch == 0 || ch == 224) {          // 방향키: 코드가 두 개
			Key key = Key::NONE;
			switch (_getch()) {
			case 72: key = Key::UP;    break;
			case 80: key = Key::DOWN;  break;
			case 75: key = Key::LEFT;  break;
			case 77: key = Key::RIGHT; break;
			}
			if (key != Key::NONE) Sound::PlaySfx("Sfx Title Interface.mp3");   // 방향키 소리
			return key;
		}
		if (ch == '\r') {                    // Enter
			Sound::PlaySfx("Sfx Select.mp3");
			return Key::ENTER;
		}
		if (ch == '\b') return Key::BACK;    // Backspace
		return Key::NONE;
	}


	//커서(▶) 이동 범위 지정. 범위 밖으로 나가지 않게 함.
	int MoveCursor(int cur, Key key, int count) {
		int next = cur;
		switch (key)
		{
		case Key::UP:	next = cur - 2; break;
		case Key::DOWN:	next = cur + 2; break;
		case Key::LEFT:	if (cur % 2 == 1)next = cur - 1; break; //오른쪽 칸에서만 검사
		case Key::RIGHT:	if (cur % 2 == 0)next = cur + 1; break; //왼쪽 칸에서만 검사
		default: break;
		}
		return (next >= 0 && next < count) ? next : cur;
	}

	int RunMenu(const Player& player, const vector<string>& options, int first, const Enemy* enemy, bool allowBack) {
		string help = HELP;
		if (allowBack) help += "   뒤로가기: " + options.back();

		int cursor = 0;
		while (true) {
			Render(player, OptionPanel(options, cursor, help), enemy);
			Key key = ReadKey();
			if (key == Key::ENTER) return first + cursor;
			if (key == Key::BACK && allowBack) return first + (int)options.size() - 1;

			cursor = MoveCursor(cursor, key, (int)options.size());
		}
	}
}

// ===== 공개 함수 =====
void UI::Init() {
	system("mode con cols=100 lines=30");
	system("cls");
	SetCursorVisible(false);
}

void UI::SetTitle(const string& t) { title = t; }

void UI::ClearLog() { logLines.clear(); }

void UI::Log(const string& text) {
	int w = Inner(LEFT_W);
	string line;
	int cur = 0;
	for (size_t i = 0; i < text.size(); i += CharBytes(text[i])) {
		int cw = CharWidth(text, i);
		if (cur + cw > w) { logLines.push_back(line); line.clear(); cur = 0; }
		line += text.substr(i, CharBytes(text[i]));
		cur += cw;
	}
	logLines.push_back(line);
}

int UI::Menu(const Player& player, const vector<string>& options, int first, const Enemy* enemy) {
	return RunMenu(player, options, first, enemy, false);
}

int UI::MenuBack(const Player& player, const vector<string>& options) {
	return RunMenu(player, options, 1, nullptr, true);
}

string UI::AskText(const Player& player, const string& prompt) {
	Render(player, OptionPanel({}, -1, prompt), nullptr);     // 선택지 없이 입력 줄만
	

	COORD pos;
	pos.X = (SHORT)(SYMBOL_W + 1 + Width(prompt));
	pos.Y = (SHORT)(TOP_H + BOTTOM_H - 2);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	SetCursorVisible(true);

	string text;
	cin >> text;
	SetCursorVisible(false);
	return text;
}

int UI::SelectSlot(const Player& player, bool allowPage, const string& guide) {
	if (panelCursor < 0) panelCursor = 0;          // 처음이면 첫 칸부터 (아니면 지난 위치 유지)

	while (true) {
		int count = equipPage ? EQUIPSLOT_END : INVEN_SIZE;
		if (panelCursor >= count) panelCursor = count - 1;

		Render(player, SlotInfoLines(player, guide, allowPage), nullptr);
		Key key = ReadKey();

		if (key == Key::ENTER) return panelCursor;
		if (key == Key::BACK)  return -1;
		if (key == Key::UP && panelCursor > 0)         --panelCursor;
		if (key == Key::DOWN && panelCursor < count - 1) ++panelCursor;
		if (allowPage && (key == Key::LEFT || key == Key::RIGHT)) {
			equipPage = !equipPage;                    // 페이지가 두 개라 좌/우 모두 "반대 페이지로"
			panelCursor = 0;
		}
	}
}

bool UI::IsEquipPage() { return equipPage; }

void UI::ResetSlotSelect() {
	equipPage = false;
	panelCursor = -1;
}

//선택지 리스트 출력
int UI::SelectList(const Player& player, const string& panelTitle,
	const vector<ListEntry>& entries, const string& guide, int& cursor) {
	if (entries.empty()) return -1;                    // 고를 게 없으면 바로 닫기

	listEntries = &entries;
	listTitle = panelTitle;
	listCursor = max(0, min(cursor, (int)entries.size() - 1));
	int last = (int)entries.size() - 1;

	int result = -1;
	while (true) {
		Render(player, ListInfoLines(guide), nullptr);
		Key key = ReadKey();
		if (key == Key::ENTER) { result = listCursor; break; }
		if (key == Key::BACK) { result = -1; break; }
		if (key == Key::UP)    listCursor = max(0, listCursor - 1);
		if (key == Key::DOWN)  listCursor = min(last, listCursor + 1);
		if (key == Key::LEFT)  listCursor = max(0, listCursor - LIST_ROWS);       // 이전 페이지
		if (key == Key::RIGHT) listCursor = min(last, listCursor + LIST_ROWS);    // 다음 페이지
	}

	cursor = listCursor;        // 부르는 쪽에 위치를 돌려줌 (다음에 이어서 고를 수 있게)
	listEntries = nullptr;      // 목록은 부르는 쪽 것이라, 함수가 끝나면 반드시 손을 뗀다
	return result;
}