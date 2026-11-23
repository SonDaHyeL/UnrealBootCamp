#include "TimeManager.h"
#include <Windows.h>

static ULONGLONG StartTick = GetTickCount64(); //게임 시작 시간

int FPS = 0;
int FPSCount = 0;

void FPSUpdate()
{
    ++FPSCount;

    //GetTickCount(); //DWORD
    //GetTickCount64(); //longlong
    if (GetTickCount64() - StartTick >= 1000){ // == 1초! 1초마다 호출하는 중
        //초당 몇 프레임인지 측정하기 위한 식
        FPS = FPSCount; //1초동안 센 값을 확정
        FPSCount = 0; //다음 초를 위해 리셋
        StartTick += 1000;
    }
    
}

int GetFps()
{
    return FPS;
}

float GetDeltaTime()
{
    return 0.0f;
}
