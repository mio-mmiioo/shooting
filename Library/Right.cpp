#include "Right.h"
#include "DxLib.h"

namespace Right {
	VECTOR rightDirection; // 影をつけるのに使用しているライト
}

void Right::Init()
{
	rightDirection = { -1, -2, 1 };
	ChangeLightTypeDir(rightDirection);
}

void Right::Update()
{

}

VECTOR Right::GetRightDirection()
{
	return rightDirection;
}
