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
	void SetPosList(std::vector<VECTOR2> posList) { posList_ = posList; }
	
	bool isAttacked_;

private:
	void UpdateWalk();
	void UpdateStay();
	void UpdateAttack();

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

	std::vector<VECTOR2> posList_; // ÉvÉåÉCÉÑÅ[Ç‹Ç≈ÇÃãóó£
	VECTOR3 goPosition_;
	bool isArrive_;
	E_STATE state_;
	float timer_;

	// çUåÇä÷òA
	float attackTimer_; // çUåÇÇÃä‘äu
};