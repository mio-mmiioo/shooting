#pragma once
#include "../Actor.h"

class Enemy : public Actor
{
public:
	Enemy(const VECTOR3& position, float ang, int hp);
	Enemy() {};
	~Enemy() override;
	void Update() override;
	void Draw() override;

	void SetIsArrive(bool isArrive) { isArrive_ = isArrive; }
	bool GetIsArrive() { return isArrive_; }
	void SetIsSetNextPos(bool isSetNextPos) { IsSetNextPos_ = isSetNextPos; }
	void SetGoPosition(VECTOR3 goPosition);
	VECTOR3 GetGoPosition() { return goPosition_; }
	bool GetIsSetNextPos() { return IsSetNextPos_; }

	void AddEnemy(Enemy* e);

// 継承先でも使用したい関数
protected:
	void UpdateWalk();
	void UpdateStay();
	void UpdateAttack();

	void DevelopmentInput(); // 開発時のみ使用する、キー入力の処理

	// 移動関連
	VECTOR3 goPosition_;  // 次に向かう場所
	bool isArrive_;
	bool IsSetNextPos_;

	// 攻撃関連
	float attackTimer_; // 攻撃の間隔
	int attackPower_; // 攻撃力

private:
	enum E_STATE {
		STAY,
		WALK,
		ATTACK,
		MAX_STATE
	};

	E_STATE state_;
	float timer_;

	std::list<Enemy*> enemyList_;
};