#pragma once
#include <string>

// 소리 파일은 모두 sound 폴더에 있음
namespace Sound {

	// 음량: 0(무음) ~ 1000(최대)
	void SetBgmVolume(int volume);           // 재생 중인 배경음에도 바로 적용
	void SetSfxVolume(int volume);           // 이후 효과음부터 적용
	
	void PlayBgm(const std::string& file);   // 배경음 반복 재생. 이미 같은 곡이면 아무것도 안 함
	void StopBgm();
	void PlaySfx(const std::string& file, int volume = -1);  // 효과음 한 번 (배경음 위에 겹쳐서)
	void Shutdown();                         // 게임 끝날 때: 열어둔 소리 전부 닫기
}
