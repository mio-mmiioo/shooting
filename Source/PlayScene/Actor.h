#pragma once
#include "../../Library/Object3D.h"

class Actor : public Object3D
{
public:
	Actor() {};
	Actor(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~Actor();
	virtual void Update() override;
	virtual void Attack() {}; // 攻撃をする
	virtual void addHp(int num) { hp_ += num; } // 体力の増減はこれで管理
	virtual void SetMove(VECTOR3 toPosition, float angSpeed, float moveSpeed); // 移動したい場所, 回転スピード, 動くスピード

private:
	int hp_; // 体力
	bool isAlive_; // 生きてる？　生きていたらtrue 死んでいたらfalse

};
