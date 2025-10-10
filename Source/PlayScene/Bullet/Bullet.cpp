#include "Bullet.h"
#include <assert.h>

namespace BULLET
{
	const int MAX_SETTING_BULLET = 16;
}

Bullet::Bullet()
{
	remainingAll_ = 5;
	remainingSetting_ = BULLET::MAX_SETTING_BULLET;

	hImageGauge_ = LoadGraph("data/image/bulletUi01.png");
	assert(hImageGauge_ > 0);
	hImageGaugeRemaining_ = LoadGraph("data/image/bulletUi02.png");
	assert(hImageGaugeRemaining_ > 0);
	hImageEffectOutBullet_ = LoadGraph("data/image/right.png");
	assert(hImageEffectOutBullet_ > 0);
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
}

void Bullet::Draw()
{
	DrawRemainingSetting(remainingSetting_); // 残弾数のゲージの表示

	// 残弾数の表示分岐処理入れて
	if (remainingAll_ <= 0 && remainingSetting_ == 0)
	{
		DrawFormatString(100, 200, GetColor(255, 0, 0), "EMPTY");
	}
	//else if (remainingAll > 0 && remainingSetting == 0)
	//{
	//	DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", remainingSetting + remainingAll); 
	//  // ここにこのボタンを押してリロードしてって表示を一定時間後に表示できたらしたい
	//}
	else
	{
		DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", remainingSetting_ + remainingAll_); // 残弾数の表示
	}
}

void Bullet::DrawRemainingSetting(int currentRemainingSetting)
{
	float remaining = 115.0 - (currentRemainingSetting * 5);

	DrawCircleGauge(100, 200, 115.0, hImageGauge_, 35.0, 1.0, 0, 0);
	DrawCircleGauge(100, 200, 115.0, hImageGaugeRemaining_, remaining, 1.0, 0, 0);
}

int Bullet::OutBullet()
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

void Bullet::OutBulletEffect()
{
	// 銃を発砲したエフェクト画像　最終的にはライトで光らせる処理をいれたい
	DrawGraph(-10, -10, hImageEffectOutBullet_, TRUE); // 位置をマイナスにしているのは入れた画像の端が黒くなってたから
}

void Bullet::ReloadBullet()
{
	if (!(remainingAll_ == remainingSetting_))
	{
		int canSetNum = BULLET::MAX_SETTING_BULLET - remainingSetting_; // 装填可能数

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

void Bullet::AddBullet(int addNumber)
{
	remainingAll_ += addNumber;
}
