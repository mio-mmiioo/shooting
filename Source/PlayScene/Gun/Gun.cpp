#include "Gun.h"
#include <assert.h>

Gun::Gun()
{
	gunType_ = GUN::TYPE::HAND;

	hImageEffectOutBullet_ = LoadGraph("data/image/right.png");
	assert(hImageEffectOutBullet_ > 0);

	current = { GUN::TYPE::MAX_TYPE, 0, 0, 0, -1, -1 };

	hand = {
		GUN::TYPE::HAND,
		5,
		16,
		16,
		LoadGraph("data/image/bulletUi01.png"),
		LoadGraph("data/image/bulletUi02.png")
	};

	machine = {
		GUN::TYPE::MACHINE,
		200,
		50,
		50,
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
	DrawRemainingSetting();
}

void Gun::DrawRemainingSetting()
{
	float remaining = 115.0 - (current.remainingSetting * 5);

	DrawCircleGauge(100, 200, 115.0, current.hImageGauge, 35.0, 1.0, 0, 0);
	DrawCircleGauge(100, 200, 115.0, current.hImageGaugeRemaining, remaining, 1.0, 0, 0);

	// 残弾数の表示
	if (current.remainingAll <= 0 && current.remainingSetting == 0)
	{
		DrawFormatString(100, 200, GetColor(255, 0, 0), "EMPTY");
	}
	else
	{
		DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", current.remainingSetting + current.remainingAll);
	}
}

int Gun::OutBullet()
{
	if (current.remainingSetting > 0)
	{
		current.remainingSetting -= 1;
		OutBulletEffect();
		return current.remainingSetting;
	}
	else
	{
		// 銃弾がない場合の処理
		return -1;
	}
}

void Gun::OutBulletEffect()
{
	// 銃を発砲したエフェクト画像
	DrawGraph(-10, -10, hImageEffectOutBullet_, TRUE); // 位置をマイナスにしているのは入れた画像の端が黒くなってたから
}

void Gun::ReloadBullet()
{
	if (!(current.remainingAll == current.remainingSetting))
	{
		int canSetNum = current.maxSetting - current.remainingSetting; // 装填可能数 0になってからリロードとは限らない

		if (canSetNum >= current.remainingAll) // 装填可能数 >= 残弾数
		{
			current.remainingSetting += current.remainingAll;
			current.remainingAll = 0;
		}
		else
		{
			current.remainingSetting += canSetNum;
			current.remainingAll -= canSetNum;
		}
	}
}

void Gun::AddGun(int addNumber)
{
	current.remainingAll += addNumber;
}

void Gun::SetGunType(GUN::TYPE type)
{
	// currentを元のstructに代入
	switch (current.type)
	{
	case GUN::TYPE::HAND:
		hand = current;
		break;
	case GUN::TYPE::MACHINE:
		machine = current;
		break;
	}

	// currentに新しくセットされる種類の情報を代入
	switch (type)
	{
	case GUN::TYPE::HAND:
		current = hand;
		break;
	case GUN::TYPE::MACHINE:
		current = machine;
		break;
	}
}
