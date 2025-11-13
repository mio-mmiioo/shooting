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
	DrawFormatString(100, 100, GetColor(255, 255, 255), "“G‚ğ“|‚µ‚½”F%d", Observer::GetEnemyKillled());
}

