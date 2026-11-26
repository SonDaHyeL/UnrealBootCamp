#define NOMINMAX	// windows.h의 min/max 매크로가 std::max와 충돌하지 않게
#include <windows.h>
#include <iostream>
#include <limits>
#include "UI.h"
#include "Player.h"
#include "Enemy.h"
using namespace std;

namespace {
	const int LEFT_W = 68;
	const int RIGHT_W = 30;
	const int TOP_H = 18;
	const int BOTTOM_H = 9;
	const char* PROMPT = " 어떻게 할까요? > ";

	const int SYMBOL_W = 1; // ■ □ ─ │ ┌ ★ 같은 기호가 화면에서 차지하는 칸 수.
	// 예전 콘솔(conhost) + 한국어 = 2, Windows Terminal = 1
	// → 실행했을 때 오른쪽 테두리가 들쭉날쭉하면 이 값을 1로 바꿀 것

	string title;
	vector<string> logLines;
	bool equipPage = false;

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
		for (int i = 0; i < INVEN_SIZE; ++i)
			v.push_back(to_string(i + 1) + ". " + p.GetInven().Get(i).GetName());
		v.push_back("");
		v.push_back("소지금 : " + to_string(p.GetGold()) + "G");
		return v;
	}

	vector<string> EquipPanel(const Player& p) {
		vector<string> v;
		for (int i = 0; i < EQUIPSLOT_END; ++i) {
			const ITEM& item = p.GetEquip(i);
			v.push_back(Fit(EQUIPSLOT_NAME[i], 7) + ": " + (item.IsEmpty() ? "-": item.GetName()));
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
		return {
			p.GetName() + " [" + p.GetJobName() + "] Lv." + to_string(p.GetLevel()),
			"HP " + HpBar(p.GetHp(), p.GetMaxHp()),
			"   " + to_string(p.GetHp()) + " / " + to_string(p.GetMaxHp()),
			"ATK " + to_string(p.GetAttack()) + "  DEF " + to_string(p.GetDefense()),
		};
	}

	vector<string> OptionPanel(const vector<string>& options, int first, const string& prompt) {
		// 2열로 배치하고, 마지막 줄은 입력 줄로 비워둔다
		int colW = Inner(LEFT_W) / 2;
		int rows = (int)(options.size() + 1) / 2;
		vector<string> v;
		for (int r = 0; r < rows; ++r) {
			string line;
			for (int c = 0; c < 2; ++c) {
				int idx = r * 2 + c;
				if (idx < (int)options.size())
					line += Fit(to_string(first + idx) + ". " + options[idx], colW);
			}
			v.push_back(line);
		}
		while ((int)v.size() < BOTTOM_H - 3) v.push_back("");
		v.push_back(prompt);          // PROMPT → prompt
		return v;
	}

	// 위·아래 각각 왼쪽 상자와 오른쪽 상자를 한 줄씩 이어 붙여 출력
	void Render(const Player& p, const vector<string>& options, int first, const Enemy* enemy, const string& prompt) {
		system("cls");
		vector<string> tl = Box(title, LogPanel(), LEFT_W, TOP_H);
		vector<string> tr = enemy ? Box("적", EnemyPanel(*enemy), RIGHT_W, TOP_H)
			: equipPage ? Box("장 비 (2/2)",EquipPanel(p), RIGHT_W, TOP_H)
			: Box("인벤토리 (1/2)", InvenPanel(p), RIGHT_W, TOP_H);
		vector<string> bl = Box("선택지", OptionPanel(options, first, prompt), LEFT_W, BOTTOM_H);
		vector<string> br = Box("상 태", StatusPanel(p), RIGHT_W, BOTTOM_H);

		for (int i = 0; i < TOP_H; ++i) cout << tl[i] << tr[i] << '\n';
		for (int i = 0; i < BOTTOM_H; ++i) cout << bl[i] << br[i] << '\n';

		// 커서를 선택지 상자 안의 입력 줄 끝으로 이동
		COORD pos;
		pos.X = (SHORT)(SYMBOL_W + 1 + Width(prompt));  
		pos.Y = (SHORT)(TOP_H + BOTTOM_H - 2);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}

	int ReadNumber() {
		int n = 0;
		if (!(cin >> n)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return -1;
		}
		return n;
	}
}

// ===== 공개 함수 =====
void UI::Init() {
	system("mode con cols=100 lines=30");
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
	while (true) {
		Render(player, options, first, enemy, PROMPT);
		int input = ReadNumber();
		if (input >= first && input < first + (int)options.size()) return input;
		Log("(!) 잘못된 입력입니다.");
	}
}

string UI::AskText(const Player& player, const string& prompt) {
	Render(player, {}, 0, nullptr, prompt);     // 선택지 없이 입력 줄만
	string text;
	cin >> text;
	return text;
}

void UI::SetEquipPage(bool on) { equipPage = on; }