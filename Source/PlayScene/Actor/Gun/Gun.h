#pragma once
#include "../../../../Library/Object3D.h"

namespace GUN
{
	enum TYPE {
		HAND,		// ハンドガン/拳銃
		MACHINE,	// マシンガン/機関銃
		// ショットガン　散弾銃
		// グレネードランチャー
		MAX_TYPE
	};
}

class Gun : public Object3D
{
	struct gun {
		GUN::TYPE type;					// 銃の種類
		int attak;						// 攻撃力
		int remainingAll;				// 残弾数
		int maxSetting;					// 銃に装填できる最大数
		int remainingSetting;			// 銃に装てんされている残弾数
		int hImageGauge;				// 銃弾の装填数のゲージ
		int hImageGaugeRemaining;		// 残弾数を示すゲージ
		float reloadTimer;				// リロード時間を管理するためのタイマー
		float reloadTime;				// 武器のリロードにかかる時間
		float coolDownTimer;			// 連射を防ぐためのタイマー
		float coolDownTime;				// 次に撃てるようになるまでの時間
	};

public:
	Gun();
	~Gun();
	void Update() override;
	void Draw() override;

	void DrawRemainingSetting();
	bool OutBullet();
	void OutBulletEffect();
	void ReloadBullet();
	void AddBullet(int addNumber);
	void SetGunType(GUN::TYPE type);

	int GetAttack();
	float GetReloadTimer(); // 残りのリロードの時間
	float GetReloadTime();  // 現在の銃のリロードにかかる時間

private:
	GUN::TYPE gunType_;

	struct gun current;
	struct gun hand;
	struct gun machine;

	int hImageEffectOutBullet_;		// 発砲後に画面を光らせるための画像
};
