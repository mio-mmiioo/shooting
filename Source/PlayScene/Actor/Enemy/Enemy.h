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

	void SetIsArrive(bool isArrive) { isArrive_ = isArrive; }
	void SetPosList(std::vector<VECTOR3> posList);
	
	bool isAttacked_;

private:
	void UpdateWalk();
	void UpdateStay();
	void UpdateAttack();

	void AutoMove(); // Œo˜H’Tõ‚ğ‚à‚Æ‚É©“®ˆÚ“®‚·‚é

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

	std::vector<VECTOR3> posList_; // ƒvƒŒƒCƒ„[‚Ü‚Å‚Ì‹——£
	VECTOR3 goPosition_;  // Ÿ‚ÉŒü‚©‚¤êŠ
	bool isArrive_;
	E_STATE state_;
	float timer_;

	// UŒ‚ŠÖ˜A
	float attackTimer_; // UŒ‚‚ÌŠÔŠu
};