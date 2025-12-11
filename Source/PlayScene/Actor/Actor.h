#pragma once
#include "../CanShoot.h"

class Actor : public CanShoot
{
public:
	Actor();
	Actor(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~Actor();
	virtual void Update() override;
	virtual int Attack() { return 0; }			// UŒ‚‚ğ‚·‚é
	virtual void addHp(int num) { hp_ += num; } // ‘Ì—Í‚Ì‘Œ¸‚Í‚±‚ê‚ÅŠÇ—

protected:

};
