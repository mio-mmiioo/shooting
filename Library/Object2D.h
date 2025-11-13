#pragma once
#include "GameObject.h"

class Object2D : public GameObject
{
public:
	Object2D();
	Object2D(VECTOR2 position, int hImage);
	virtual ~Object2D();
	virtual void Update() override; // ‚±‚±‚Åd—Í‚ğ‚©‚¯‚Ä‚¢‚«‚½‚¢
	virtual void Draw() override;
	VECTOR2 GetPosition() { return position_; }

protected:
	int hImage_; // ‰æ‘œƒnƒ“ƒhƒ‹
	VECTOR2 position_;
};