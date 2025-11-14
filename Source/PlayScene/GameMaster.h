#pragma once
#include "../../Library/Object3D.h"

namespace GameMaster {
	void Init();
	void Update();
	void SetPlayerPos();
	void SetEnemyPos();

	bool IsBulletHitEnemy(VECTOR3 startPos, VECTOR3 endPos); // e’e‚ª“G‚É“–‚½‚é‚© ‚ ‚½‚é‚È‚çtrue
}