#include "Gun.h"
#include <assert.h>

Gun::Gun()
{
	gunType_ = GUN_TYPE::HAND;

	hImageEffectOutBullet_ = LoadGraph("data/image/right.png");
	assert(hImageEffectOutBullet_ > 0);

	gun hand = {
		5,
		16,
		16,
		LoadGraph("data/image/bulletUi01.png"),
		LoadGraph("data/image/bulletUi02.png")
	};

	gun machine = {
		200,
		50,
		machine.MAX_SETTING,
		LoadGraph("data/image/bulletUi01.png"), // 画像作って書き換えて
		LoadGraph("data/image/bulletUi02.png")
	};
}

Gun::~Gun()
{
}

void Gun::Update()
{
}

void Gun::Draw()
{
	switch (gunType_)
	{
	case GUN_TYPE::HAND:
		DrawRemainingSetting(&hand);
		break;
	case GUN_TYPE::MACHINE:
		DrawRemainingSetting(&machine);
		break;
	}
}

void Gun::DrawRemainingSetting(struct gun* g)
{
	float remaining = 115.0 - (g.remainingSetting_ * 5);

	DrawCircleGauge(100, 200, 115.0, g.hImageGauge_, 35.0, 1.0, 0, 0);
	DrawCircleGauge(100, 200, 115.0, g.hImageGaugeRemaining_, remaining, 1.0, 0, 0);

	// 残弾数の表示
	if (remainingAll_ <= 0 && remainingSetting_ == 0)
	{
		DrawFormatString(100, 200, GetColor(255, 0, 0), "EMPTY");
	}
	else
	{
		DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", g.remainingSetting_ + g.remainingAll_);
	}
}

int Gun::OutBullet()
{
	if (remainingSetting_ > 0)
	{
		remainingSetting_ -= 1;
		OutBulletEffect();
		return remainingSetting_;
	}
	else
	{
		// 銃弾がない場合の処理
		return -1;
	}
}

void Gun::OutBulletEffect()
{
	// 銃を発砲したエフェクト画像　最終的にはライトで光らせる処理をいれたい
	DrawGraph(-10, -10, hImageEffectOutBullet_, TRUE); // 位置をマイナスにしているのは入れた画像の端が黒くなってたから
}

void Gun::ReloadBullet()
{
	if (!(remainingAll_ == remainingSetting_))
	{
		int canSetNum = 16 - remainingSetting_; // 装填可能数

		if (canSetNum >= remainingAll_) // 装填可能数 >= 残弾数
		{
			remainingSetting_ += remainingAll_;
			remainingAll_ = 0;
		}
		else
		{
			remainingSetting_ += canSetNum;
			remainingAll_ -= canSetNum;
		}
	}
}

void Gun::AddGun(int addNumber)
{
	remainingAll_ += addNumber;
}