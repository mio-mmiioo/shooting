#pragma once
#include "../../Library/Object3D.h"

class CanShoot : public Object3D
{
public:
	CanShoot();
	CanShoot(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~CanShoot() override;
	virtual void Update() override;
	virtual void addHp(int num) { hp_ += num; } // 体力の増減はこれで管理

	bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr);
	void CheckLinePush(VECTOR3& pos1, VECTOR3 pos2, float minDistance);
	VECTOR3 GetGravity() { return gravity_; }
	void SetPosition(VECTOR3 newPosition) { transform_.position_ = newPosition; }
	float GetDistanceR() { return distanceR_; }
	void SetMove(VECTOR3 toPosition);

	CanShoot* CheckHitCanShoot(VECTOR3* hit);

protected:
	void SetPlayer(CanShoot* player) { player_ = player; }

	int hp_;		// 体力
	int point_;		// ポイント　倒された時のポイント
	bool isAlive_;	// 生きてる？　生きていたらtrue 死んでいたらfalse
	float time_;	// 空中にいる時間
	VECTOR3 gravity_; // 重力

	// 当たり判定関連
	float distanceR_; // 当たり判定に使用する半径
	float moveSpeed_;
	float rotateSpeed_; // 回転速度

private:
	CanShoot* player_;
	VECTOR3 prevBulletStartPos_;
	VECTOR3 prevBulletEndPos_;
};
