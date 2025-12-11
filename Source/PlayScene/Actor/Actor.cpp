#include "Actor.h"

Actor::Actor()
{
}

Actor::Actor(const VECTOR3& position, float ang, int hModel, int hp)
{
	isAlive_ = true;
	hp_ = hp;
	time_ = 0.0f;
	transform_.position_ = position;
	hModel_ = hModel;
}

Actor::~Actor()
{
	if (hModel_ > 0)
	{
		hModel_ = -1;
	}
}

void Actor::Update()
{
	if (hp_ <= 0)
	{
		isAlive_ = false;
	}
}



