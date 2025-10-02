#pragma once
#include "../../Library/Object3D.h"

class Actor : public Object3D
{
public:
	Actor() {};
	Actor(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~Actor();
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Attack() {}; // UŒ‚‚ğ‚·‚é
	virtual void addHp(int num) { hp_ += num; } // ‘Ì—Í‚Ì‘Œ¸‚Í‚±‚ê‚ÅŠÇ—

private:
	int hp_; // ‘Ì—Í
	bool isAlive_; // ¶‚«‚Ä‚éH@¶‚«‚Ä‚¢‚½‚çtrue €‚ñ‚Å‚¢‚½‚çfalse

};
