#include "Right.h"
#include "DxLib.h"

namespace Right {
	VECTOR rightDirection;
}

void Right::Init()
{
	rightDirection = { -1,-1,1 };

	ChangeLightTypeDir(rightDirection);
}

void Right::Update()
{

}

VECTOR Right::GetRightDirection()
{
	return rightDirection;
}
