#include "Gun.h"
#include <assert.h>
#include "../../Sound.h"
#include "../../Color.h"

namespace GUN
{
	int gaugePosX = 80;
	int gaugePosY = 140;

	const float START_PERCENT = 35.0f;
	const float END_PERCENT = 115.0f;
}

Gun::Gun()
{
	gunType_ = GUN::TYPE::HAND;

	hImageEffectOutBullet_ = LoadGraph("data/image/light.png");
	assert(hImageEffectOutBullet_ > 0);

	current = { GUN::TYPE::MAX_TYPE, 0, 0, 0, 0, -1, -1, 0.0f, 0.0f, 0.0f, 0.0f };

	hand = {
		GUN::TYPE::HAND,
		5,
		5,
		16,
		16,
		LoadGraph("data/image/bulletUi01.png"),
		LoadGraph("data/image/bulletUi02.png"),
		0.0f,
		3.0f,
		0.0f,
		0.7f
	};

	machine = {
		GUN::TYPE::MACHINE,
		2,
		200,
		50,
		50,
		LoadGraph("data/image/bulletUi01.png"), // 画像作って書き換えて
		LoadGraph("data/image/bulletUi02.png"),
		0.0f,
		2.0f,
		0.0f,
		0.06f
	};
}

Gun::~Gun()
{
	if (hImageEffectOutBullet_ > 0)
	{
		DeleteGraph(hImageEffectOutBullet_);
		hImageEffectOutBullet_ = -1;
	}
}

void Gun::Update()
{
	// タイマーを進める
	if (current.reloadTimer > 0)
	{
		current.reloadTimer -= Time::DeltaTime();
	}
	if (current.coolDownTimer > 0)
	{
		current.coolDownTimer -= Time::DeltaTime();
	}
}

void Gun::Draw()
{
	DrawRemainingSetting();
}

void Gun::DrawRemainingSetting()
{
	float p = 100.0f / (current.maxSetting * 1.25f);
	float remaining = GUN::END_PERCENT - (current.remainingSetting * p);

	DrawCircleGauge(GUN::gaugePosX, GUN::gaugePosY, GUN::END_PERCENT, current.hImageGauge, GUN::START_PERCENT, 1.0, 0, 0);
	DrawCircleGauge(GUN::gaugePosX, GUN::gaugePosY, GUN::END_PERCENT, current.hImageGaugeRemaining, remaining, 1.0, 0, 0);

	// 残弾数の表示
	if (current.remainingAll <= 0 && current.remainingSetting == 0)
	{
		DrawFormatString(GUN::gaugePosX, GUN::gaugePosY, Color::RED, "EMPTY");
	}
	else
	{
		DrawFormatString(GUN::gaugePosX, GUN::gaugePosY, Color::BLACK, "%04d", current.remainingAll);
	}
}

bool Gun::OutBullet()
{
	if (current.coolDownTimer > 0 || current.reloadTimer > 0) // 残弾数が0以下 or クールダウン中
	{
		return false;
	}
	if (current.remainingSetting <= 0)
	{
		if (CheckSoundMem(Sound::se["CanNotOutBullet"]) == 0)
		{
			PlaySoundMem(Sound::se["CanNotOutBullet"], DX_PLAYTYPE_BACK, TRUE);
		}
		return false;
	}
	
	current.remainingSetting -= 1;
	current.coolDownTimer = current.coolDownTime;
	PlaySoundMem(Sound::se["OutBullet1"], DX_PLAYTYPE_BACK, TRUE);
	OutBulletEffect();
	return true;
}

void Gun::OutBulletEffect()
{
	// 銃を発砲したエフェクト画像
	DrawGraph(-10, -10, hImageEffectOutBullet_, TRUE); // 位置をマイナスにしているのは入れた画像の端が黒くなってたから
}

void Gun::ReloadBullet()
{
	if (current.reloadTimer <= 0) // リロード中じゃない
	{
		if (!((current.remainingAll == current.remainingSetting) || (current.remainingAll <= 0))) // リロードできない銃弾数条件じゃない
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
			PlaySoundMem(Sound::se["Reload"], DX_PLAYTYPE_BACK, TRUE);
			current.reloadTimer = current.reloadTime; // リロードの時間をセット
		}
	}
}

void Gun::AddBullet(int addNumber)
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

int Gun::GetAttack()
{
	return current.attak;
}

float Gun::GetReloadTimer()
{
	return current.reloadTimer;
}

float Gun::GetReloadTime()
{
	return current.reloadTime;
}
