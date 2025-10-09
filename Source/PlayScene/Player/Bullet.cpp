#include "Bullet.h"
#include "DxLib.h"
#include <assert.h>

namespace BULLET
{
	const int MAX_SETTING_BULET = 16; // e‚É‘•“U‚Å‚«‚éÅ‘å”
	int remainingAll = 5;//Žc’e”
	int remainingSetting = MAX_SETTING_BULET;//e‚É‘•‚Ä‚ñ‚³‚ê‚Ä‚¢‚éŽc’e”@–¼‘OŒã‚Å‚í‚©‚é‚æ‚¤‚É•Ï‚¦‚Ä

	int hImageGauge; // e’e‚Ì‘•“U”‚ÌƒQ[ƒW
	int hImageGaugeRemaining; // Žc’e”‚ðŽ¦‚·‚Ì‚ÉŽg—p‚·‚é
}

void BULLET::Init()
{
	hImageGauge = LoadGraph("data/image/bulletUi01.png");
	assert(hImageGauge > 0);
	hImageGaugeRemaining = LoadGraph("data/image/bulletUi02.png");
	assert(hImageGaugeRemaining > 0);

}

void BULLET::Update()
{

}

void BULLET::Draw()
{
	DrawRemainingSetting(remainingSetting); // Žc’e”‚ÌƒQ[ƒW‚Ì•\Ž¦

	DrawFormatString(10, 100, GetColor(255, 255, 255), "tamesi");
}

//Žc’e”‚ðGet‚µ‚ÄŽc’e”‚Ì•ª,•\Ž¦‚·‚é‰~ƒQ[ƒW
void BULLET::DrawRemainingSetting(int currentRemainingSetting)
{
	float remaining = 115.0 - (currentRemainingSetting * 5);

	DrawCircleGauge(100, 200, 115.0, hImageGauge, 35.0, 1.0, 0, 0);
	DrawCircleGauge(100, 200, 115.0, hImageGaugeRemaining, remaining, 1.0, 0, 0);
}

void BULLET::OutBullet()
{
	if (remainingSetting > 0)
	{
		remainingSetting -= 1;
	}
}

void BULLET::AddBullet(int addNumber)
{
	remainingAll += addNumber;
}
