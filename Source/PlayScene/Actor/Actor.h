#pragma once
#include "../../../Library/Object3D.h"

class Stage;

class Actor : public Object3D
{
public:
	Actor();
	Actor(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~Actor();
	virtual void Update() override;
	virtual int Attack() { return 0; }			// UŒ‚‚ğ‚·‚é
	virtual void addHp(int num) { hp_ += num; } // ‘Ì—Í‚Ì‘Œ¸‚Í‚±‚ê‚ÅŠÇ—

	bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr) const override;

protected:
	Stage* stage_;
	int hp_;		// ‘Ì—Í
	bool isAlive_;	// ¶‚«‚Ä‚éH@¶‚«‚Ä‚¢‚½‚çtrue €‚ñ‚Å‚¢‚½‚çfalse
	float time_;	// ‹ó’†‚É‚¢‚éŠÔ

};
