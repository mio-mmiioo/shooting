#pragma once
#include "../../Library/Object3D.h"

namespace GameMaster {
	void Init();
	void Update();
	bool IsChangeArea();
	void SetPlayerPos();

	VECTOR3 SetMove(Transform currentTransform, VECTOR3 toPosition, float angSpeed, float moveSpeed);
}