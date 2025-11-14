#include "Result.h"
#include "Observer.h"
#include "DxLib.h"

void Result::Init()
{

}

void Result::Update()
{

}

void Result::Draw()
{
	DrawFormatString(100, 100, GetColor(255, 255, 255), "敵を倒した数：%d", Observer::GetEnemyKillled());
	DrawFormatString(100, 130, GetColor(255, 255, 255), "ポイント:%d", Observer::GetPoint());
}

