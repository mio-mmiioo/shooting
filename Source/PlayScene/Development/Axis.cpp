#include "Axis.h"

Axis::Axis()
{
}

Axis::~Axis()
{
}

void Axis::Draw()
{
	unsigned int red = GetColor(255, 0, 0);
	unsigned int blue = GetColor(0, 255, 0);
	unsigned int green = GetColor(0, 0, 255);

	DrawLine3D(VECTOR3(-100, 0, 0), VECTOR3(100, 0, 0), red); // xé≤
	DrawCone3D(VECTOR3(100, 0, 0), VECTOR3(80, 0, 0), 10, 20, red, red, 1);
	DrawLine3D(VECTOR3(0, -100, 0), VECTOR3(0, 100, 0), blue); // yé≤
	DrawCone3D(VECTOR3(0, 100, 0), VECTOR3(0, 80, 0), 10, 20, blue, blue, 1);
	DrawLine3D(VECTOR3(0, 0, -100), VECTOR3(0, 0, 100), GetColor(0, 0, 255)); // zé≤
	DrawCone3D(VECTOR3(0, 0, 100), VECTOR3(0, 0, 80), 10, 20, GetColor(0, 0, 255), GetColor(0, 0, 255), 1);

	VECTOR3 pos1 = { -100, 0, -100 };
	VECTOR3 pos2 = { -100, 0,  100 };
	VECTOR3 pos3 = { 100,  0,  100 };
	VECTOR3 pos4 = { 100,  0, -100 };
	DrawTriangle3D()
}