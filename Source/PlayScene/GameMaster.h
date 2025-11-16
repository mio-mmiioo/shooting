#pragma once
#include "../../Library/Object3D.h"

class Enemy;

namespace GameMaster {
	void Init();
	void Update();
	void Release();

	// ŒÄ‚Ño‚³‚ê‚éŠÖ”
	bool IsBulletHitEnemy(VECTOR3 startPos, VECTOR3 endPos); // e’e‚ª“G‚É“–‚½‚é‚© ‚ ‚½‚é‚È‚çtrue
	void CheckSetPosition(Transform& transform, float time, VECTOR3 gravity, float distanceR); // ˆÊ’u’²®‚ğ‚·‚é
	bool IsCanAttackPlayer(Enemy* enemy);
	void AttackPlayer(int atackPower);
}