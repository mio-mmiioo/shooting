#pragma once
#include "../../Library/Object3D.h"

class Stage;

class Actor : public Object3D
{
public:
	Actor();
	Actor(const VECTOR3& position, float ang, int hModel, int hp);
	virtual ~Actor();
	virtual void Update() override;
	virtual int Attack() { return 0; } // 攻撃をする
	virtual void addHp(int num) { hp_ += num; } // 体力の増減はこれで管理
	virtual void SetMove(VECTOR3 toPosition, float angSpeed, float moveSpeed); // 移動したい場所, 回転スピード, 動くスピード

	bool CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit = nullptr) const override;

protected:
	Stage* stage_;
	int hp_; // 体力
	bool isAlive_; // 生きてる？　生きていたらtrue 死んでいたらfalse
	float time_; // 空中にいる時間

};
