#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>                 // mciSendString
#include "Sound.h"
#pragma comment(lib, "winmm.lib")     // 프로젝트 설정 없이 winmm 라이브러리 연결
using namespace std;

namespace {
	const string SOUND_DIR = "sound/";
	const int SFX_CHANNELS = 4;       // 효과음을 동시에 몇 개까지 겹쳐 낼지

	string currentBgm;                // 지금 재생 중인 배경음 파일 이름 (없으면 "")
	int nextSfx = 0;                  // 다음 효과음에 쓸 채널 번호

	// MCI에 문자열 명령을 보낸다. 성공하면 true
	// 프로젝트가 유니코드 설정이라, string(CP949)을 그대로 넘기려면 A 버전을 직접 불러야 한다
	bool Mci(const string& command) {
		return mciSendStringA(command.c_str(), nullptr, 0, nullptr) == 0;
	}

	int bgmVolume = 500;              // 기본 음량 (0~1000)
	int sfxVolume = 500;

	int ClampVolume(int v) { return v < 0 ? 0 : (v > 1000 ? 1000 : v); }

	void ApplyVolume(const string& alias, int volume) {
		Mci("setaudio " + alias + " volume to " + to_string(volume));
	}
}

void Sound::PlayBgm(const string& file) {
	if (file == currentBgm) return;

	StopBgm();
	if (!Mci("open \"" + SOUND_DIR + file + "\" type mpegvideo alias bgm")) return;
	ApplyVolume("bgm", bgmVolume);                       // ← 추가
	Mci("play bgm repeat");
	currentBgm = file;
}

void Sound::PlaySfx(const string& file, int volume) {
	string alias = "sfx" + to_string(nextSfx);
	nextSfx = (nextSfx + 1) % SFX_CHANNELS;

	Mci("close " + alias);
	if (Mci("open \"" + SOUND_DIR + file + "\" type mpegvideo alias " + alias)) {
		ApplyVolume(alias, volume < 0 ? sfxVolume : ClampVolume(volume));   // ← 추가
		Mci("play " + alias);
	}
}

//음량 조절 (BGM,SFX)
void Sound::SetBgmVolume(int volume) {
	bgmVolume = ClampVolume(volume);
	if (!currentBgm.empty()) ApplyVolume("bgm", bgmVolume);   // 틀고 있는 곡에 바로 적용
}

void Sound::SetSfxVolume(int volume) {
	sfxVolume = ClampVolume(volume);
}

void Sound::StopBgm() {
	Mci("close bgm");                 // 열린 게 없으면 실패하지만 상관없음
	currentBgm.clear();
}

void Sound::Shutdown() {
	Mci("close all");
	currentBgm.clear();
}