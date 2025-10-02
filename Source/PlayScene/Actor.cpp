#include "Actor.h"

Actor::Actor(VECTOR3 position, int hModel, int hp)
	:isAlive_(true),
	hp_(hp)
{
	GetTransform().position = position; // これで変更できたっけ？？できてないかも確認して
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
