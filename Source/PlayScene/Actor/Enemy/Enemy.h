#pragma once
#include "../Actor.h"

class Enemy : public Actor
{
public:
	Enemy(const VECTOR3& position, float ang, int hp);
	~Enemy() override;
	void Update() override;
	void Draw() override;

	void SetToGo(VECTOR3 goPosition) { goPosition_ = goPosition; }
	void SetIsArrive(bool isArrive) { isArrive_ = isArrive; }
	
	bool isAttacked_;

private:
	void UpdateWalk();
	void UpdateStay();
	void UpdateAttack();


	enum E_STATE {
		WALK,
		STAY,
		ATTACK,
		MAX_STATE
	};

	VECTOR3 goPosition_;
	bool isArrive_;
	E_STATE state_;
	float timer_;

	// çUåÇä÷òA
	float attackTimer_; // çUåÇÇÃä‘äu
};