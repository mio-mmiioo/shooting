#include "Result.h"
#include "Observer.h"
#include "DxLib.h"
#include "../Color.h"

void Result::Init()
{

}

void Result::Update()
{

}

void Result::Draw()
{
	DrawFormatString(100, 100, Color::WHITE, "敵を倒した数：%d", Observer::GetEnemyKillled());
	DrawFormatString(100, 130, Color::WHITE, "ポイント:%d", Observer::GetPoint());
}

