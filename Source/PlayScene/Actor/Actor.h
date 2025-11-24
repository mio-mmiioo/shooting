#pragma once
#include "../../../Library/Object3D.h"

class Stage;

class Actor : public Object3D
{
public:
	Actor();
	Actor(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~Actor();
	virtual void Update() override;
	virtual int Attack() { return 0; }			// 攻撃をする
	virtual void addHp(int num) { hp_ += num; } // 体力の増減はこれで管理

	bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr) const override;
	void CheckLinePush(VECTOR3& pos1, VECTOR3 pos2, float minDistance);
	VECTOR3 GetGravity() { return gravity_; }
	void SetPosition(VECTOR3 newPosition) { transform_.position_ = newPosition; }
	float GetDistanceR() { return distanceR_; }
	void SetMove(VECTOR3 toPosition);

protected:
	Stage* stage_;
	int hp_;		// 体力
	bool isAlive_;	// 生きてる？　生きていたらtrue 死んでいたらfalse
	float time_;	// 空中にいる時間
	VECTOR3 gravity_; // 重力

	// 当たり判定関連
	float distanceR_; // 当たり判定に使用する半径
	float moveSpeed_;
	float rotateSpeed_; // 回転速度
};
