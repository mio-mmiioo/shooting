#pragma once
#include "../../../Library/Object3D.h"

class Gun : public Object3D
{
	struct gun {
		int remainingAll_;				// 残弾数
		const int MAX_SETTING;			// 銃に装填できる最大数
		int remainingSetting_;			// 銃に装てんされている残弾数　名前後でわかるように変えて
		int hImageGauge_;				// 銃弾の装填数のゲージ
		int hImageGaugeRemaining_;		// 残弾数を示すゲージ
	};

public:
	Gun();
	~Gun();
	void Update() override;
	void Draw() override;

	void DrawRemainingSetting(struct gun* g);
	int OutBullet();
	void OutBulletEffect();
	void ReloadBullet();
	void AddGun(int addNumber);

protected:
	int remainingAll_;				// 残弾数
	int remainingSetting_;			// 銃に装てんされている残弾数　名前後でわかるように変えて

	int hImageGauge_;				// 銃弾の装填数のゲージ
	int hImageGaugeRemaining_;		// 残弾数を示すゲージ
	int hImageReload_;				// リロードを示すゲージ

private:
	enum GUN_TYPE {
		HAND,		// ハンドガン/拳銃
		MACHINE,	// マシンガン/機関銃
		// ショットガン　散弾銃
		// グレネードランチャー
		MAX_GUN_TYPE
	};

	GUN_TYPE gunType_;

	//gun hand;
	//gun machine;

	int hImageEffectOutBullet_;		// 発砲後に画面を光らせるための画像
};
