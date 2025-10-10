#include "Bullet.h"
#include "DxLib.h"
#include <assert.h>

namespace BULLET
{
	const int MAX_SETTING_BULET = 16;			// e‚É‘•“U‚Å‚«‚éÅ‘å”
	int remainingAll = 5;						// c’e”
	int remainingSetting = MAX_SETTING_BULET;	// e‚É‘•‚Ä‚ñ‚³‚ê‚Ä‚¢‚éc’e”@–¼‘OŒã‚Å‚í‚©‚é‚æ‚¤‚É•Ï‚¦‚Ä

	int hImageGauge;							// e’e‚Ì‘•“U”‚ÌƒQ[ƒW
	int hImageGaugeRemaining;					// c’e”‚ğ¦‚·ƒQ[ƒW
	int hImageReload;							// ƒŠƒ[ƒh‚ğ¦‚·ƒQ[ƒW
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
	DrawRemainingSetting(remainingSetting); // c’e”‚ÌƒQ[ƒW‚Ì•\¦

	// c’e”‚Ì•\¦•ªŠòˆ—“ü‚ê‚Ä
	if (remainingAll <= 0 && remainingSetting == 0)
	{
		DrawFormatString(100, 200, GetColor(255, 0, 0), "EMPTY");
	}
	else if (remainingAll > 0 && remainingSetting == 0)
	{
		DrawFormatString(100, 200, GetColor(255, 0, 0), "RELOAD");
	}
	else
	{
		DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", remainingSetting + remainingAll); // c’e”‚Ì•\¦
	}
}

// c’e”‚ğGet‚µ‚Äc’e”‚Ì•ª,•\¦‚·‚é‰~ƒQ[ƒW
void BULLET::DrawRemainingSetting(int currentRemainingSetting)
{
	float remaining = 115.0 - (currentRemainingSetting * 5);

	DrawCircleGauge(100, 200, 115.0, hImageGauge, 35.0, 1.0, 0, 0);
	DrawCircleGauge(100, 200, 115.0, hImageGaugeRemaining, remaining, 1.0, 0, 0);
}

int BULLET::OutBullet()
{
	if (remainingSetting > 0)
	{
		remainingSetting -= 1;
		return remainingSetting;
	}
	else
	{
		// e’e‚ª‚È‚¢ê‡‚Ìˆ—
		DrawFormatString(100, 200, GetColor(255, 0, 0), "RELOAD");
		return -1;
	}
}

void BULLET::ReloadBullet()
{
	int canSetNum = BULLET::MAX_SETTING_BULET - remainingSetting; // ‘•“U‰Â”\”

	if (canSetNum >= remainingAll) // ‘•“U‰Â”\” >= c’e”
	{
		remainingSetting += remainingAll;
		remainingAll = 0;
	}
	else
	{
		remainingSetting += canSetNum;
		remainingAll -= canSetNum;
	}
}

void BULLET::AddBullet(int addNumber)
{
	remainingAll += addNumber;
}
