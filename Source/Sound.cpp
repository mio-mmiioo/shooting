#include "Sound.h"
#include <DxLib.h>

namespace Sound {
	std::map<std::string, int> se;
	std::map<std::string, int> bgm;
}

void Sound::InitSE()
{
	se["OutBullet1"] = LoadSoundMem("data/sound/outBullet1.mp3");

	se["Reload"] = LoadSoundMem("data/sound/reload.mp3");
}

void Sound::InitBgm()
{
}
