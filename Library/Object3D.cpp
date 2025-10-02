#include "Object3D.h"

Object3D::Object3D()
	:hModel_(-1)
{
}

Object3D::~Object3D()
{
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
}

void Object3D::SetMove(VECTOR3 toPosition, float angSpeed, float moveSpeed)
{
	VECTOR3 toGo = toPosition - transform_.position;

	VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(transform_.rotation.y);//³–Ê
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(transform_.rotation.y);//‰E ‰ñ“]‚ðŒ©‚é‚Ì‚ÉŽg‚Á‚Ä‚é

	if (VDot(front, toGo.Normalize()) >= cos(angSpeed))
	{
		transform_.rotation.y = atan2f(toGo.x, toGo.z);
	}
	else if (VDot(right, toGo) > 0)
	{
		transform_.rotation.y += angSpeed;
	}
	else
	{
		transform_.rotation.y -= angSpeed;
	}

	transform_.position += VECTOR3(0, 0, moveSpeed) * MGetRotY(transform_.rotation.y);
}
