#pragma once
#include "../../../Library/Object2D.h"

class HP : public Object2D {
public:
	HP(int hp);
	~HP();
	void Update() override;
	void Draw() override;
	int GetHP() { return hp_; }
	void AddHP(int addHp);

private:
	int hp_; // 体力
	int maxHp_; // 体力の最大値

	// 画像
	int barFrameImage_; // HPゲージの枠
	int barGreenImage_; // HPゲージ・緑色
	int barRedImage_;	// HPゲージ・赤色
};

