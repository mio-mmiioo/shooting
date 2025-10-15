#include "Light.h"
#include "DxLib.h"

namespace Light {
	VECTOR lightDirection; // 影をつけるのに使用しているライト
}

void Light::Init()
{
	lightDirection = { -1, -2, 1 };
	ChangeLightTypeDir(lightDirection);
}

void Light::Update()
{

}

VECTOR Light::GetLightDirection()
{
	return lightDirection;
}