#pragma once
#pragma once
#include <DxLib.h>
#include <vector>
#include <string>
#include <map>

namespace Sound
{
	extern std::map<std::string, int> se;
	extern std::map<std::string, int> bgm;

	void InitSE();

	void InitBgm();
}
