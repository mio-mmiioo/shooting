#pragma once
#include "../Actor.h"

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
	void SetGoPosition(VECTOR3 goPosition);
	VECTOR3 GetGoPosition() { return goPosition_; }
	bool GetIsSetNextPos() { return IsSetNextPos_; }

private:
	void UpdateWalk();
	void UpdateStay();
	void UpdateAttack();

	void AutoMove(); // Œo˜H’Tõ‚ğ‚à‚Æ‚É©“®ˆÚ“®‚·‚é

	enum E_STATE {
		STAY,
		WALK,
		ATTACK,
		MAX_STATE
	};

	VECTOR3 goPosition_;  // Ÿ‚ÉŒü‚©‚¤êŠ
	bool isArrive_;
	bool IsSetNextPos_;
	E_STATE state_;
	float timer_;

	// UŒ‚ŠÖ˜A
	float attackTimer_; // UŒ‚‚ÌŠÔŠu
	int attackPower_; // UŒ‚—Í
};