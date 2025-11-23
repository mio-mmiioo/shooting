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

	bool IsBulletHitEnemy(VECTOR3 startPos, VECTOR3 endPos); // 銃弾が敵に当たるか あたるならtrue
	void CheckSetPosition(Transform& transform, float time, VECTOR3 gravity, float distanceR); // 位置調整をする
	bool IsCanAttackPlayer(Enemy* enemy);
	void AttackPlayer(int atackPower);
}