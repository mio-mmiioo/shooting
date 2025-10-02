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
	VECTOR3 toGo = toPosition - transform_.position_;

	VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(transform_.rotation_.y);//³–Ê
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(transform_.rotation_.y);//‰E ‰ñ“]‚ðŒ©‚é‚Ì‚ÉŽg‚Á‚Ä‚é

	if (VDot(front, toGo.Normalize()) >= cos(angSpeed))
	{
		transform_.rotation_.y = atan2f(toGo.x, toGo.z);
	}
	else if (VDot(right, toGo) > 0)
	{
		transform_.rotation_.y += angSpeed;
	}
	else
	{
		transform_.rotation_.y -= angSpeed;
	}

	transform_.position_ += VECTOR3(0, 0, moveSpeed) * MGetRotY(transform_.rotation_.y);
}
