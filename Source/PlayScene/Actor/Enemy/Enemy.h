#pragma once
#include "../Actor.h"
#include <vector>

class Enemy : public Actor
{
public:
	Enemy(const VECTOR3& position, float ang, int hp);
	~Enemy() override;
	void Update() override;
	void Draw() override;

	void SetIsArrive(bool isArrive) { isArrive_ = isArrive; }
	bool GetIsArrive() { return isArrive_; }
	void SetIsSetNextPos(bool isSetNextPos) { IsSetNextPos_ = isSetNextPos; }
	void SetPosList(std::vector<VECTOR3> posList);
	bool GetIsSetNextPos() { return IsSetNextPos_; }

private:
	void UpdateWalk();
	void UpdateStay();
	void UpdateAttack();

	void AutoMove(); // 経路探索をもとに自動移動する

	void DrawPosList(); // 開発時のみ使用

	enum E_STATE {
		STAY,
		WALK,
		ATTACK,
		MAX_STATE
	};

	std::vector<VECTOR3> posList_; // プレイヤーまでの距離
	VECTOR3 goPosition_;  // 次に向かう場所
	bool isArrive_;
	bool IsSetNextPos_;
	E_STATE state_;
	float timer_;

	// 攻撃関連
	float attackTimer_; // 攻撃の間隔
};