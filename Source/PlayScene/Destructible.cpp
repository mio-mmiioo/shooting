#include "Destructible.h"

Destructible::Destructible()
	:isAlive_(true),
	hp_(10),
	time_(0)
{
}

Destructible::Destructible(const VECTOR3& position, float ang, int hModel, int hp)
	:isAlive_(true),
	hp_(hp),
	time_(0)
{
	transform_.position_ = position;
	hModel_ = hModel;
}

Destructible::~Destructible()
{
	if (hModel_ > 0)
	{
		hModel_ = -1;
	}
}

void Destructible::Update()
{
	if (hp_ <= 0)
	{
		isAlive_ = false;
	}
}

bool Destructible::CollideLine(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit)
{
	prevBulletStartPos_ = pos1;
	prevBulletEndPos_ = pos2;
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2 - pos1)).Size();
	std::list<Destructible*> destructible = FindGameObjects<Destructible>();
	for (Destructible* dest : destructible)
	{
		if (dest != player_)
		{
			VECTOR3 ret;
			if (dest->Object3D::CollideLine(pos1, pos2, &ret))
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
	}
	if (hit != nullptr)
	{
		*hit = now;
	}
	return found;
}

void Destructible::CheckLinePush(VECTOR3& pos1, VECTOR3 pos2, float minDistance)
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

void Destructible::SetMove(VECTOR3 toPosition)
{
	VECTOR3 toGo = toPosition - transform_.position_;

	VECTOR3 front = VECTOR3(0, 0, 1) * MGetRotY(transform_.rotation_.y); // 正面
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(transform_.rotation_.y); // 右 回転を見るのに使ってる

	if (VDot(front, toGo.Normalize()) >= cos(rotateSpeed_))
	{
		transform_.rotation_.y = atan2f(toGo.x, toGo.z);
	}
	else if (VDot(right, toGo) > 0)
	{
		transform_.rotation_.y += rotateSpeed_;
	}
	else
	{
		transform_.rotation_.y -= rotateSpeed_;
	}

	transform_.position_ += VECTOR3(0, 0, moveSpeed_) * MGetRotY(transform_.rotation_.y);
}

Destructible* Destructible::CheckHitDestructible(VECTOR3* hit)
{
	Destructible* retDestructible = nullptr;
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(prevBulletEndPos_ - prevBulletStartPos_)).Size();
	std::list<Destructible*> destructible = FindGameObjects<Destructible>();
	for (Destructible* dest : destructible)
	{
		if (dest != player_)
		{
			VECTOR3 ret;
			if (dest->Object3D::CollideLine(prevBulletStartPos_, prevBulletEndPos_, &ret))
			{
				found = true;
				VECTOR3 v = prevBulletStartPos_ - ret;
				float len = v.Size();
				if (len < nowVal)
				{
					nowVal = len;
					now = ret;
					retDestructible = dest;
				}
			}
		}
	}
	if (hit != nullptr)
	{
		*hit = now;
	}
	return retDestructible;
}
