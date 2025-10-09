#include "Actor.h"

Actor::Actor()
{
}

Actor::Actor(const VECTOR3& position, float ang, int hModel, int hp)
	:isAlive_(true),
	hp_(hp),
	time_(0)
{
	transform_.position_ = position;
	hModel_ = hModel;
}

Actor::~Actor()
{
}

void Actor::Update()
{
	if (hp_ <= 0)
	{
		isAlive_ = false;
	}
}

void Actor::SetMove(VECTOR3 toPosition, float angSpeed, float moveSpeed)
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

bool Actor::CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit) const 
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2 - pos1)).Size();
	std::list<Actor*> act = FindGameObjects<Actor>();
	for (Actor* ac : act)
	{
		VECTOR3 ret;
		if (ac->Object3D::CollideLine(pos1, pos2, &ret))
		{
			found = true;
			VECTOR3 v = pos1 - ret;
			float len = v.Size();
			if (len < nowVal)
			{
				nowVal = len;
				now = ret;
			}
		}
	}
	if (hit != nullptr)
	{
		*hit = now;
	}
	return found;
}
