#include "Bullet.h"
#include "DxLib.h"
#include <assert.h>

namespace BULLET
{
	const int MAX_SETTING_BULET = 16;			// èeÇ…ëïìUÇ≈Ç´ÇÈç≈ëÂêî
	int remainingAll = 5;						// écíeêî
	int remainingSetting = MAX_SETTING_BULET;	// èeÇ…ëïÇƒÇÒÇ≥ÇÍÇƒÇ¢ÇÈécíeêîÅ@ñºëOå„Ç≈ÇÌÇ©ÇÈÇÊÇ§Ç…ïœÇ¶Çƒ

	int hImageGauge;							// èeíeÇÃëïìUêîÇÃÉQÅ[ÉW
	int hImageGaugeRemaining;					// écíeêîÇé¶Ç∑ÉQÅ[ÉW
	int hImageReload;							// ÉäÉçÅ[ÉhÇé¶Ç∑ÉQÅ[ÉW
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
	DrawRemainingSetting(remainingSetting); // écíeêîÇÃÉQÅ[ÉWÇÃï\é¶

	// écíeêîÇÃï\é¶ï™äÚèàóùì¸ÇÍÇƒ
	DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", remainingSetting + remainingAll); // écíeêîÇÃï\é¶
}

// écíeêîÇGetÇµÇƒécíeêîÇÃï™,ï\é¶Ç∑ÇÈâ~ÉQÅ[ÉW
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
		// èeíeÇ™Ç»Ç¢èÍçáÇÃèàóù
		DrawFormatString(100, 200, GetColor(255, 0, 0), "RELOAD");
		return -1;
	}
}

void BULLET::ReloadBullet()
{
	int canSetNum = BULLET::MAX_SETTING_BULET - remainingSetting; // ëïìUâ¬î\êî

	if (canSetNum >= remainingAll) // ëïìUâ¬î\êî >= écíeêî
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
