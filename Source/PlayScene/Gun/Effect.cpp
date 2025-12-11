#include "Effect.h"
#include "../../Color.h"

Effect::Effect(VECTOR3 effectPos, GUN::TYPE type)
{
	transform_.position_ = effectPos;
	type_ = type;
	timer_ = 5.0f;
}

Effect::~Effect()
{
}

void Effect::Update()
{
	timer_ -= Time::DeltaTime();
	if (timer_ < 0)
	{
		DestroyMe();
	}
}

void Effect::Draw()
{
	switch (type_)
	{
	case GUN::TYPE::HAND:
		Hand();
		break;
	case GUN::TYPE::MACHINE:
		Machine();
		break;
	}
}

void Effect::Hand()
{
	DrawSphere3D(transform_.position_, 20, 20, Color::HAND, Color::HAND, TRUE);
}

void Effect::Machine()
{
	DrawSphere3D(transform_.position_, 20, 20, Color::MACHINE, Color::MACHINE, TRUE);
}
