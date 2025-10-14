#pragma once
#include "../../../Library/Object3D.h"

namespace GUN
{
	enum TYPE {
		HAND,		// ƒnƒ“ƒhƒKƒ“/Œe
		MACHINE,	// ƒ}ƒVƒ“ƒKƒ“/‹@ŠÖe
		// ƒVƒ‡ƒbƒgƒKƒ“@U’ee
		// ƒOƒŒƒl[ƒhƒ‰ƒ“ƒ`ƒƒ[
		MAX_TYPE
	};
}

class Gun : public Object3D
{
	struct gun {
		GUN::TYPE type;					// e‚Ìí—Ş
		int remainingAll;				// c’e”
		int maxSetting;					// e‚É‘•“U‚Å‚«‚éÅ‘å”
		int remainingSetting;			// e‚É‘•‚Ä‚ñ‚³‚ê‚Ä‚¢‚éc’e”@–¼‘OŒã‚Å‚í‚©‚é‚æ‚¤‚É•Ï‚¦‚Ä
		int hImageGauge;				// e’e‚Ì‘•“U”‚ÌƒQ[ƒW
		int hImageGaugeRemaining;		// c’e”‚ğ¦‚·ƒQ[ƒW
	};

public:
	Gun();
	~Gun();
	void Update() override;
	void Draw() override;

	void DrawRemainingSetting();
	int OutBullet();
	void OutBulletEffect();
	void ReloadBullet();
	void AddGun(int addNumber);
	void SetGunType(GUN::TYPE type);

private:
	GUN::TYPE gunType_;

	struct gun current;
	struct gun hand;
	struct gun machine;

	int hImageEffectOutBullet_;		// ”­–CŒã‚É‰æ–Ê‚ğŒõ‚ç‚¹‚é‚½‚ß‚Ì‰æ‘œ
};
