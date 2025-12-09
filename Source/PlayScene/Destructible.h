#pragma once
#include "../../Library/Object3D.h"

class Destructible : public Object3D
{
public:
	Destructible();
	Destructible(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~Destructible() override;
	virtual void Update() override;
	virtual void addHp(int num) { hp_ += num; } // 体力の増減はこれで管理

	bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr) const override;
	void CheckLinePush(VECTOR3& pos1, VECTOR3 pos2, float minDistance);
	VECTOR3 GetGravity() { return gravity_; }
	void SetPosition(VECTOR3 newPosition) { transform_.position_ = newPosition; }
	float GetDistanceR() { return distanceR_; }
	void SetMove(VECTOR3 toPosition);

	void AddDestructibleList(Destructible* dest);
	std::list<Destructible*> GetDestructibleObject() { return destructibleList_; }

protected:
	std::list<Destructible*> destructibleList_;

	int hp_;		// 体力
	bool isAlive_;	// 生きてる？　生きていたらtrue 死んでいたらfalse
	float time_;	// 空中にいる時間
	VECTOR3 gravity_; // 重力

	// 当たり判定関連
	float distanceR_; // 当たり判定に使用する半径
	float moveSpeed_;
	float rotateSpeed_; // 回転速度
};
