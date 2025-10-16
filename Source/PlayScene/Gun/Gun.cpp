#include "Gun.h"
#include <assert.h>
#include "../../Sound.h"

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
		DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", current.remainingAll);
	}
}

bool Gun::OutBullet()
{
	if (current.remainingSetting <= 0 || current.coolDownTimer > 0) // 残弾数が0以下 or クールダウン中
	{
		return false;
	}
	else
	{
		current.remainingSetting -= 1;
		current.coolDownTimer = current.coolDownTime;
		PlaySoundMem(Sound::se["OutBullet1"], DX_PLAYTYPE_BACK, TRUE);
		StartJoypadVibration(DX_INPUT_PAD1, 300, 120);
		OutBulletEffect();
		return true;
	}
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
