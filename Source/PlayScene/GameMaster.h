#pragma once
#include "../../Library/Object3D.h"

class Enemy;

namespace GameMaster {
	void Init();
	void Update();
	void Draw();
	void Release();

	// 呼び出される関数
	void PlayerDeath(); // プレイヤーが死んだときに呼び出す関数

	bool IsBulletHit(VECTOR3 startPos, VECTOR3 endPos); // これを呼び出せば、当たり判定完了
	void CheckSetPosition(Transform& transform, float time, VECTOR3 gravity, float distanceR); // 位置調整をする
	bool IsCanAttackPlayer(Enemy* enemy);
	void AttackPlayer(int atackPower);
	float GetDistanceToPlayer(float distance); // プレイヤーと引数の距離を返す関数
}