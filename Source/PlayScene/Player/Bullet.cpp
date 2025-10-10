#include "Bullet.h"
#include "DxLib.h"
#include <assert.h>

namespace BULLET
{
	const int MAX_SETTING_BULET = 16;			// 銃に装填できる最大数
	int remainingAll = 5;						// 残弾数
	int remainingSetting = MAX_SETTING_BULET;	// 銃に装てんされている残弾数　名前後でわかるように変えて

	int hImageGauge;							// 銃弾の装填数のゲージ
	int hImageGaugeRemaining;					// 残弾数を示すゲージ
	int hImageReload;							// リロードを示すゲージ
	int hImageEffectOutBullet;					// 発砲後に画面を光らせるための画像
}

void BULLET::Init()
{
	hImageGauge = LoadGraph("data/image/bulletUi01.png");
	assert(hImageGauge > 0);
	hImageGaugeRemaining = LoadGraph("data/image/bulletUi02.png");
	assert(hImageGaugeRemaining > 0);
	hImageEffectOutBullet = LoadGraph("data/image/right.png");
	assert(hImageEffectOutBullet > 0);
}

void BULLET::Update()
{

}

void BULLET::Draw()
{
	DrawRemainingSetting(remainingSetting); // 残弾数のゲージの表示

	// 残弾数の表示分岐処理入れて
	if (remainingAll <= 0 && remainingSetting == 0)
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
		DrawFormatString(100, 200, GetColor(0, 0, 0), "%04d", remainingSetting + remainingAll); // 残弾数の表示
	}
}

// 残弾数をGetして残弾数の分,表示する円ゲージ
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
		OutBulletEffect();
		return remainingSetting;
	}
	else
	{
		// 銃弾がない場合の処理
		return -1;
	}
}

void BULLET::OutBulletEffect()
{
	// 銃を発砲したエフェクト画像　最終的にはライトで光らせる処理をいれたい
	DrawGraph(-10, -10, hImageEffectOutBullet, TRUE); // 位置をマイナスにしているのは入れた画像の端が黒くなってたから
}

void BULLET::ReloadBullet()
{
	if (!(remainingAll == remainingSetting))
	{
		int canSetNum = BULLET::MAX_SETTING_BULET - remainingSetting; // 装填可能数

		if (canSetNum >= remainingAll) // 装填可能数 >= 残弾数
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
}

void BULLET::AddBullet(int addNumber)
{
	remainingAll += addNumber;
}
