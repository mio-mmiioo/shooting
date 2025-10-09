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
	unsigned int green = GetColor(0, 255, 0);
	unsigned int blue = GetColor(0, 0, 255);

	// xyzé≤
	DrawLine3D(VECTOR3(-100, 0, 0), VECTOR3(100, 0, 0), red); // xé≤
	DrawCone3D(VECTOR3(100, 0, 0), VECTOR3(80, 0, 0), 10, 20, red, red, 1);
	DrawLine3D(VECTOR3(0, -100, 0), VECTOR3(0, 100, 0), blue); // yé≤
	DrawCone3D(VECTOR3(0, 100, 0), VECTOR3(0, 80, 0), 10, 20, blue, blue, 1);
	DrawLine3D(VECTOR3(0, 0, -100), VECTOR3(0, 0, 100), green); // zé≤
	DrawCone3D(VECTOR3(0, 0, 100), VECTOR3(0, 0, 80), 10, 20, green, green, 1);

	// ínñ 
	//VECTOR3 pos1 = { -100, 0, -100 };
	//VECTOR3 pos2 = { -100, 0,  100 };
	//VECTOR3 pos3 = { 100,  0,  100 };
	//VECTOR3 pos4 = { 100,  0, -100 };
	//int color = GetColor(100, 150, 100);
	//DrawTriangle3D(pos1, pos2, pos3, color, TRUE);
	//DrawTriangle3D(pos3, pos4, pos1, color, TRUE);

	// ñ⁄àÛ

}