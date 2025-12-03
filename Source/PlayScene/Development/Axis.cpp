#include "Axis.h"
#include "../../Color.h"

Axis::Axis()
{
}

Axis::~Axis()
{
}

void Axis::Draw()
{
	// xyzé≤
	DrawLine3D(VECTOR3(-100, 0, 0), VECTOR3(100, 0, 0), Color::RED); // xé≤
	DrawCone3D(VECTOR3(100, 0, 0), VECTOR3(80, 0, 0), 10, 20, Color::RED, Color::RED, 1);
	DrawLine3D(VECTOR3(0, -100, 0), VECTOR3(0, 100, 0), Color::BLUE); // yé≤
	DrawCone3D(VECTOR3(0, 100, 0), VECTOR3(0, 80, 0), 10, 20, Color::BLUE, Color::BLUE, 1);
	DrawLine3D(VECTOR3(0, 0, -100), VECTOR3(0, 0, 100), Color::GREEN); // zé≤
	DrawCone3D(VECTOR3(0, 0, 100), VECTOR3(0, 0, 80), 10, 20, Color::GREEN, Color::GREEN, 1);
}