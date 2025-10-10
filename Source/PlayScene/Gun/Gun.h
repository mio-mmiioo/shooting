#pragma once
#include "../../../Library/Object3D.h"

class Gun : public Object3D
{
public:
	Gun();
	~Gun();
	void Update() override;
	void Draw() override;

	void DrawRemainingSetting(int currentRemainingSetting);
	int OutBullet();
	void OutBulletEffect();
	void ReloadBullet();
	void AddGun(int addNumber);

private:
	int remainingAll_;				// c’e”
	int remainingSetting_;			// e‚É‘•‚Ä‚ñ‚³‚ê‚Ä‚¢‚éc’e”@–¼‘OŒã‚Å‚í‚©‚é‚æ‚¤‚É•Ï‚¦‚Ä

	int hImageGauge_;				// e’e‚Ì‘•“U”‚ÌƒQ[ƒW
	int hImageGaugeRemaining_;		// c’e”‚ğ¦‚·ƒQ[ƒW
	int hImageReload_;				// ƒŠƒ[ƒh‚ğ¦‚·ƒQ[ƒW
	int hImageEffectOutBullet_;		// ”­–CŒã‚É‰æ–Ê‚ğŒõ‚ç‚¹‚é‚½‚ß‚Ì‰æ‘œ

	enum GUN_TYPE {
		NORMAL,
		MACHINE,
		MAX_GUN_TYPE
	};
};
