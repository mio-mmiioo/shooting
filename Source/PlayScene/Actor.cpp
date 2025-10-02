#include "Actor.h"

Actor::Actor(const VECTOR3& position, float ang, int hModel, int hp)
	:isAlive_(true),
	hp_(hp)
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

void Actor::Draw()
{
}
