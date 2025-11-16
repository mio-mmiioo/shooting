#include "Actor.h"

Actor::Actor()
	:isAlive_(true),
	hp_(10),
	time_(0)
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

bool Actor::CollideCapsule(VECTOR3 pos1, VECTOR3 pos2, float minDistance, Actor* act)
{
	bool found = false;
	std::list<Actor*> actor = FindGameObjects<Actor>();
	for (Actor* ac : actor)
	{
		if (ac != act)
		{
			float min = ac->GetDistanceR() + minDistance; // ぶつかる距離
			VECTOR3 cap2 = ac->transform_.position_ + VECTOR3(0, 1, 0) * 180 * ac->transform_.GetRotationMatrix();
			float checkDistance = Segment_Segment_MinLength(pos1, pos2, ac->transform_.position_, cap2); // 二つの線分の最近点間の距離を得る
			if (checkDistance < minDistance) // ぶつかっている
			{
				found = true;
			}
		}
	}
	return found;
}

void Actor::CheckLinePush(VECTOR3& pos1, VECTOR3 pos2, float minDistance)
{
	VECTOR3 hit;
	VECTOR3 direction;
	if (CollideLine(pos1, pos2, &hit)) // 正面にオブジェクトがある
	{
		if (VSize(pos1 - hit) < minDistance) // めり込んでいる→めり込んでいる距離押し返す
		{
			direction = VNorm(hit - pos1); // 押し返す方向のベクトル
			pos1 -= direction * (minDistance - VSize(pos1 - hit)); // ( 押し返す方向 ) * ( 押し返したい距離 )
		}
	}
}

void Actor::CheckCapsulePush(VECTOR3 pos1, VECTOR3 pos2, float minDistance, Actor* act)
{
	
}
