#pragma once
#include "../../../Library/Object3D.h"
#include "Gun.h"

class Effect : public Object3D
{
public:
	Effect(VECTOR3 effectPos, GUN::TYPE type);
	~Effect();

	void Update() override;
	void Draw() override;
private:
	GUN::TYPE type_;
	float timer_; // エフェクトが表示される時間

	void Hand();		// 拳銃のエフェクト
	void Machine();		// マシンガンのエフェクト
};
