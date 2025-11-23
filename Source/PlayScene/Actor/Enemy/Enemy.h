#pragma once
#include "../Actor.h"
#include "../../../../Library/Animator.h"
#include <vector>

class Enemy : public Actor
{
public:
	Enemy(const VECTOR3& position, float ang, int hp);
	~Enemy() override;
	void Update() override;
	void Draw() override;

	void SetToGo(VECTOR3 goPosition) { goPosition_ = goPosition; }
	void SetIsArrive(bool isArrive) { isArrive_ = isArrive; }
	void SetPosList(std::vector<VECTOR3> posList);
	std::vector<VECTOR3> GetPosList() { return posList_; }
	
	bool isAttacked_;

private:
	void UpdateWalk();
	void UpdateStay();
	void UpdateAttack();

	void AutoMove(); // 経路探索をもとに自動移動する

	enum ANIM_ID {
		A_NEUTRAL = 0,
		A_IDLE,
		A_WALK,
		A_ATTACK,
		A_MAX
	};

	Animator* animator_;

	enum E_STATE {
		STAY,
		WALK,
		ATTACK,
		MAX_STATE
	};

	std::vector<VECTOR3> posList_; // プレイヤーまでの距離
	VECTOR3 goPosition_;  // 次に向かう場所
	VECTOR3 endPosition_; // 最終目的地
	bool isArrive_;
	bool isNextSetPosition_;
	E_STATE state_;
	float timer_;

	// 攻撃関連
	float attackTimer_; // 攻撃の間隔
};